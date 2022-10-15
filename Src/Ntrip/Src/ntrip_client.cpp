#include <memory>
#include <sys/types.h>
#include <stdexcept>

#include "ntrip_client.hpp"
#include "login_encode.hpp"
#include "nmea.hpp"
#include "mount_point.hpp"


namespace VrsTunnel::Ntrip
{
	std::unique_ptr<char[]> ntrip_client::build_request(const char* mountpoint,
						std::string name, std::string password)
	{
		const char* requestFormat = "GET /%s HTTP/1.0\r\n"
			"User-Agent: NTRIP PvvovanNTRIPClient/1.0.0\r\n"
			"Accept: */*\r\n" "Connection: close\r\n"
			"Authorization: Basic %s\r\n" "\r\n";
		
		std::unique_ptr<char[]> request;
		std::string auth{""};
		if (name.size() > 0) {
			std::unique_ptr<login_encode> encoder = login_encode::make_instance();
			auth = (*encoder).get(name, password);
		}
		request = std::make_unique<char[]>(::strlen(requestFormat) +
							::strlen(mountpoint) + auth.length() + 2);
		::sprintf(request.get(), requestFormat, mountpoint, auth.c_str());
		return request;
	}

	std::variant<std::vector<mount_point>, io_status>
	ntrip_client::getMountPoints(std::string address, uint16_t tcpPort,
			std::string name, std::string password)
	{
		tcp_client tc{};
		auto con_res = tc.connect(address, tcpPort);
		if (con_res != io_status::Success) {
			return con_res;
		}

		async_io aio{tc.get_sockfd()};
		std::unique_ptr<char[]> request = build_request("", name, password);
		
		auto res = aio.write(request.get(), strlen(request.get()));
		if (res != io_status::Success) {
			return res;
		}

		std::string responseRaw{};
		for(int i = 0; i < 50; i++) { // 5 seconds timeout
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			int avail = aio.available();
			if (avail < 0) {
				return io_status::Error;
			}
			else if (avail > 0) {
				auto chunk = aio.read(static_cast<std::size_t>(avail));
				responseRaw.append(chunk.get(), static_cast<std::size_t>(avail));
				if (this->hasTableEnding(responseRaw)) {
					break;
				}
			}
		}
		ssize_t end_res = aio.end();
		if (end_res != (ssize_t)strlen(request.get())) {
			return io_status::Error;
		}

		if (!this->hasTableEnding(responseRaw)) {
			return io_status::Error;
		}

		return mount_point::parse_table(responseRaw);
	}

	bool ntrip_client::hasTableEnding(std::string_view data)
	{
		const std::string tableEnding {"ENDSOURCETABLE\r\n"};
		if (data.length() >= tableEnding.length()) {
			return (data.compare(data.length() - tableEnding.length(),
				tableEnding.length(), tableEnding) == 0);
		}
		else {
			return false;
		}
	}

	[[nodiscard]] status ntrip_client::connect(const ntrip_login& nlogin)
	{
		if (m_tcp) {
			throw std::runtime_error("tcp connection already created");
		}
		m_tcp = std::make_unique<tcp_client>();
		auto con_res = m_tcp->connect(nlogin.address, nlogin.port);
		if (con_res != io_status::Success) {
			m_tcp.reset();
			m_status = status::error;
			return m_status;
		}
		m_aio = std::make_unique<async_io>(m_tcp->get_sockfd());
		std::unique_ptr<char[]> request = build_request(nlogin.mountpoint.data(),
								nlogin.username,
								nlogin.password);

		auto res = m_aio->write(request.get(), ::strlen(request.get()));
		if (res != io_status::Success) {
			m_status = status::error;
			return m_status;
		}

		// read authentication result
		std::string responseText{};
		for(int i = 1; i < 50; ++i) { // 5 second timeout
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			auto avail = m_aio->available();
			if (avail < 0) {
				m_status = status::error;
				return m_status;
			}
			else if (avail > 0) {
				auto chunk = m_aio->read((size_t)avail);
				responseText.append(chunk.get(), (size_t)avail);
				const std::string ending {"\r\n\r\n"};
				if (responseText.length() >= ending.length()) {
					if (responseText.compare(responseText.length() - ending.length(),
								ending.length(), ending) == 0) {
						break;
					}
				}
			}
		}
		if (m_aio->check() != io_status::Success) {
			m_status = status::error;
			return m_status;
		}
		ssize_t end_res = m_aio->end();
		if (end_res != (ssize_t)::strlen(request.get())) {
			m_status = status::error;
			return m_status;
		}

		auto startsWith = [text = &responseText](std::string_view start) -> bool
		{
			if (start.size() > text->size()) {
				return false;
			}
			return text->compare(0, start.size(), start) == 0;
		};

		if (startsWith("ICY 200 OK\r\n")) {
			m_status = status::ready;
		}
		else if (startsWith("HTTP/1.1 401 Unauthorized\r\n")) {
			m_status = status::authfailure;
		}
		else if (startsWith("HTTP/1.1 404 Not Found\r\n")) {
			m_status = status::nomount;
		}
		else {
			m_status = status::error;
		}
		return m_status;
	}

	int ntrip_client::available()
	{
		return m_aio->available();
	}

	std::unique_ptr<char[]> ntrip_client::receive(std::size_t size)
	{
		return m_aio->read(size);
	}

	[[nodiscard]] io_status ntrip_client::send_gga_begin(location location, std::chrono::system_clock::time_point time)
	{
		if (!m_aio) {
			throw std::runtime_error("no tcp connection");
		}
		std::string gga = std::get<std::string>(nmea::getGGA(location, time));
		return m_aio->write(gga.c_str(), gga.length());
	}

	[[nodiscard]] status ntrip_client::get_status()
	{
		if (m_status == status::ready) {
			io_status res = m_aio->check();
			switch (res)
			{
			case io_status::Success:
				m_status = status::ready;
				break;
			case io_status::InProgress:
				m_status = status::sending;
				break;
			
			default:
				m_status = status::error;
				break;
			}
		}
		return m_status;
	}

	void ntrip_client::disconnect()
	{
		constexpr int timeout = 50;
		int time = 0;
		while (m_aio->check() == io_status::InProgress)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			++time;
			if (time > timeout) {
				break;
			}
		}
		[[maybe_unused]] ssize_t res = m_aio->end();
		m_tcp->close();
		m_status = status::uninitialized;
	}

	[[nodiscard]] ssize_t ntrip_client::send_end()
	{
		return m_aio->end();
	}
}

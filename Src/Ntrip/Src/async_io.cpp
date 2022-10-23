#include <aio.h>
#include <errno.h>
#include <bits/c++config.h>
#include <stdexcept>

#include "async_io.hpp"


namespace VrsTunnel::Ntrip
{
	async_io::async_io(int sockfd) noexcept
	{
		::memset(&m_read_cb, 0, sizeof(m_read_cb));
		m_read_cb.aio_fildes = sockfd;
		m_read_cb.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
	}

	[[nodiscard]] io_status async_io::check() noexcept
	{
		int res = ::aio_error(&m_read_cb);
		if (res == EINPROGRESS) {
			return io_status::InProgress;
		}
		else if (res == 0) {
			return io_status::Success;
		}
		else {
			return io_status::Error;
		}
	}

	[[nodiscard]] io_status async_io::write(const char* data, std::size_t size)
	{
		m_data = std::make_unique<char[]>(size);
		::memcpy(m_data.get(), data, size);
		m_read_cb.aio_nbytes = size;
		m_read_cb.aio_offset = 0;
		m_read_cb.aio_buf = m_data.get();
		int res = ::aio_write(&m_read_cb);
		if (res == 0) {
			return io_status::Success;
		}
		else {
			return io_status::Error;
		}
	}

	int async_io::available() noexcept
	{
		int n_bytes_avail = 0;
		int res = ::ioctl(m_read_cb.aio_fildes, FIONREAD, &n_bytes_avail);
		if (res == 0) {
			return n_bytes_avail;
		}
		else {
			return (res > 0) ? (-res) : res;
		}
	}

	std::unique_ptr<char[]> async_io::read(std::size_t size)
	{
		auto data = std::make_unique<char[]>(size);
		ssize_t n_read = ::read(m_read_cb.aio_fildes, data.get(), size);
		if (n_read < 0) {
			throw std::runtime_error("read error");
		} else if (static_cast<std::size_t>(n_read) != size) {
			throw std::runtime_error("missing data");
		}
		return data;
	}

	[[nodiscard]] ssize_t async_io::end() noexcept
	{
		if (m_data) {
			m_data.reset();
		}
		return ::aio_return(&m_read_cb);
	}

	void async_io::close() noexcept
	{
		::close(m_read_cb.aio_fildes);
	}
}

#include "ntrip_server.hpp"
#include "login_encode.hpp"

namespace VrsTunnel::Ntrip
{
    [[nodiscard]] status ntrip_server::connect(ntrip_login& nlogin)
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
        std::unique_ptr<char[]> request = this->build_request(nlogin);
        auto res = m_aio->write(request.get(), strlen(request.get()));
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
                auto chunk = m_aio->read(avail);
                responseText.append(chunk.get(), avail);
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
        m_aio->end();

        auto startsWith = [text = &responseText](std::string_view start) -> bool
        {
            if (start.size() > text->size()) {
                return false;
            }
            return text->compare(0, start.size(), start) == 0;
        };

        if (startsWith("HTTP/1.1 200 OK\r\n")) {
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
    
    std::unique_ptr<char[]> ntrip_server::build_request(ntrip_login& nlogin)
    {
        // mount, port, auth, mount, lat, lon
        const char* requestFormat = "POST /%s HTTP/1.1\r\n"
            "Host: somehost:%d\r\n"
            "Ntrip-Version: Ntrip/2.0\r\n"
            "User-Agent: NTRIP PvvovanServer\r\n"
            "Authorization: Basic %s\r\n"
            "NTRIP-STR: %s;CMR-;12(1),12(1);2;GPS+GLONASS;23;ua;"
            "%9.6f;%9.6f;0;0;Trimble AgGPS_542;none;B;N;9600;none;"
            "\r\nTransfer-Encoding: chunked";

        std::unique_ptr<char[]> request;
        std::string auth{""};
        if (nlogin.username.size() > 0) {
            std::unique_ptr<login_encode> encoder = login_encode::make_instance();
            auth = (*encoder).get(nlogin.username, nlogin.password);
        }

        request = std::make_unique<char[]>(strlen(requestFormat) + nlogin.mountpoint.size()*2 + 5 + auth.length() + 33);
        sprintf(request.get(), requestFormat, nlogin.mountpoint.data(), nlogin.port, auth.c_str(),
            nlogin.mountpoint.data(), nlogin.position.Latitude, nlogin.position.Longitude);
        return request;
    }

    void ntrip_server::disconnect()
    {
        while (m_aio->check() == io_status::InProgress) { } // timeout missing?
        m_aio->end();
        m_tcp->close();
        m_status = status::uninitialized;
    }

    [[nodiscard]] ssize_t ntrip_server::send_end()
    {
        return m_aio->end();
    }

    [[nodiscard]] status ntrip_server::get_status()
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

    [[nodiscard]] status ntrip_server::send_begin(const char* data, int size)
    {
        if (!m_aio) {
            throw std::runtime_error("no tcp connection");
        }
        auto res = m_aio->write(data, size);
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
        return m_status;
    }
}
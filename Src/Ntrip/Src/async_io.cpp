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
    };

    [[nodiscard]] io_status async_io::write(const char* data, int size)
    {
        m_data = std::make_unique<char[]>(size);
        memcpy(m_data.get(), data, size);
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
        else if (res > 0) {
            return -res;
        }
        else {
            return res;
        }
    }

    std::unique_ptr<char[]> async_io::read(int size)
    {
        auto data = std::make_unique<char[]>(size);
        ssize_t n_read = static_cast<int>(::read(m_read_cb.aio_fildes, data.get(), size));
        if (n_read != size) {
            throw std::runtime_error("missing data");
        }
        return data;
    }

    ssize_t async_io::end() noexcept
    {
        return aio_return(&m_read_cb);
    }
}
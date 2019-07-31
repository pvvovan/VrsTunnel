#include "async_io.hpp"

namespace VrsTunnel::Ntrip
{
    async_io::async_io(int sockfd) noexcept
    {
        m_read_cb.aio_fildes = sockfd;
        m_read_cb.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
    }

    [[nodiscard]] io_status async_io::Check() noexcept
    {
        int res = aio_error(&m_read_cb);
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

    [[nodiscard]] io_status async_io::Write(const char* data, int size)
    {
        m_data = std::make_unique<char[]>(size);
        memcpy(m_data.get(), data, size);
        m_read_cb.aio_nbytes = size;
        m_read_cb.aio_offset = 0;
        m_read_cb.aio_buf = m_data.get();
        int res = aio_write(&m_read_cb);
        if (res == 0) {
            return io_status::Success;
        }
        else {
            return io_status::Error;
        }
    }

    int async_io::Available() noexcept
    {
        int n_bytes_avail = 0;
        int res = ioctl(m_read_cb.aio_fildes, FIONREAD, &n_bytes_avail);
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

    std::unique_ptr<char[]> async_io::Read(int size)
    {
        auto data = std::make_unique<char[]>(size);
        read(m_read_cb.aio_fildes, data.get(), size);
        return data;
    }

    ssize_t async_io::End() noexcept
    {
        return aio_return(&m_read_cb);
    }
}
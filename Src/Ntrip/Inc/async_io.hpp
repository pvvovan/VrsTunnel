#ifndef ASYNCHRONOUS_INPUT_OUTPUT_
#define ASYNCHRONOUS_INPUT_OUTPUT_

#include <aio.h>
#include <memory>
#include <cstring>
#include <sys/ioctl.h>
#include <unistd.h>


namespace VrsTunnel::Ntrip
{
    enum class io_status { InProgress, Error, Success };

    class async_io
    {
        async_io(const async_io&)               = delete;
        async_io& operator=(const async_io&)    = delete;
        async_io(async_io&&)                    = delete;
        async_io& operator=(async_io&&)         = delete;

        // control block to ckeck transmission status
        struct aiocb m_read_cb { 0 };
        std::unique_ptr<char[]> m_data;


        public:
        async_io(int sockfd)
        {
            m_read_cb.aio_fildes = sockfd;
            m_read_cb.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
        }

        [[nodiscard]] io_status Check()
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

        [[nodiscard]] io_status Write(const char* data, int size)
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

        int Available()
        {
            int n_bytes_avail = 0;
            ioctl(m_read_cb.aio_fildes, FIONREAD, &n_bytes_avail);
            return n_bytes_avail;
        }

        std::unique_ptr<char[]> Read(int size)
        {
            auto data = std::make_unique<char[]>(size);
            read(m_read_cb.aio_fildes, data.get(), size);
            return data;
        }

        ssize_t End()
        {
            return aio_return(&m_read_cb);
        }

    };

    
    
};

#endif /* ASYNCHRONOUS_INPUT_OUTPUT_ */
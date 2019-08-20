#ifndef VRS_TUNNEL_ACCEPT_LISTENER_
#define VRS_TUNNEL_ACCEPT_LISTENER_

#include <memory>
#include <map>
#include <mutex>
#include <list>

#include "tcp_client.hpp"

namespace VrsTunnel::Ntrip
{
    class accept_listener
    {
    public:
        struct element
        {
            std::shared_ptr<tcp_client> tcp_client{};
            std::shared_ptr<async_io> asy_io{};
        };
        void OnClientConnected(std::unique_ptr<tcp_client> client);
        std::list<std::weak_ptr<async_io>> get_asyncs();

    private:
    std::mutex the_mutex{};
    std::map<int, element> m_clients{};
    };
}

#endif /* VRS_TUNNEL_ACCEPT_LISTENER_ */
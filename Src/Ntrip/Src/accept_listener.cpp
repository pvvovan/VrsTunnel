
#include "accept_listener.hpp"

namespace VrsTunnel::Ntrip
{
    
    void accept_listener::OnClientConnected(std::unique_ptr<VrsTunnel::Ntrip::tcp_client> client)
    {
        std::scoped_lock sl(the_mutex);
        element elem{};
        elem.asy_io = std::make_unique<async_io>(client->get_sockfd());
        elem.tcp_client = std::move(client);
        m_clients.emplace_back(std::move(elem));
    }
    
    std::vector<std::weak_ptr<async_io>> accept_listener::get_asyncs()
    {
        std::vector<std::weak_ptr<async_io>> vec{};
        std::scoped_lock sl(the_mutex);
        for (const auto el : m_clients) {
            vec.emplace_back(std::weak_ptr(el.asy_io));
        }
        return vec;
    }
}

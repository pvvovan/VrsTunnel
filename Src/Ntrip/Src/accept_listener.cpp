
#include "accept_listener.hpp"

namespace VrsTunnel::Ntrip
{
    
    void accept_listener::OnClientConnected(std::unique_ptr<tcp_client> client)
    {
        std::scoped_lock sl(the_mutex);
        element elem{};
        elem.asy_io = std::make_unique<async_io>(client->get_sockfd());
        elem.sh_client = std::move(client);
        int id = elem.sh_client->get_sockfd();
        auto [el_iter, success] = m_clients.emplace(id, std::move(elem));
        if (!success) {
            throw std::runtime_error("TCP client exists already");
        }
    }
    
    std::list<std::weak_ptr<async_io>> accept_listener::get_asyncs() const
    {
        std::list<std::weak_ptr<async_io>> list{};
        std::scoped_lock sl(the_mutex);
        for (const auto el : m_clients) {
            list.emplace_back(std::weak_ptr(el.second.asy_io));
        }
        return list;
    }
}

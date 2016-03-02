
#include "ConnManager.hpp"

ConnManager::ConnManager()
{

}



ConnManager* ConnManager::get_instance()
{
    static ConnManager g_conn_manager;
    return &g_conn_manager;

}

void ConnManager::insert_conn(uint64_t id_, ip::tcp::socket& socket_)
{
    m_ConnMap.insert(make_pair(id_, Conn_t(socket_)));
    //m_conns.emplace_back(id_, socket_);
}

Conn_t* ConnManager::get_conn(uint64_t id_)
{
//    auto it = find_if (m_conns.begin(), m_conns.end(),
//                [&] (Conn_t& conn)
//                {
//                    return conn.m_nId == id_;
//                });
//
//    return it != m_conns.end() ? &(*it) : nullptr;
    return nullptr;
}

void ConnManager::stop_all()
{
//    for_each(m_conns.begin(), m_conns.end(),
//            [] (Conn_t& u)
//            {
//                u.m_socket.close();
//            });
}

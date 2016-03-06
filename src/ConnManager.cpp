
#include "ConnManager.hpp"

ConnManager::ConnManager()
{

}



ConnManager* ConnManager::get_instance()
{
    static ConnManager g_conn_manager;
    return &g_conn_manager;

}

void ConnManager::insert_conn(int64_t id_, ip::tcp::socket& socket_)
{
    m_ConnMap.insert(make_pair(id_, Conn_t(socket_)));
}


bool ConnManager::remove_conn(ip::tcp::socket& sock_)
{
//    auto it = find_if(m_ConnMap.begin(), m_ConnMap.end(),
//        [&] (Conn_t& conn_)
//        {
//           return conn_.is_open();
//        });
//
//    return it == m_ConnMap.end() ? true : false;
    return false;
}


Conn_t* ConnManager::get_conn(int64_t id_)
{
    auto it = m_ConnMap.find(id_);

    return it != m_ConnMap.end() ? &(it->second) : nullptr;
}

void ConnManager::stop_all()
{

}

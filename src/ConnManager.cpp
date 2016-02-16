
#include "ConnManager.hpp"

ConnManager::ConnManager()
{

}



ConnManager* ConnManager::get_instance()
{
    static ConnManager g_conn_manager;
    return &g_conn_manager;

}

void ConnManager::insert_conn(string& id_, ip::tcp::socket& socket_)
{
    //m_conn[id_] = socket_;
    m_conns.emplace_back(id_, socket_);
}


void ConnManager::stop_all()
{
    for_each(m_conns.begin(), m_conns.end(),
        [] (Conn_t& u)
        {
            u.m_socket.close();
        });
}


#include "ConnManager.hpp"

ConnManager::ConnManager()
{

}



ConnManager* ConnManager::get_instance()
{
    static ConnManager g_conn_manager;
    return &g_conn_manager;

}

void ConnManager::insert(connection_ptr conn_)
{
    m_Conns.insert(conn_);
}


void ConnManager::remove(connection_ptr conn_)
{
    m_Conns.erase(conn_);
}


void ConnManager::insert_conn_user(int conn_id_, int64_t user_id_)
{
    m_mapUserConn.insert(make_pair(conn_id_, user_id_));
}

int64_t ConnManager::get_user_id(int conn_id_)
{
    auto it = m_mapUserConn.find(conn_id_);
    return it == m_mapUserConn.end() ? 0 : it->second;
}

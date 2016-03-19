#ifndef CONNMANAGER_HPP_INCLUDED
#define CONNMANAGER_HPP_INCLUDED


#include <vector>
#include <map>
#include <set>
#include <string>
#include <boost/asio/ip/tcp.hpp>

#include "Connection.hpp"


using namespace std;
using namespace boost::asio;


class ConnManager
{
    ConnManager();

public:
    static ConnManager* get_instance();

public:
    void insert(connection_ptr);
    void remove(connection_ptr);

    // 插入connid-->userid
    void insert_conn_user(int, int64_t);
    int64_t get_user_id(int);


private:
    // map<连接标志, 连接>
    set<connection_ptr> m_Conns;

    // map<用户id, 连接标志>
    map<int, int64_t> m_mapUserConn;
};

#endif // CONNMANAGER_HPP_INCLUDED

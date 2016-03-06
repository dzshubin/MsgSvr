#ifndef CONNMANAGER_HPP_INCLUDED
#define CONNMANAGER_HPP_INCLUDED


#include <vector>
#include <map>
#include <set>
#include <string>
#include <boost/asio/ip/tcp.hpp>


using namespace std;
using namespace boost::asio;


struct Conn_t
{

    ip::tcp::socket& m_socket;


    Conn_t (ip::tcp::socket& socket_): m_socket(socket_)
    {
    }


    ip::tcp::socket& socket()
    {
        return m_socket;
    }

};



class ConnManager
{
private:
    ConnManager ();
public:
    static ConnManager* get_instance();

public:
    void insert_conn(int64_t, ip::tcp::socket& );
    bool remove_conn(ip::tcp::socket&);
    Conn_t* get_conn (int64_t);


    void stop_all();

private:
    // map<用户id, 连接信息>
    map<int64_t, Conn_t> m_ConnMap;
};

#endif // CONNMANAGER_HPP_INCLUDED

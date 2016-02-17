#ifndef CONNMANAGER_HPP_INCLUDED
#define CONNMANAGER_HPP_INCLUDED


#include <vector>
#include <set>
#include <string>
#include <boost/asio/ip/tcp.hpp>

#include "typeinfo.hpp"
using namespace std;
using namespace boost::asio;


struct Conn_t
{

    UL64 m_nId;
    ip::tcp::socket& m_socket;


    Conn_t (UL64 id_, ip::tcp::socket& socket_)
        :m_nId(id_), m_socket(socket_)
    {
    }

};



class ConnManager
{
private:
    ConnManager ();
public:
    static ConnManager* get_instance();

public:
    void insert_conn(UL64, ip::tcp::socket& );
    Conn_t* get_conn (UL64);


    void stop_all();

private:
    vector<Conn_t> m_conns;
};

#endif // CONNMANAGER_HPP_INCLUDED

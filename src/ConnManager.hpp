#ifndef CONNMANAGER_HPP_INCLUDED
#define CONNMANAGER_HPP_INCLUDED


#include <vector>
#include <set>
#include <string>
#include <boost/asio/ip/tcp.hpp>

using namespace std;
using namespace boost::asio;


struct Conn_t
{

    string m_strId;
    ip::tcp::socket& m_socket;


    Conn_t (string& id_, ip::tcp::socket& socket_)
        :m_strId(id_), m_socket(socket_)
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
    void insert_conn(string&, ip::tcp::socket& );

    void stop_all();

private:
    vector<Conn_t> m_conns;
};

#endif // CONNMANAGER_HPP_INCLUDED

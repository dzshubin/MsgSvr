#ifndef SERVER_H
#define SERVER_H


#include <boost/asio/io_service.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/noncopyable.hpp>

#include <signal.h>

#include "Connection.hpp"

using namespace std;
using namespace boost::asio;

class Server: private boost::noncopyable
{
public:
    typedef boost::system::error_code err_code;

public:
    Server();

    // 初始化
    bool initialization();
    // 运行
    void run();
    //
    bool connect_router_after();


public:
    int get_listen_port();
    void set_listen_port(int);
    int allocate_conn_id();


private:
    // 等待信号停止
    void await_stop();

    // 连接RouterSvr
    void connect_router();

    // 等待客户端连接
    void wait_accept_client();

    // 连接loginsvr
    void connect_login();

    // 绑定监听端口
    void bind_and_connect();

    // 链接DBsvr
    void connect_db();

private:
    io_service  m_io_service;

    // 新的连接
    connection_ptr m_client_conn;
    connection_ptr m_router_conn;
    connection_ptr m_login_conn;
    connection_ptr m_db_conn;


    ip::tcp::acceptor m_accClient;
    // 关闭程序的信息集
    signal_set m_signals;


private:
    int m_ClientListenPort;         // 监听用户连接端口
    static int g_count ;
};


extern Server *g;
#endif // SERVER_H

#ifndef SERVER_H
#define SERVER_H


#include <boost/asio/io_service.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <signal.h>

using namespace std;
using namespace boost::asio;


using err_code = boost::system::error_code;

class Server
{
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

    // 监听客户端
    ip::tcp::socket   m_sockClient;
    // 链接router
    ip::tcp::socket   m_sockRouter;

    ip::tcp::socket   m_sockLogin;

    ip::tcp::socket   m_sockDBSvr;

    ip::tcp::acceptor m_accClient;
    // 关闭程序的信息集
    signal_set m_signals;


private:
    int m_ClientListenPort;         // 监听用户连接端口
};


extern Server *g;

#endif // SERVER_H

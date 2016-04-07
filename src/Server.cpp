#include "Server.h"
#include "RouterConn.hpp"
#include "ClientConn.hpp"
#include "LoginConn.hpp"
#include "DbsvrConn.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/bind.hpp>
#include <boost/asio/connect.hpp>


#include <vector>
#include <thread>
#include <algorithm>
#include <memory>

Server* g = nullptr;
int Server::g_count = 1;

/**********************************************
 *
 *
 */

Server::Server()
  :m_io_service(),
   m_accClient(m_io_service),
   m_signals(m_io_service)
{
    //ctor
    await_stop();
}



bool Server::initialization()
{
    // 读取配置文件
    // 向routersvr请求监听端口
    connect_router();

    return true;
}


void Server::run ()
{
    // 线程池大小
    std::size_t threads_pool_size = 4;


    vector<shared_ptr<std::thread>> threads;
    for(int i = 0; i < threads_pool_size; i++)
    {
        shared_ptr<std::thread> thread(new std::thread(
            boost::bind(&io_service::run, &m_io_service)));
        threads.push_back(thread);
    }


    // 等待所有线程结束
    for_each(threads.begin(), threads.end(),
        [] (shared_ptr<std::thread> ptr)
        {
            ptr->join();
        });
}


bool Server::connect_router_after()
{
    bind_and_connect();
    connect_login();
    connect_db();
    return true;
}


/**********************************************
 *
 *
 */


void Server::await_stop()
{
    m_signals.async_wait([this](err_code /*ec*/, int /*signo*/)
    {
        // The server is stopped by cancelling all outstanding asynchronous
        // operations. Once all operations have finished the io_service::run()
        // call will exit.
        m_accClient.close();
    });
}

void Server::bind_and_connect()
{
    if (m_ClientListenPort <= 0)
    {
        cout << __FUNCTION__ << ": listenport < 0!" << endl;
        return;
    }

    // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
    ip::tcp::endpoint endpoint = ip::tcp::endpoint(ip::tcp::v4(), m_ClientListenPort);
    m_accClient.open(endpoint.protocol());
    m_accClient.set_option(ip::tcp::acceptor::reuse_address(true));
    m_accClient.bind(endpoint);
    m_accClient.listen();


    wait_accept_client();
}


void Server::connect_router()
{

    ip::tcp::resolver resolver(m_io_service);
    ip::tcp::resolver::iterator it = resolver.resolve( {"127.0.0.1", "10000"} );


    m_router_conn.reset(new RouterConn(m_io_service));
    async_connect(m_router_conn->socket(), it,
        [this] (const err_code& ec, ip::tcp::resolver::iterator it)
        {
            if (!ec)
            {
                cout << "connected router!" << endl;

                int id = allocate_conn_id();
                m_router_conn->connect(id);
            }
            else
            {
                std::cout << "error. try connect router..." << std::endl;
                boost::asio::deadline_timer t(m_io_service, boost::posix_time::seconds(10));
                t.wait();
                connect_router();
            }
        });
}


void Server::connect_db()
{
    ip::tcp::resolver resolver(m_io_service);
    ip::tcp::resolver::iterator it = resolver.resolve( {"127.0.0.1", "12000"} );

    m_db_conn.reset(new DBSvrConn(m_io_service));
    async_connect(m_db_conn->socket(), it,
        [this] (const err_code& ec, ip::tcp::resolver::iterator it_)
        {
            if (!ec)
            {
                cout << "connected DbSvr!" << endl;

                int id = allocate_conn_id();
                m_db_conn->connect(id);
            }

            else
            {
                cout << "error. try connect db..." << endl;
                boost::asio::deadline_timer t(m_io_service, boost::posix_time::seconds(10));
                t.wait();
                connect_db();

            }
        });
}


void Server::connect_login()
{
    ip::tcp::resolver resolver(m_io_service);
    ip::tcp::resolver::iterator it = resolver.resolve( {"127.0.0.1", "9800"} );


    m_login_conn.reset(new LoginConn(m_io_service));
    async_connect(m_login_conn->socket(), it,
        [this] (const err_code& ec, ip::tcp::resolver::iterator it_)
        {
            if (!ec)
            {
                cout << "connected login!" << endl;

                int id = allocate_conn_id();
                m_login_conn->connect(id);
            }
            else
            {
                std::cout << "error. try connect login..." << std::endl;
                boost::asio::deadline_timer t(m_io_service, boost::posix_time::seconds(10));
                t.wait();
                connect_login();
            }
        });
}


void Server::wait_accept_client()
{
    m_client_conn.reset(new ClientConn(m_io_service));

    m_accClient.async_accept(m_client_conn->socket(), [this] (const err_code& ec)
        {
            if (!ec)
            {
                cout << "client address: " << m_client_conn->socket().remote_endpoint().address().to_string()
                     << "client port: "    << m_client_conn->socket().remote_endpoint().port() << endl;


                int id = allocate_conn_id();
                m_client_conn->connect(id);
            }
            else
            {
                cout << "error" << endl;
            }
            wait_accept_client();
        });
}



/**********************************************
 *
 *
 */

 int Server::get_listen_port()
 {
    return m_ClientListenPort;
 }

 void Server::set_listen_port(int port_)
 {
    m_ClientListenPort = port_;
 }


int Server::allocate_conn_id()
{
    return (g_count++) % 65535;
}










#include "Server.h"
#include "RouterHandler.hpp"
#include "ClientHandler.hpp"
#include "LoginHandler.hpp"
#include "DBSvrHandler.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/asio/deadline_timer.hpp>
Server* g = nullptr;


/**********************************************
 *
 *
 */

Server::Server()
  :m_io_service(), m_sockClient(m_io_service),
   m_sockRouter(m_io_service), m_sockLogin(m_io_service),
   m_sockDBSvr(m_io_service), m_accClient(m_io_service),
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
    m_io_service.run();
    connect_router();
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
    m_signals.async_wait([this](boost::system::error_code /*ec*/, int /*signo*/)
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
    //
    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 10000);
    m_sockRouter.async_connect(ep, [this] (const err_code& ec)
        {
            if (!ec)
            {
                cout << "connected router!" << endl;
                make_shared<RouterHandler>(move(m_sockRouter))->start();
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
    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 12000);
    m_sockDBSvr.async_connect(ep, [this] (const err_code& ec)
        {
            if (!ec)
            {
                cout << "connected DbSvr!" << endl;
                make_shared<DBsvrHandler>(move(m_sockDBSvr))->start();
            }
            else
            {
                cout << "error. try connect router..." << endl;
                boost::asio::deadline_timer t(m_io_service, boost::posix_time::seconds(10));
                t.wait();
                connect_db();

            }
        });
}


void Server::connect_login()
{
    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 9800);
    m_sockLogin.async_connect(ep, [this] (const err_code& ec)
        {
            if (!ec)
            {
                cout << "connected login!" << endl;
                make_shared<LoginHandler>(move(m_sockLogin))->start();
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
    std::cout << "start accept!" << std::endl;
    m_accClient.async_accept(m_sockClient, [this] (const err_code& ec)
        {
            if (!ec)
            {
                cout << "client address: " << m_sockClient.remote_endpoint().address().to_string()
                     << "client port: "    << m_sockClient.remote_endpoint().port() << endl;

                make_shared<ClientHandler>(move(m_sockClient))->start();
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












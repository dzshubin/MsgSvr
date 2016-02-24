#include <iostream>
#include <string>

#include <array>
#include <vector>

#include <boost/asio/io_service.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/basic_socket_acceptor.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/basic_endpoint.hpp>
#include <boost/archive/text_iarchive.hpp>


#include <signal.h>
#include <arpa/inet.h>

#include "UserManager.hpp"
#include "ConnManager.hpp"
#include "RouterHandler.hpp"
#include "MsgStruct.hpp"
#include "LoginHandler.hpp"
#include "ClientMsgTypeDefine.hpp"
#include "ClientHandler.hpp"

using namespace std;
using namespace boost::asio;

class Server
{
public:
    using err_code = boost::system::error_code;

public:
    Server(io_service& io, unsigned short port)
        :m_UserSock(io),
         m_RouterSock(io),
         m_LoginSock(io),
         m_UserAcceptor(io),
         m_signals (io),
         listen_port(port)

    {
        m_signals.add(SIGINT);
        m_signals.add(SIGTERM);
#if defined(SIGQUIT)
        m_signals.add(SIGQUIT);
#endif // defined(SIGQUIT)

        await_stop();
        connect_router();
        //bind_and_connect();
    }

private:

    void allocate_port ()
    {

        // 向router注册,申请账号
        Msg_allocate_port msg_port;
        msg_port.m_port = 9600;


        CMsg msg;
        msg.set_msg_type((int)M2R::AllocatePort);
        msg.set_send_data(msg_port);


        string send_str;
        send_str.resize(sizeof(int32_t));


        int32_t type = msg.get_msg_type();
        int32_t be_type = ::htonl(type);
        cout << "size: " << send_str.size() << endl;

        send_str.append(reinterpret_cast<char*>(&be_type), sizeof(be_type));
        cout << "size: " << send_str.size() << endl;
        send_str.append(msg.get_send_data().c_str(), msg.send_data_len() + 1);
        cout << "size: " << send_str.size() << endl;


        int32_t len = msg.send_data_len() + 1 + sizeof(int32_t);
        int32_t be_len = ::htonl(len);

        std::copy(reinterpret_cast<char*>(&be_len),
                  reinterpret_cast<char*>(&be_len) + sizeof (int32_t),
                  send_str.begin());




        std::cout << "msg size: " << msg.send_data_len() + 1 << std::endl;
        std::cout << "str size: " << send_str.size() << std::endl;

        write(m_RouterSock, buffer(send_str));


        std::cout << "read port!" <<std::endl;
        char head_info[4];
        size_t st = read(m_RouterSock, buffer(head_info));


        int32_t pack_len;
        std::copy(head_info, head_info+sizeof(int32_t), reinterpret_cast<char*>(&pack_len));
        int32_t be_pack_len = ::ntohl(pack_len);

        cout << "data len： " << be_pack_len << endl;

        boost::asio::streambuf rf;
        read(m_RouterSock, rf, transfer_exactly(be_pack_len));


        // read port
        std::ostringstream os;
        os << &rf;

        std::string data(os.str());
        data = data.substr(sizeof(int32_t));

        std::istringstream is_stream(data);
        boost::archive::text_iarchive archive(is_stream);
        archive & msg_port;


        listen_port = msg_port.m_port;

        std::cout<< "allocate port: " << listen_port<< std::endl;
        bind_and_connect();

    }

    void bind_and_connect ()
    {
        // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
        ip::tcp::endpoint endpoint = ip::tcp::endpoint(ip::tcp::v4(), listen_port);
        m_UserAcceptor.open(endpoint.protocol());
        m_UserAcceptor.set_option(ip::tcp::acceptor::reuse_address(true));
        m_UserAcceptor.bind(endpoint);
        m_UserAcceptor.listen();

        connect_login();
        wait_accept();
    }

    void connect_login()
    {
        ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 9800);
        m_LoginSock.async_connect(ep, [this] (const err_code& ec)
            {
                if(!ec)
                {
                    std::make_shared<LoginHandler>(std::move(m_LoginSock),listen_port)->start();
                }
                else
                {
                    std::cout << "error. try connect login..." << std::endl;
                    boost::asio::deadline_timer t(m_LoginSock.get_io_service(), boost::posix_time::seconds(10));
                    t.wait();
                    connect_login();

                }
            });
    }

    void connect_router()
    {
        ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 10000);
        m_RouterSock.async_connect(ep, [this] (const err_code& ec)
            {
                if (!ec)
                {
                    allocate_port();

                    std::cout << "connected router!" << std::endl;
                    std::make_shared<RouterHandler>(std::move(m_RouterSock))->start();
                }
                else
                {
                    std::cout << "error. try connect router..." << std::endl;
                    boost::asio::deadline_timer t(m_RouterSock.get_io_service(), boost::posix_time::seconds(10));
                    t.wait();
                    connect_router();
                }

            });
    }

    /// Wait for a request to stop the server.
    void await_stop()
    {

        m_signals.async_wait([this](boost::system::error_code /*ec*/, int /*signo*/)
            {
            // The server is stopped by cancelling all outstanding asynchronous
            // operations. Once all operations have finished the io_service::run()
            // call will exit.
                    m_UserAcceptor.close();
            });
    }

    void wait_accept ()
    {
        std::cout << "start accept!" << std::endl;
        m_UserAcceptor.async_accept(m_UserSock, [this] (const err_code& ec)
            {
                if (!ec)
                {
                    cout << "client address: " <<m_UserSock.remote_endpoint().address().to_string()
                        <<"client port: " << m_UserSock.remote_endpoint().port() << endl;

                    std::make_shared<ClientHandler>(std::move(m_UserSock))->start();
                }
                else
                {
                    cout << "error" << endl;
                }
                wait_accept();
            });
    }




private:
    ip::tcp::socket   m_UserSock;
    ip::tcp::socket   m_RouterSock;
    ip::tcp::socket   m_LoginSock;
    ip::tcp::acceptor m_UserAcceptor;
    signal_set m_signals;

    int listen_port;
};


int main()
{
    io_service io;
    unsigned short port = 0;
    std::cin >> port;
    // 9600
    Server s(io, port);
    io.run();
}

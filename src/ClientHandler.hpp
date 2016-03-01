#ifndef CLIENTHANDLER_HPP_INCLUDED
#define CLIENTHANDLER_HPP_INCLUDED

#include "Handler.hpp"
#include "ConnManager.hpp"
#include "UserManager.hpp"

#include <boost/asio/ip/tcp.hpp>

using namespace boost::asio;
using namespace std;


class ClientHandler: public Handler
{
public:
    ClientHandler(ip::tcp::socket);

public:
    virtual void start();
    virtual void process_msg(int, string);

private:
    void handle_UserLogin(string);
    void handle_UserChat(string);
};

#endif // CLIENTHANDLER_HPP_INCLUDED

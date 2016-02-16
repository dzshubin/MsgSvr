#ifndef CLIENTHANDLER_HPP_INCLUDED
#define CLIENTHANDLER_HPP_INCLUDED

#include "Handler.hpp"
#include "ConnManager.hpp"
#include "UserManager.hpp"

#include <boost/asio/ip/tcp.hpp>

using namespace boost::asio;

class ClientHandler: public Handler
{
public:
    ClientHandler(ip::tcp::socket);

public:
    virtual void start();
    virtual void process_msg(int type_);

private:
    void handle_UserLogin();
    void handle_UserChat();
};

#endif // CLIENTHANDLER_HPP_INCLUDED

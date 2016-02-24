#ifndef ROUTERHANDLER_HPP_INCLUDED
#define ROUTERHANDLER_HPP_INCLUDED

#include <boost/asio/ip/tcp.hpp>

#include "Handler.hpp"
#include "ConnManager.hpp"
#include "UserManager.hpp"
#include "CMsg.h"


class RouterHandler:public Handler
{
public:
    RouterHandler (boost::asio::ip::tcp::socket);

public:
    virtual void start ();
    virtual void process_msg(int, string);

public:
    void handle_UserChat();
};


void send_to_router(CMsg&);

#endif // ROUTERHANDLER_HPP_INCLUDED

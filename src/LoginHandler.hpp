#ifndef LOGINHANDLER_HPP_INCLUDED
#define LOGINHANDLER_HPP_INCLUDED
#include <string>

#include <boost/asio/ip/tcp.hpp>

#include "Handler.hpp"
#include "CMsg.h"
#include "UserManager.hpp"

class LoginHandler : public Handler
{
public:
    LoginHandler (ip::tcp::socket,  int);

    virtual void start() override;
    virtual void process_msg(int) override;

private:
    int m_port;
};


void send_to_login(CMsg&);

#endif // LOGINHANDLER_HPP_INCLUDED


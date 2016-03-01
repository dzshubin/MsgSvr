#ifndef LOGINHANDLER_HPP_INCLUDED
#define LOGINHANDLER_HPP_INCLUDED
#include <string>

#include <boost/asio/ip/tcp.hpp>

#include "Handler.hpp"
#include "CMsg.hpp"

using namespace std;
using namespace boost::asio;

class LoginHandler: public Handler
{
public:
    LoginHandler (ip::tcp::socket);

    virtual void start() override;
    virtual void process_msg(int, string) override;


private:
    void register_msgsvr();

};


void send_to_login(CMsg&);

#endif // LOGINHANDLER_HPP_INCLUDED


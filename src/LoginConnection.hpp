#ifndef LOGINCONN_HPP_INCLUDED
#define LOGINCONN_HPP_INCLUDED
#include <string>

#include <boost/asio/ip/tcp.hpp>

#include "Connection.hpp"
#include "CMsg.hpp"

using namespace std;
using namespace boost::asio;

class LoginConn: public Connection
{
public:
    LoginConn (io_service&);

    virtual void on_connect() override;
    virtual void on_recv_msg(int, pb_message_ptr) override;
    virtual void on_disconnect() override;

private:
    void register_msgsvr();

};


void send_to_login(CMsg&);

#endif // LOGINCONN_HPP_INCLUDED


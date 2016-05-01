#ifndef LOGINCONN_HPP_INCLUDED
#define LOGINCONN_HPP_INCLUDED
#include <string>

#include <boost/asio/ip/tcp.hpp>

#include "Connection.hpp"
#include "CMsg.hpp"

#include "MessageDispatcher.h"

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
    void handle_lookup(pb_message_ptr);


private:
    void register_msgsvr();
private:
    MessageDispatcher m_dispatcher;

};


void send_to_login(CMsg&);

#endif // LOGINCONN_HPP_INCLUDED


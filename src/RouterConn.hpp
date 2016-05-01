#ifndef ROUTER_HPP_INCLUDED
#define ROUTER_HPP_INCLUDED

#include <boost/asio/ip/tcp.hpp>

#include "CMsg.hpp"
#include "Connection.hpp"
#include "MessageDispatcher.h"

using namespace std;
using namespace boost::asio;

class RouterConn: public Connection
{
public:
    RouterConn (io_service&);

public:
    virtual void on_connect () override;
    virtual void on_recv_msg(int, pb_message_ptr) override;
    virtual void on_disconnect() override;

private:
    void handle_user_chat(pb_message_ptr);
    /// 频道聊天转发
    void handle_channel_dispatch_chat(pb_message_ptr);
    void handle_allocate_port(pb_message_ptr);

private:
    // 申请分配端口
    void allocate_port();

private:
    MessageDispatcher m_dispatcher;

};


void send_to_router(CMsg&);

#endif // ROUTER_HPP_INCLUDED

#ifndef ROUTERHANDLER_HPP_INCLUDED
#define ROUTERHANDLER_HPP_INCLUDED

#include <boost/asio/ip/tcp.hpp>

#include "Handler.hpp"
#include "ConnManager.hpp"
#include "UserManager.hpp"
#include "CMsg.hpp"

using namespace std;
using namespace boost::asio;

class RouterHandler:public Handler
{
public:
    RouterHandler (ip::tcp::socket);

public:
    virtual void start () override;
    virtual void process_msg(int, string) override;

public:
    void handle_user_chat(string buf_);
    void handle_allocate_port(string buf_);

private:
    // 申请分配端口
    void allocate_port();

};


void send_to_router(CMsg&);

#endif // ROUTERHANDLER_HPP_INCLUDED

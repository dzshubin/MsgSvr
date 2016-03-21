#ifndef RouterConnection_HPP_INCLUDED
#define RouterConnection_HPP_INCLUDED

#include <boost/asio/ip/tcp.hpp>

#include "CMsg.hpp"
#include "Connection.hpp"

using namespace std;
using namespace boost::asio;

class RouterConnection: public Connection
{
public:
    RouterConnection (io_service&);

public:
    virtual void start () override;
    virtual void process_msg(int, string) override;

    virtual void stop_after() override;

public:
    void handle_user_chat(string buf_);
    void handle_allocate_port(string buf_);

private:
    // 申请分配端口
    void allocate_port();

};


void send_to_router(CMsg&);

#endif // RouterConnection_HPP_INCLUDED

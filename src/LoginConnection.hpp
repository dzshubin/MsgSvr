#ifndef LoginConnection_HPP_INCLUDED
#define LoginConnection_HPP_INCLUDED
#include <string>

#include <boost/asio/ip/tcp.hpp>

#include "Connection.hpp"
#include "CMsg.hpp"

using namespace std;
using namespace boost::asio;

class LoginConnection: public Connection
{
public:
    LoginConnection (io_service&);

    virtual void start() override;
    virtual void process_msg(int, string) override;

    void stop_after();

private:
    void register_msgsvr();

};


void send_to_login(CMsg&);

#endif // LoginConnection_HPP_INCLUDED


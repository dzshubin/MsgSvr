#ifndef ClientConnection_HPP_INCLUDED
#define ClientConnection_HPP_INCLUDED

#include "Connection.hpp"

#include <boost/asio/ip/tcp.hpp>

using namespace boost::asio;
using namespace std;


class ClientConnection: public Connection
{
public:
    ClientConnection(io_service&);

public:
    virtual void start() override;
    virtual void process_msg(int, string) override;

    virtual void stop_after() override;

private:
    void handle_client_login(string);
    void handle_chat(string);
    void handle_fetch_contacts(string);


};

#endif // ClientConnection_HPP_INCLUDED

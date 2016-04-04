#ifndef DBSVRHANDLER_HPP_INCLUDED
#define DBSVRHANDLER_HPP_INCLUDED


#include "Connection.hpp"
#include "MessageDispatcher.h"

class DBSvrConn: public Connection
{
public:
    DBSvrConn (io_service& io_);

    virtual void on_connect() override;
    virtual void on_recv_msg(int, pb_message_ptr) override;
    virtual void on_disconnect() override;

private:
    void handle_fetch_info(pb_message_ptr );
    void handle_fetch_contacts(pb_message_ptr);


private:
    MessageDispatcher m_dispatcher;
};


void send_to_db(CMsg&);


#endif // DBSVRHANDLER_HPP_INCLUDED

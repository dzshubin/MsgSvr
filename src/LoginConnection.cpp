#include "LoginConnection.hpp"
#include "ClientMsgTypeDefine.hpp"
#include "Server.h"
#include "MsgStruct.hpp"
#include  <boost/archive/binary_oarchive.hpp>

static LoginConnection* g_login_handler = nullptr;


/****************************************
 *
 *
 */

LoginConnection::LoginConnection(io_service& io_)
  :Connection(io_)
{

}


void LoginConnection::start()
{
    g_login_handler = this;

    register_msgsvr();
    read_head();
}



void LoginConnection::process_msg(int /**/, string /**/)
{

}



void send_to_login (CMsg& msg)
{
    if (g_login_handler)
        g_login_handler->send(msg);
}




/****************************************
 *
 *
 */

void LoginConnection::register_msgsvr()
{

    Msg_msgsvr_register msgsvr;
    msgsvr.m_port = g->get_listen_port();

    CMsg packet;
    packet.set_msg_type((int)M2L::REGISTER);
    packet.serialization_data_Asio(msgsvr);

    send(packet);
}

#include "LoginConn.hpp"
#include "ClientMsgTypeDefine.hpp"
#include "Server.h"
#include "MsgStruct.hpp"
#include  <boost/archive/binary_oarchive.hpp>


#include "register.pb.h"

static LoginConn* g_login_handler = nullptr;


/****************************************
 *
 *
 */

LoginConn::LoginConn(io_service& io_)
  :Connection(io_)
{

}


void LoginConn::on_connect()
{
    g_login_handler = this;

    register_msgsvr();
    read_head();
}


void LoginConn::on_disconnect()
{

}


void LoginConn::on_recv_msg(int /**/, pb_message_ptr /**/)
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

void LoginConn::register_msgsvr()
{

    IM::Register msgsvr;
    msgsvr.set_port(g->get_listen_port());


    CMsg packet;
    packet.encode((int)M2L::REGISTER, msgsvr);
    send(packet);
}

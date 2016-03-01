#include "LoginHandler.hpp"
#include "ClientMsgTypeDefine.hpp"
#include "Server.h"
#include "MsgStruct.hpp"
#include  <boost/archive/binary_oarchive.hpp>

static LoginHandler* g_login_handler = nullptr;


/****************************************
 *
 *
 */

LoginHandler::LoginHandler(ip::tcp::socket sock_)
    :Handler(std::move(sock_))
{

}


void LoginHandler::start()
{
    g_login_handler = this;

    register_msgsvr();
    read_head_from_socket();
}



void LoginHandler::process_msg(int /**/, string /**/)
{

}



void send_to_login (CMsg& msg)
{
    if (g_login_handler)
        g_login_handler->send_msg(msg);
}




/****************************************
 *
 *
 */

void LoginHandler::register_msgsvr()
{

    Msg_msgsvr_register msgsvr;
    msgsvr.m_port = g->get_listen_port();

    CMsg packet;
    packet.set_msg_type((int)M2L::Register);
    packet.serialization_data_Asio(msgsvr);

    send_msg(packet);
}

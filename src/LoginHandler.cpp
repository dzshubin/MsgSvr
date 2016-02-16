#include "LoginHandler.hpp"
#include "ClientMsgTypeDefine.hpp"
#include "CMsg.h"
#include "MsgStruct.hpp"
#include  <boost/archive/binary_oarchive.hpp>

static LoginHandler* g_login_handler = nullptr;

LoginHandler::LoginHandler(ip::tcp::socket sock_, int port_)
    : Handler(std::move(sock_)), m_port(port_)
{

}


void LoginHandler::start()
{
    g_login_handler = this;

    Msg_msgsvr_register msgsvr;
    msgsvr.m_port = m_port;

    CMsg msgsvr_info;
    msgsvr_info.set_msg_type((int)M2L::Register);
    msgsvr_info.set_send_data(msgsvr);

    send_msg(msgsvr_info);

    read_head_from_socket();
}

void LoginHandler::process_msg(int /**/)
{

}



void send_to_login (CMsg& msg)
{
    if (g_login_handler)
        g_login_handler->send_msg(msg);
}



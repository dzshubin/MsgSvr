#include "RouterHandler.hpp"
#include "MsgStruct.hpp"
#include "ClientMsgTypeDefine.hpp"
#include "Server.h"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>


static RouterHandler* g_router_handler = nullptr;

/**********************************************
 *
 * Constructor
 *
 */

RouterHandler::RouterHandler(ip::tcp::socket sock_)
  :Handler(std::move(sock_))
{

}


/**********************************************
 *
 *
 *
 */

void RouterHandler::start()
{
    g_router_handler = this;

    allocate_port();
    read_head();
}


void RouterHandler::allocate_port()
{
    // 向router注册,申请账号
    Msg_allocate_port msg_port;
    msg_port.m_port = 9600;


    CMsg packet;
    packet.set_msg_type((int)M2R::ALLOCATE_PORT);
    packet.serialization_data_Asio(msg_port);

    send(packet);
}

void RouterHandler::process_msg(int type_, string buf_)
{
    std::cout << "roiter process msg!" << std::endl;

    switch (type_)
    {
    case (int)M2R::DISPATCH_CHAT:
        std::cout << "router dispatch chat msg" << std::endl;
        handle_user_chat(buf_);

    case (int)M2R::ALLOCATE_PORT:
        handle_allocate_port(buf_);
        break;
    }
}



/**********************************************
 *
 *  msg process function
 *
 */

void RouterHandler::handle_allocate_port(string buf_)
{
    // 获得分配端口, 开始监听
    Msg_allocate_port port;
    deserialization(port, buf_);

    cout << "allocate port: " << port.m_port << endl;
    g->set_listen_port(port.m_port);
    g->connect_router_after();
}

void RouterHandler::handle_user_chat(string buf_)
{
    std::cout << "recv router msg chat!" << std::endl;

    Msg_chat recv_chat;
    deserialization(recv_chat, buf_);

    std::cout << "chat sendid: " << recv_chat.m_send_id
              << "chat recvid: " << recv_chat.m_recv_id
              << "content: "     << recv_chat.m_content << std::endl;

    // 玩家在这个服务器？
    bool result = UserManager::get_instance()->find_user(recv_chat.m_recv_id);
    if (result)
    {
        CMsg packet;
        packet.set_msg_type(1900);
        packet.serialization_data_Asio(recv_chat);

        // 找到链接
        Conn_t* pConn = ConnManager::get_instance()->get_conn(recv_chat.m_recv_id);

        if (pConn != nullptr)
        {
            send(packet, pConn->socket());
        }
        else
        {
            cout << "error: " << __FUNCTION__ <<  " ,玩家连接不存在！" << endl;
        }

    }
    else
    {
        cout << "error: " << __FUNCTION__ << " 玩家不存在" << endl;
    }

}


void send_to_router (CMsg& msg)
{
    if (g_router_handler)
        g_router_handler->send(msg);
}


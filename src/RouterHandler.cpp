#include "RouterHandler.hpp"
#include "MsgStruct.hpp"
#include "ClientMsgTypeDefine.hpp"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>


static RouterHandler* g_router_handler = nullptr;

/**********************************************
 *
 * Constructor
 *
 */

RouterHandler::RouterHandler(boost::asio::ip::tcp::socket sock_)
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
    read_head_from_socket();
}

void RouterHandler::process_msg(int type_, string /**/)
{
    std::cout << "roiter process msg!" << std::endl;

    switch (type_)
    {
    case (int)M2R::UserChat:
        std::cout << "router dispatch chat msg" << std::endl;
        handle_UserChat();
        break;
    }
}



/**********************************************
 *
 *  msg process function
 *
 */
void RouterHandler::handle_UserChat()
{
    std::cout << "router msg chat!" << std::endl;


    Msg_chat recv_chat;
    deserialization(recv_chat, m_rBuf);


    std::cout << "chat sendid: " << recv_chat.m_send_id
              << "chat recvid: " << recv_chat.m_recv_id
              << "content: "     << recv_chat.m_content << std::endl;


//    // 通过id得到context
//    ClientHandler* p_context = (ClientHandler*)UserManager::get_instance()->get_context_by_id(recv_chat.m_recv_id);
//    if (p_context == nullptr)
//    {
//        std::cout << "router:Not found p_context! send to router!" << std::endl;
//        read_head_from_socket();
//        return;
//    }
//
//
//    // 通过context得到socket
//    Conn_t* p_conn = ConnManager::get_instance()->get_conn_by_context(p_context);
//    if (p_conn == nullptr)
//    {
//        std::cout << "router: Not found p_conn!" << std::endl;
//        read_head_from_socket();
//        return;
//    }
//
//
//    CMsg msg;
//    msg.set_msg_type(1900);
//    msg.set_send_data(recv_chat);
//
//
//    send_msg(p_conn->m_socket, msg);
}


void send_to_router (CMsg& msg)
{
    if (g_router_handler)
        g_router_handler->send_msg(msg);
}


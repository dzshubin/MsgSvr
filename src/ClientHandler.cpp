
#include "ClientHandler.hpp"
#include "LoginHandler.hpp"
#include "MsgStruct.hpp"
#include "DBSvrHandler.hpp"
#include "ClientMsgTypeDefine.hpp"
#include "RouterHandler.hpp"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

ClientHandler::ClientHandler(ip::tcp::socket usersock_)
    :Handler(std::move(usersock_))
{


}


/**********************************************
 *
 *
 *
 */

void ClientHandler::start()
{
    // 插入客户端连接信息
//ConnManager::get_instance()->insert_conn(this, m_sock);
    //std::cout << "Insert Conn info!" << std::endl;

    // 开始从客户端读取消息
    read_head_from_socket();
}

void ClientHandler::process_msg(int type_)
{
    std::cout << "start process client msg!" << std::endl;
    std::cout << "msg type: " <<type_<< std::endl;

    switch (type_)
    {
    case (int)C2L::UserLogin:
        std::cout << "client login!" << std::endl;
        handle_UserLogin();
        break;

    case (int)C2L::UserChat:
        std::cout << "client chat!" << std::endl;
        handle_UserChat();
        break;
    }
}



/****************************************
 *
 *
 *
 */

void ClientHandler::handle_UserLogin()
{

    Msg_login ml;
    deserialization(ml, m_rBuf);


    std::cout << "login id: " << ml.m_strId << std::endl;


    // 储存用户链接信息
    ConnManager::get_instance()->insert_conn(ml.m_strId, m_sock);

    // 准备读取用户信息
    Msg_init_user init_user;
    init_user.m_strId = ml.m_strId;

    CMsg read_info;
    read_info.set_msg_type((int)M2DB::read_info_by_id);
    read_info.set_send_data(init_user);
    send_to_db(read_info);


}


void ClientHandler::handle_UserChat()
{

    cout << "Client msg chat!" << endl;
//    std::cout << "client msg chat!" << std::endl;
//
//
//    Msg_chat msg_chat;
//    deserialization(msg_chat, m_rBuf);
//
//    std::cout << "chat sendid: " << msg_chat.m_send_id
//              << "chat recvid: " << msg_chat.m_recv_id
//              << "content: "     << msg_chat.m_content << std::endl;
//
//    // 通过id得到context
//    ClientHandler* p_context = (ClientHandler*)UserManager::get_instance()->get_context_by_id(msg_chat.m_recv_id);
//    if (p_context == nullptr)
//    {
//        std::cout << "Not found p_context! send to router!" << std::endl;
//        read_head_from_socket();
//        return;
//    }
//
//
//    // 通过context得到socket
//    Conn_t* p_conn = ConnManager::get_instance()->get_conn_by_context(p_context);
//    if (p_conn == nullptr)
//    {
//        std::cout << "Not found p_conn!" << std::endl;
//        read_head_from_socket();
//        return;
//    }
//
//    Msg_chat send_to_other;
//    send_to_other.m_recv_id = msg_chat.m_recv_id;
//    send_to_other.m_send_id = msg_chat.m_send_id;
//    send_to_other.m_content = msg_chat.m_content;
//
//    CMsg msg;
//    msg.set_msg_type(1900);
//    msg.set_send_data(send_to_other);
//
//    send_msg(p_conn->m_socket, msg);

}


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

void ClientHandler::process_msg(int type_, string /**/)
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

    Msg_login login_info;
    deserialization(login_info, m_rBuf);


    std::cout << "login id: " << login_info.m_nId << std::endl;


    // 储存用户链接信息
    ConnManager::get_instance()->insert_conn(login_info.m_nId, m_sock);



    CMsg read_info;
    read_info.set_msg_type(static_cast<int>(M2D::read_info_by_id));
    read_info.set_send_data(login_info);
    send_to_db(read_info);


}


void ClientHandler::handle_UserChat()
{

    cout << "Client msg chat!" << endl;
    Msg_chat chat_info;
    deserialization(chat_info, m_rBuf);


    cout << "chat sendid: " << chat_info.m_send_id
         << "chat recvid: " << chat_info.m_recv_id
         << "content: "     << chat_info.m_content << endl;


    CMsg dispatch_chat;
    dispatch_chat.set_send_data(chat_info);

    // user in this server??
    bool bIsFind = UserManager::get_instance()->find_user(chat_info.m_recv_id);

    if (bIsFind)
    {
        // get conn
        auto conn = ConnManager::get_instance()->get_conn(chat_info.m_recv_id);

        dispatch_chat.set_msg_type(1900);
        send_msg(conn->m_socket, dispatch_chat);
    }
    else
    {
        // send to router
        dispatch_chat.set_msg_type((int)M2R::UserChat);
        send_to_router(dispatch_chat);

    }

}



#include "DBSvrHandler.hpp"
#include "RouterHandler.hpp"
#include "LoginHandler.hpp"
#include "ClientMsgTypeDefine.hpp"
#include "MsgStruct.hpp"
#include "UserManager.hpp"
#include "User.hpp"

static DBsvrHandler* g_dbsvr_handler = nullptr;

DBsvrHandler::DBsvrHandler(ip::tcp::socket sock_)
    :Handler(std::move(sock_))
{

}


void DBsvrHandler::start()
{
    g_dbsvr_handler = this;
    read_head();
}

void DBsvrHandler::process_msg(int type_, string buf_)
{
    switch (type_)
    {
    case (int)M2D::READ_INFO:
        handle_fetch_info(buf_);
        break;
    }
}


void DBsvrHandler::handle_fetch_info(string buf_)
{
    Msg_user_info user_info;
    deserialization(user_info, buf_);


    // 插入用户信息
    User user;
    user.set_id(user_info.m_nId);
    user.set_name(user_info.m_strName);
    user.set_nick_name(user_info.m_strNickName);

    UserManager::get_instance()->insert_user(user);


    // 向router发送用户上线的消息
    Msg_login user_login;
    user_login.m_nId = user_info.m_nId;

    CMsg packet;
    packet.set_msg_type((int)M2R::LOGIN);
    packet.serialization_data_Asio(user_login);

    send_to_router(packet);



    // 向loginsvr发送本msgsvr的总人数
    Msg_update_count counts;
    counts.m_user_count = UserManager::get_instance()->size();

    packet.clear();
    packet.set_msg_type((int)M2L::UPDATE);
    packet.serialization_data_Asio(counts);

    send_to_login(packet);
}





void send_to_db (CMsg& msg)
{
    if (g_dbsvr_handler)
        g_dbsvr_handler->send(msg);
}

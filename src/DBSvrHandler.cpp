

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
    read_head_from_socket();
}

void DBsvrHandler::process_msg(int type_)
{
    switch (type_)
    {
    case (int)M2DB::read_info_by_id:
        handle_ReadInfoById();
        break;
    }
}


void DBsvrHandler::handle_ReadInfoById()
{
    Msg_user_info user_info;
    deserialization(user_info, m_rBuf);


    // 插入用户信息
    User user;
    user.set_id(user_info.m_strId);
    user.set_name(user_info.m_strName);
    user.set_nick_name(user_info.m_strNickName);

    UserManager::get_instance()->insert_user(user);


    Msg_login send_info;
    send_info.m_strId = user_info.m_strId;

    CMsg user_login_msg;
    user_login_msg.set_msg_type((int)M2R::UserLogin);
    user_login_msg.set_send_data(send_info);
    // 向router发送用户上线的消息
    send_to_router(user_login_msg);



    Msg_update_count update_count;
    update_count.m_user_count = UserManager::get_instance()->get_user_size();

    CMsg update_count_msg;
    update_count_msg.set_msg_type((int)M2L::UpdateMsgSvr);
    update_count_msg.set_send_data(update_count);

    // 向loginsvr发送本msgsvr的总人数
    send_to_login(update_count_msg);
}





void send_to_db (CMsg& msg)
{
    if (g_dbsvr_handler)
        g_dbsvr_handler->send_msg(msg);
}
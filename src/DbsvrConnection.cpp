

#include "DbsvrConnection.hpp"
#include "RouterConnection.hpp"
#include "LoginConnection.hpp"
#include "ClientMsgTypeDefine.hpp"
#include "MsgStruct.hpp"
#include "UserManager.hpp"
#include "User.hpp"

static DBsvrConnection* g_dbsvr_handler = nullptr;

DBsvrConnection::DBsvrConnection(io_service& io_)
  :Connection(io_)
{

}


void DBsvrConnection::start()
{
    g_dbsvr_handler = this;
    read_head();
}


void DBsvrConnection::stop_after()
{


}

void DBsvrConnection::process_msg(int type_, string buf_)
{
    switch (type_)
    {
    case (int)M2D::READ_INFO:
        handle_fetch_info(buf_);
        break;
    }
}


void DBsvrConnection::handle_fetch_info(string buf_)
{
    Msg_user_info user_info;
    deserialization(user_info, buf_);


    cout << "name: " << user_info.m_strName << endl;
    cout << "nick name: " << user_info.m_strNickName << endl;


    ImUser* pImUser = UserManager::get_instance()->get_user(user_info.m_nId);
    if (pImUser == nullptr)
    {
        //error
        cout << "error! pImuser is null! user_id: "<< user_info.m_nId << endl;
        return;
    }

    pImUser->set_id(user_info.m_nId);
    pImUser->set_name(user_info.m_strName);
    pImUser->set_nick_name(user_info.m_strNickName);



    // 向router发送用户上线的消息
    Msg_login_id user_login;
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

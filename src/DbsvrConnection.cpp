

#include "DbsvrConnection.hpp"
#include "RouterConnection.hpp"
#include "LoginConnection.hpp"
#include "ClientMsgTypeDefine.hpp"
#include "MsgStruct.hpp"
#include "UserManager.hpp"
#include "User.hpp"


#include "contacts.pb.h"


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

    case (int)M2D::FETCH_CONTACTS:
        handle_fetch_contacts(buf_);
        break;
    }
}


/****************************************
 *  handle function
 *
 */




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



    // 向客户端推送消息
    IM::User user;
    user.set_id(user_info.m_nId);
    user.set_name(user_info.m_strName);
    user.set_nick_name(user_info.m_strNickName);

    CMsg packet;
    packet.set_msg_type((int)C2M::LOGIN);
    packet.serialization_data_protobuf(user);
    send(packet, pImUser->get_conn()->socket());



    // 向router发送用户上线的消息
    MSG_LOGIN_ID user_login;
    user_login.m_nId = user_info.m_nId;

    packet.clear();
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


void DBsvrConnection::handle_fetch_contacts(string buf_)
{
    MSG_CONTACTS db_contacts;
    deserialization(db_contacts, buf_);

    int64_t nUserId = db_contacts.m_req_id;


    IM::Contacts client_contacts;

    auto it = db_contacts.m_contacts.begin();
    for(; it != db_contacts.m_contacts.end(); ++it)
    {
        IM::User* pUser = client_contacts.add_contacts();

        pUser->set_id(it->m_user_id);
        pUser->set_name(it->m_name);
        pUser->set_nick_name(it->m_nick_name);
    }


    CMsg packet;
    packet.set_msg_type(static_cast<int>(C2M::FETCH_CONTACTS));
    packet.serialization_data_protobuf(client_contacts);


    ImUser* pImUser = UserManager::get_instance()->get_user(nUserId);
    if (pImUser == nullptr)
    {
        //error
        cout << "error! pImuser is null! user_id: "<< nUserId << endl;
    }
    else
    {
        send(packet, pImUser->get_conn()->socket());
    }
}





void send_to_db (CMsg& msg)
{
    if (g_dbsvr_handler)
        g_dbsvr_handler->send(msg);
}



#include "DbsvrConnection.hpp"
#include "RouterConnection.hpp"
#include "LoginConnection.hpp"
#include "ClientMsgTypeDefine.hpp"
#include "MsgStruct.hpp"
#include "UserManager.hpp"
#include "User.hpp"


#include "contacts.pb.h"
#include "login.pb.h"
#include "msg_update.pb.h"


#include <google/protobuf/message.h>
#include <google/protobuf/reflection.h>


static DBSvrConn* g_dbsvr_handler = nullptr;

DBSvrConn::DBSvrConn(io_service& io_)
  :Connection(io_)
{

}


void DBSvrConn::on_connect()
{

    m_dispatcher.register_message_callback((int)M2D::READ_INFO,
        bind(&DBSvrConn::handle_fetch_info, this, std::placeholders::_1));

    m_dispatcher.register_message_callback((int)M2D::FETCH_CONTACTS,
        bind(&DBSvrConn::handle_fetch_contacts, this, std::placeholders::_1));

    g_dbsvr_handler = this;
    read_head();
}


void DBSvrConn::on_disconnect()
{


}

void DBSvrConn::on_recv_msg(int type_, pb_message_ptr p_msg_)
{
    cout << "recv msg type: "  << type_ << endl;
    m_dispatcher.on_message(type_, p_msg_);
}







/****************************************
 *  handle function
 *
 */



void DBSvrConn::handle_fetch_info(pb_message_ptr p_msg_)
{

    try
    {

        GOOGLE_PROTOBUF_VERIFY_VERSION;
        using namespace google::protobuf;

        auto descriptor = p_msg_->GetDescriptor();
        const Reflection* rf = p_msg_->GetReflection();

        const FieldDescriptor* f_id         = descriptor->FindFieldByName("id");
        const FieldDescriptor* f_name       = descriptor->FindFieldByName("name");
        const FieldDescriptor* f_sex        = descriptor->FindFieldByName("sex");
        const FieldDescriptor* f_nick_name  = descriptor->FindFieldByName("nick_name");



        assert(f_id         && f_id->type()==FieldDescriptor::TYPE_INT64);
        assert(f_name       && f_name->type()==FieldDescriptor::TYPE_STRING);
        assert(f_sex        && f_sex->type()==FieldDescriptor::TYPE_STRING);
        assert(f_nick_name  && f_nick_name->type()==FieldDescriptor::TYPE_STRING);



        int64_t id          = rf->GetInt64(*p_msg_, f_id);
        string name         = rf->GetString(*p_msg_, f_name);
        string sex          = rf->GetString(*p_msg_, f_sex);
        string nick_name    = rf->GetString(*p_msg_, f_nick_name);


        ImUser* pImUser = UserManager::get_instance()->get_user(id);
        if (pImUser == nullptr)
        {
            //error
            cout << "error! pImuser is null! user_id: "<< id << endl;
            return;
        }

        pImUser->set_id(id);
        pImUser->set_name(name);
        pImUser->set_nick_name(nick_name);
        pImUser->set_sex(sex);

        // 向客户端推送消息
        IM::User user;
        user.set_id(id);
        user.set_name(name);
        user.set_nick_name(nick_name);
        user.set_sex(sex);

        CMsg packet;
        packet.encode((int)C2M::LOGIN, user);
        send(packet, pImUser->get_conn()->socket());



        // 向router发送用户上线的消息
        IM::Account user_login;
        user_login.set_id(id);
        user_login.set_passwd("");

        CMsg routePkt;
        routePkt.encode((int)M2R::LOGIN, user_login);
        send_to_router(routePkt);



        // 向loginsvr发送本msgsvr的总人数
        IM::Update update_info;
        update_info.set_count(UserManager::get_instance()->size());

        CMsg loginPkt;
        loginPkt.encode((int)M2L::UPDATE, update_info);
        send_to_login(loginPkt);


    }
    catch (exception& e)
    {
        cout << "# ERR: exception in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what() << endl;
    }
}


void DBSvrConn::handle_fetch_contacts(pb_message_ptr p_msg_)
{

    try
    {
        GOOGLE_PROTOBUF_VERIFY_VERSION;
        using namespace google::protobuf;

        auto descriptor = p_msg_->GetDescriptor();
        const Reflection* rf = p_msg_->GetReflection();
        const FieldDescriptor* f_req_id = descriptor->FindFieldByName("req_id");
        const FieldDescriptor* f_contacts = descriptor->FindFieldByName("contacts");


        assert(f_req_id   && f_req_id->type()==FieldDescriptor::TYPE_INT64);
        assert(f_contacts && f_contacts->is_repeated());


        int64_t id = rf->GetInt64(*p_msg_, f_req_id);


        CMsg packet;
        packet.encode((int)C2M::FETCH_CONTACTS, *p_msg_);


        ImUser* pImUser = UserManager::get_instance()->get_user(id);
        if (pImUser == nullptr)
        {
            cout << "error! pImuser is null! user_id: "<< id << endl;
        }
        else
        {
            send(packet, pImUser->get_conn()->socket());
        }
    }
    catch (exception& e)
    {
        cout << "# ERR: exception in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what() << endl;
    }

}



void send_to_db (CMsg& msg)
{
    if (g_dbsvr_handler)
        g_dbsvr_handler->send(msg);
}

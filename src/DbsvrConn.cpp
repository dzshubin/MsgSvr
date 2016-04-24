

#include "DbsvrConn.hpp"
#include "RouterConn.hpp"
#include "LoginConn.hpp"
#include "ClientMsgTypeDefine.hpp"
#include "MsgStruct.hpp"
#include "UserManager.hpp"
#include "User.hpp"
#include "ChannelManager.hpp"


#include "contacts.pb.h"
#include "login.pb.h"
#include "msg_update.pb.h"
#include "channel.pb.h"


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
        bind(&DBSvrConn::handle_fetch_info,             this, std::placeholders::_1));

    m_dispatcher.register_message_callback((int)M2D::FETCH_CONTACTS,
        bind(&DBSvrConn::handle_fetch_contacts,         this, std::placeholders::_1));

    m_dispatcher.register_message_callback((int)M2D::FETCH_OFFLINE_MESSAGE,
        bind(&DBSvrConn::handle_fetch_offline_message,  this, std::placeholders::_1));

    m_dispatcher.register_message_callback((int)M2D::FETCH_CHANNELS,
        bind(&DBSvrConn::handle_fetch_channels,         this, std::placeholders::_1));

    m_dispatcher.register_message_callback((int)M2D::JOIN_CHANNEL,
        bind(&DBSvrConn::handle_join_channel,           this, std::placeholders::_1));

    m_dispatcher.register_message_callback((int)M2D::EXIT_CHANNEL,
        bind(&DBSvrConn::handle_exit_channel,           this, std::placeholders::_1));

    m_dispatcher.register_message_callback((int)M2D::CHANNEL_USER_UPDATE,
        bind(&DBSvrConn::handle_channel_user_update,    this, std::placeholders::_1));




    g_dbsvr_handler = this;



    // 加载频道信息
    LoadChannel();

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



        // 推送频道信息
        IM::ChannelBaseInfo base_info;
        ChannelManager::get_instance()->LoadChannelBaseInfo(id, base_info);

        CMsg channel_pkt;
        channel_pkt.encode((int)C2M::SEND_CHANNELS, base_info);
        send(channel_pkt, pImUser->get_conn()->socket());



        IM::ChannelMembersInfo members;
        ChannelManager::get_instance()->LoadChannelMembers(members);

        CMsg member_pkt;
        member_pkt.encode((int)C2M::SEND_CHANNEL_MEMBERS, members);
        send(member_pkt, pImUser->get_conn()->socket());






        // 向router发送用户上线的消息
        IM::Account user_login;
        user_login.set_id(id);

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



void DBSvrConn::handle_fetch_offline_message(pb_message_ptr p_msg_)
{
    TRY
    auto descriptor = p_msg_->GetDescriptor();
    const Reflection* rf = p_msg_->GetReflection();
    const FieldDescriptor* f_req_id = descriptor->FindFieldByName("user_id");
    const FieldDescriptor* f_messages = descriptor->FindFieldByName("chat_message");


    assert(f_req_id   && f_req_id->type()==FieldDescriptor::TYPE_INT64);
    assert(f_messages && f_messages->is_repeated());

    int64_t req_id = rf->GetInt64(*p_msg_, f_req_id);


    ImUser* pImUser = UserManager::get_instance()->get_user(req_id);
    if (pImUser == nullptr)
    {
        cout << "error! pImuser is null! user_id: "<< req_id << endl;
    }
    else
    {
        CMsg packet;
        packet.encode((int)C2M::SEND_OFFLINE_MESSAGE, *p_msg_);
        send(packet, pImUser->get_conn()->socket());


        // 删除离线消息
        // 保存为历史消息
        CMsg history_req;
        history_req.encode((int)M2D::SAVE_TO_HISTORY, *p_msg_);
        send(history_req);
    }
    CATCH
}


void DBSvrConn::handle_fetch_channels(pb_message_ptr p_msg_)
{
    TRY


    auto descriptor = p_msg_->GetDescriptor();
    const Reflection* rf = p_msg_->GetReflection();
    const FieldDescriptor* f_channels = descriptor->FindFieldByName("channels");

    assert(f_channels && f_channels->is_repeated());


    RepeatedPtrField<IM::Channel> channels = rf->GetRepeatedPtrField<IM::Channel>(*p_msg_, f_channels);


    auto it = channels.begin();
    for(; it != channels.end(); ++it)
    {
        int64_t channel_id  = it->id();
        string channel_name = it->name();

        ChannelManager::get_instance()->CreateChannel(channel_id, move(channel_name));

        for(int i = 0; i < it->user_size(); i++)
        {
            IM::User user = it->user(i);
            ChannelManager::get_instance()->JoinChannel(channel_id, user);
        }
    }

    CATCH
}

void DBSvrConn::handle_exit_channel(pb_message_ptr p_msg_)
{

    TRY


    auto descriptor = p_msg_->GetDescriptor();
    const Reflection* rf = p_msg_->GetReflection();
    const FieldDescriptor* f_user_id    = descriptor->FindFieldByName("user_id");
    const FieldDescriptor* f_channel_id = descriptor->FindFieldByName("channel_id");
    const FieldDescriptor* f_result     = descriptor->FindFieldByName("result");




    assert(f_user_id    && f_user_id->type()    ==FieldDescriptor::TYPE_INT64);
    assert(f_result     && f_result->type()     ==FieldDescriptor::TYPE_INT32);
    assert(f_channel_id && f_channel_id->type() ==FieldDescriptor::TYPE_INT32);


    int64_t req_id      = rf->GetInt64(*p_msg_, f_user_id);
    int32_t channel_id  = rf->GetInt32(*p_msg_, f_channel_id);
    int32_t result      = rf->GetInt32(*p_msg_, f_result);


    ImUser* pImUser = UserManager::get_instance()->get_user(req_id);
    if (pImUser == nullptr)
    {
        cout << "error! pImuser is null! user_id: "<< req_id << endl;
    }
    else
    {
        if (result != 0)
        {
            // 修改内存
            ChannelManager::get_instance()->ExitChannel(channel_id, req_id);
        }
        else
        {
            // 数据库删除失败
        }


        CMsg packet;
        packet.encode((int)C2M::EXIT_CHANNEL, *p_msg_);
        send(packet, pImUser->get_conn()->socket());
    }


    CATCH

}


void DBSvrConn::handle_join_channel(pb_message_ptr p_msg_)
{
    TRY


    auto descriptor = p_msg_->GetDescriptor();
    const Reflection* rf = p_msg_->GetReflection();
    const FieldDescriptor* f_user_id    = descriptor->FindFieldByName("user_id");
    const FieldDescriptor* f_channel_id = descriptor->FindFieldByName("channel_id");
    const FieldDescriptor* f_result     = descriptor->FindFieldByName("result");




    assert(f_user_id    && f_user_id->type()    ==FieldDescriptor::TYPE_INT64);
    assert(f_result     && f_result->type()     ==FieldDescriptor::TYPE_INT32);
    assert(f_channel_id && f_channel_id->type() ==FieldDescriptor::TYPE_INT32);


    int64_t req_id      = rf->GetInt64(*p_msg_, f_user_id);
    int32_t channel_id  = rf->GetInt32(*p_msg_, f_channel_id);
    int32_t result      = rf->GetInt32(*p_msg_, f_result);


    ImUser* pImUser = UserManager::get_instance()->get_user(req_id);
    if (pImUser == nullptr)
    {
        cout << "error! pImuser is null! user_id: "<< req_id << endl;
    }
    else
    {
        //cout << "join channel result: " << result << endl;
        if (result != 0)
        {
            // 修改内存
            IM::User new_user;
            new_user.set_id(pImUser->get_id());
            new_user.set_sex(pImUser->get_sex());
            new_user.set_name(pImUser->get_name());
            new_user.set_nick_name(pImUser->get_nick_name());


            ChannelManager::get_instance()->JoinChannel(channel_id, new_user);
        }
        else
        {
            // 数据库插入失败
        }


        CMsg packet;
        packet.encode((int)C2M::JOIN_CHANNEL, *p_msg_);
        send(packet, pImUser->get_conn()->socket());
    }


    CATCH

}



void DBSvrConn::handle_channel_user_update(pb_message_ptr p_msg_)
{
    TRY

    auto descriptor = p_msg_->GetDescriptor();
    const Reflection* rf = p_msg_->GetReflection();
    const FieldDescriptor* f_user       = descriptor->FindFieldByName("user");
    const FieldDescriptor* f_channel_id = descriptor->FindFieldByName("channel_id");



    assert(f_user       && f_user->type()       ==FieldDescriptor::TYPE_MESSAGE);
    assert(f_channel_id && f_channel_id->type() ==FieldDescriptor::TYPE_INT32);



    google::protobuf::Message *pMessage = (rf->MutableMessage(&(*p_msg_), f_user));
    int32_t channel_id  = rf->GetInt32(*p_msg_,  f_channel_id);

    IM::User user;
    GetInstance(pMessage, user);


    // 更新内存
    ChannelManager::get_instance()->UpdateUser(channel_id, user);



    ImUser* pImUser = UserManager::get_instance()->get_user(user.id());
    if (pImUser == nullptr)
    {
        cout << "error! pImuser is null! user_id: "<< user.id() << endl;
    }
    {

        CMsg packet;
        packet.encode((int)C2M::CHANNEL_USER_UPDATE, *p_msg_);
        send(packet, pImUser->get_conn()->socket());
    }


    CATCH
}





/****************************************
 *  private function
 *
 */




void DBSvrConn::LoadChannel()
{
    IM::Account a;


    CMsg packet;
    packet.encode((int)M2D::FETCH_CHANNELS, a);


    send_to_db(packet);
}


void DBSvrConn::GetInstance(google::protobuf::Message* pMessage, IM::User& user)
{

    TRY

    auto descriptor = pMessage->GetDescriptor();
    const Reflection* rf = pMessage->GetReflection();
    const FieldDescriptor* f_id         = descriptor->FindFieldByName("id");
    const FieldDescriptor* f_name       = descriptor->FindFieldByName("name");
    const FieldDescriptor* f_nick_name  = descriptor->FindFieldByName("nick_name");
    const FieldDescriptor* f_sex        = descriptor->FindFieldByName("sex");



    assert(f_id         && f_id->type()         ==FieldDescriptor::TYPE_INT64);
    assert(f_name       && f_name->type()       ==FieldDescriptor::TYPE_STRING);
    assert(f_nick_name  && f_nick_name->type()  ==FieldDescriptor::TYPE_STRING);
    assert(f_sex        && f_sex->type()        ==FieldDescriptor::TYPE_STRING);


    int64_t user_id   = rf->GetInt64(*pMessage,  f_id);
    string name       = rf->GetString(*pMessage, f_name);
    string nick_name  = rf->GetString(*pMessage, f_nick_name);
    string sex        = rf->GetString(*pMessage, f_sex);



    user.set_id(user_id);
    user.set_name(name);
    user.set_sex(sex);
    user.set_nick_name(nick_name);

    CATCH

}





/****************************************
 *
 *
 */

void send_to_db (CMsg& msg)
{
    if (g_dbsvr_handler)
        g_dbsvr_handler->send(msg);
}

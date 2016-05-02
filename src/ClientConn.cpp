
#include "ClientConn.hpp"
#include "LoginConn.hpp"
#include "MsgStruct.hpp"
#include "DbsvrConn.hpp"
#include "ClientMsgTypeDefine.hpp"
#include "RouterConn.hpp"
#include "ChannelManager.hpp"
#include "User.hpp"
#include "UserManager.hpp"

#include "chat.pb.h"
#include "login.pb.h"
#include "msg_update.pb.h"
#include "message_cach.pb.h"

#include <boost/date_time/posix_time/posix_time.hpp>

#include <functional>


ClientConn::ClientConn(io_service& io_)
  :Connection(io_)
{


}


/**********************************************
 *
 *
 */

void ClientConn::on_connect()
{
    m_dispatcher.register_message_callback((int)C2M::LOGIN,
        bind(&ClientConn::handle_client_login,          this, std::placeholders::_1));

    m_dispatcher.register_message_callback((int)C2M::CHAT,
        bind(&ClientConn::handle_chat,                  this, std::placeholders::_1));

    m_dispatcher.register_message_callback((int)C2M::CHANNEL_CHAT,
        bind(&ClientConn::handle_channel_chat,          this, std::placeholders::_1));

    m_dispatcher.register_message_callback((int)C2M::FETCH_CONTACTS,
        bind(&ClientConn::handle_fetch_contacts,        this, std::placeholders::_1));

    m_dispatcher.register_message_callback((int)C2M::JOIN_CHANNEL,
        bind(&ClientConn::handle_join_channel,          this, std::placeholders::_1));

    m_dispatcher.register_message_callback((int)C2M::EXIT_CHANNEL,
        bind(&ClientConn::handle_exit_channel,          this, std::placeholders::_1));

    m_dispatcher.register_message_callback((int)C2M::CHANNEL_USER_UPDATE,
        bind(&ClientConn::handle_channel_user_update,   this, std::placeholders::_1));

    m_dispatcher.register_message_callback((int)C2M::FILE_TRANSLATION,
        bind(&ClientConn::handle_file_translation,      this, std::placeholders::_1));

    m_dispatcher.register_message_callback((int)C2M::FETCH_HISTORY,
        bind(&ClientConn::handle_fetch_history,         this, std::placeholders::_1));

    m_dispatcher.register_message_callback((int)C2M::FETCH_CHANNEL_HISTORY,
        bind(&ClientConn::handle_fetch_channel_history, this, std::placeholders::_1));

    // 开始从客户端读取消息
    read_head();
}



void ClientConn::on_disconnect()
{
    bool result = false;
    int64_t user_id = 0;
    tie(result, user_id) = UserManager::get_instance()->remove(get_conn_id());

    if (result)
    {
         // 通知Router
        IM::Account logout;
        logout.set_id(user_id);

        CMsg pkt;
        pkt.encode((int)M2R::LOGOUT, logout);

        send_to_router(pkt);


        // 通知Login
        IM::Update update;
        update.set_count(UserManager::get_instance()->size());

        CMsg loginPkt;
        loginPkt.encode((int)M2L::UPDATE, update);

        send_to_login(loginPkt);

    }
}


void ClientConn::on_recv_msg(int type_, pb_message_ptr p_msg_)
{
    std::cout << "Recv msg type: " <<type_<< std::endl;

    m_dispatcher.on_message(type_, p_msg_);
}



/****************************************
 *
 *
 *
 */

void ClientConn::handle_client_login(pb_message_ptr p_msg_)
{
    try
    {
        GOOGLE_PROTOBUF_VERIFY_VERSION;
        using namespace google::protobuf;

        auto descriptor = p_msg_->GetDescriptor();
        const Reflection* rf = p_msg_->GetReflection();
        const FieldDescriptor* f_id = descriptor->FindFieldByName("id");
        const FieldDescriptor* f_passwd = descriptor->FindFieldByName("passwd");


        assert(f_id && f_id->type()==FieldDescriptor::TYPE_INT64);
        assert(f_passwd && f_passwd->type()==FieldDescriptor::TYPE_STRING);


        int64_t id = rf->GetInt64(*p_msg_, f_id);
        cout << id  << endl;

        string passwd = rf->GetString(*p_msg_, f_passwd);
        cout << passwd << endl;

        ImUser *pImUser = UserManager::get_instance()->get_user(id);
        if (pImUser == nullptr)
        {
            pImUser = new ImUser;
            pImUser->set_id(id);
            pImUser->set_conn(shared_from_this());
            pImUser->set_conn_id(get_conn_id());


            UserManager::get_instance()->insert(pImUser);
        }


        // 读取玩家信息
        IM::Account id_req;
        id_req.set_id(id);

        CMsg fetch_info_pkt;
        fetch_info_pkt.encode((int)(M2D::READ_INFO), id_req);
        send_to_db(fetch_info_pkt);


        // 读取离线消息
        IM::Account req_id;
        req_id.set_id(id);

        CMsg fetch_message_pkt;
        fetch_message_pkt.encode((int)M2D::FETCH_OFFLINE_MESSAGE, req_id);
        send_to_db(fetch_message_pkt);


        // 读取频道离线消息
        IM::Account red_channel;
        red_channel.set_id(id);

        CMsg fetch_pkt;
        fetch_pkt.encode((int)M2D::FETCH_CHANNEL_OFFLINE_MESSAGE, red_channel);
        send_to_db(fetch_pkt);


    }
    catch (exception& e)
    {
        cout << "# ERR: exception in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what() << endl;
    }
}



void ClientConn::handle_channel_chat(pb_message_ptr p_msg_)
{
    TRY


        auto descriptor = p_msg_->GetDescriptor();
        const Reflection* rf = p_msg_->GetReflection();
        const FieldDescriptor* f_send_id    = descriptor->FindFieldByName("send_id");
        const FieldDescriptor* f_channel_id = descriptor->FindFieldByName("channel_id");
        const FieldDescriptor* f_content    = descriptor->FindFieldByName("content");
        const FieldDescriptor* f_recv_id    = descriptor->FindFieldByName("recv_id");
        const FieldDescriptor* f_send_tm    = descriptor->FindFieldByName("send_time");
        const FieldDescriptor* f_send_nm    = descriptor->FindFieldByName("send_name");


        assert(f_send_id && f_send_id->type()==FieldDescriptor::TYPE_INT64);
        assert(f_content && f_content->type()==FieldDescriptor::TYPE_BYTES);
        assert(f_send_nm && f_send_nm->type()==FieldDescriptor::TYPE_STRING);
        assert(f_channel_id && f_channel_id->type()==FieldDescriptor::TYPE_INT32);


        int64_t send_id     = rf->GetInt64(*p_msg_, f_send_id);
        int32_t channel_id  = rf->GetInt32(*p_msg_, f_channel_id);
        string  content     = rf->GetString(*p_msg_, f_content);


        string strCurrTime = get_cur_time();


        ImUser* pUser = UserManager::get_instance()->get_user(send_id);
        if (pUser == nullptr)
        {
            cout << "fatal error！" <<endl;
            return;
        }

        string send_name = pUser->get_nick_name();




        //发送给所有的群组成员
        vector<int64_t> vecIds;
        ChannelManager::get_instance()->GetUserIds(channel_id, vecIds);


        // 历史消息
        IM::ChannelMsgCach channel_pkts;
        // 转发消息
        IM::ChannelMsgCach dispatch_pkts;


        auto it = vecIds.cbegin();
        for (; it != vecIds.cend(); ++it)
        {
            int64_t recv_id = *it;
            if (recv_id == send_id)
            {
                continue;
            }


            rf->SetInt64(&(*p_msg_), f_recv_id, recv_id);
            rf->SetString(&(*p_msg_), f_send_tm, strCurrTime);
            rf->SetString(&(*p_msg_), f_send_nm, send_name);

            // 玩家在服务器里？
            ImUser* pImUser = UserManager::get_instance()->get_user(recv_id);
            if (pImUser)
            {
                // 转发
                connection_ptr conn = pImUser->get_conn();

                if (conn != nullptr)
                {
                    // 发送聊天消息
                    CMsg packet;
                    packet.encode((int)C2M::CHANNEL_CHAT, *p_msg_);
                    send(packet, conn->socket());

                    IM::ChannelChatPkt* pchat = channel_pkts.add_channel_messages();
                    pchat->set_recv_id(recv_id);
                    pchat->set_send_id(send_id);
                    pchat->set_channel_id(channel_id);
                    pchat->set_content(content);
                    pchat->set_send_time(strCurrTime);
                    pchat->set_send_name(send_name);
                }
                else
                {
                    //  玩家连接不存在！
                    cout << "error: " << __FUNCTION__ << ", user id: "
                         << recv_id << "conn isn't exists!" << endl;
                }
            }
            else
            {
                // 服务器不存在玩家
                // 转发routersvr
                IM::ChannelChatPkt* pChat = dispatch_pkts.add_channel_messages();
                pChat->set_recv_id(recv_id);
                pChat->set_send_id(send_id);
                pChat->set_channel_id(channel_id);
                pChat->set_content(content);
                pChat->set_send_time(strCurrTime);
                pChat->set_send_name(send_name);
            }

        }

        if (channel_pkts.channel_messages_size() != 0)
        {
            // 保存聊天记录
            CMsg historyPkt;
            historyPkt.encode((int)M2D::SAVE_CHANNEL_HISTORY, channel_pkts);
            send_to_db(historyPkt);
        }


        if (dispatch_pkts.channel_messages_size() != 0)
        {
            // 转发消息
            CMsg dispatchPkt;
            dispatchPkt.encode((int)M2R::DISPATCH_CHANNEL_CHAT, dispatch_pkts);
            send_to_router(dispatchPkt);
        }



    CATCH
}


void ClientConn::handle_chat(pb_message_ptr p_msg_)
{
    try
    {
        GOOGLE_PROTOBUF_VERIFY_VERSION;
        using namespace google::protobuf;

        auto descriptor = p_msg_->GetDescriptor();
        const Reflection* rf = p_msg_->GetReflection();
        const FieldDescriptor* f_send_id = descriptor->FindFieldByName("send_id");
        const FieldDescriptor* f_recv_id = descriptor->FindFieldByName("recv_id");
        const FieldDescriptor* f_content = descriptor->FindFieldByName("content");
        const FieldDescriptor* f_send_nm = descriptor->FindFieldByName("send_name");

        assert(f_send_id && f_send_id->type()==FieldDescriptor::TYPE_INT64);
        assert(f_recv_id && f_recv_id->type()==FieldDescriptor::TYPE_INT64);
        assert(f_send_nm && f_send_nm->type()==FieldDescriptor::TYPE_STRING);
        assert(f_content && f_content->type()==FieldDescriptor::TYPE_BYTES);



        int64_t send_id = rf->GetInt64(*p_msg_, f_send_id);
        int64_t recv_id = rf->GetInt64(*p_msg_, f_recv_id);
        string  content = rf->GetString(*p_msg_, f_content);
        string  send_nm = rf->GetString(*p_msg_, f_send_nm);


        CMsg packet;
        IM::ChatPkt chat;
        chat.set_send_id(send_id);
        chat.set_recv_id(recv_id);
        chat.set_content(content);
        // 设置发送时间
        chat.set_send_time(get_cur_time().c_str());
        chat.set_send_name(send_nm);


        // 玩家在服务器里？
        ImUser* pImUser = UserManager::get_instance()->get_user(recv_id);
        if (pImUser)
        {
            // 转发
            connection_ptr conn = pImUser->get_conn();

            if (conn != nullptr)
            {
                // 保存为历史消息
                IM::MessageCach msg_cach;
                msg_cach.set_user_id(recv_id);

                IM::ChatPkt* pChatPkt =  msg_cach.add_chat_message();
                pChatPkt->set_send_id(send_id);
                pChatPkt->set_recv_id(recv_id);
                pChatPkt->set_content(chat.content());
                pChatPkt->set_send_time(chat.send_time());
                pChatPkt->set_send_name(send_nm);

                // 发送聊天消息
                packet.encode((int)C2M::CHAT, chat);
                send(packet, conn->socket());


                CMsg history_pkt;
                history_pkt.encode((int)M2D::SAVE_TO_HISTORY, msg_cach);
                send_to_db(history_pkt);

            }
            else
            {
                //  玩家连接不存在！
                cout << "error: " << __FUNCTION__ << ", user id: %d." << "conn isn't exists!" << endl;
            }
        }
        else
        {
            // 发送给routersvr
            packet.encode((int)M2R::DISPATCH_CHAT, chat);
            send_to_router(packet);
        }

    }
    catch (exception& e)
    {
        cout << "# ERR: exception in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what() << endl;
    }



}




void ClientConn::handle_fetch_contacts(pb_message_ptr p_msg_)
{
    try
    {

        GOOGLE_PROTOBUF_VERIFY_VERSION;
        using namespace google::protobuf;


        const Reflection* rf = p_msg_->GetReflection();
        const FieldDescriptor* f_user_id = p_msg_->GetDescriptor()->FindFieldByName("id");


        assert(f_user_id && f_user_id->type()==FieldDescriptor::TYPE_INT64);


        int64_t id = rf->GetInt64(*p_msg_, f_user_id);

        IM::Account id_req;
        id_req.set_id(id);


        CMsg packet;
        packet.encode((int)M2D::FETCH_CONTACTS, id_req);
        send_to_db(packet);

    }
    catch (exception& e)
    {
        cout << "# ERR: exception in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what() << endl;
    }
}

void ClientConn::handle_join_channel(pb_message_ptr p_msg_)
{
    TRY
    const Reflection* rf = p_msg_->GetReflection();
    const FieldDescriptor* f_req_base = p_msg_->GetDescriptor()->FindFieldByName("req_base");
    assert(f_req_base && f_req_base->type() == FieldDescriptor::TYPE_MESSAGE);


    google::protobuf::Message *pMessage = (rf->MutableMessage(&(*p_msg_), f_req_base));
    const FieldDescriptor* f_user_id = pMessage->GetDescriptor()->FindFieldByName("user_id");
    const FieldDescriptor* f_channel_id = pMessage->GetDescriptor()->FindFieldByName("channel_id");

    assert(f_user_id        && f_user_id->type()        ==FieldDescriptor::TYPE_INT64);
    assert(f_channel_id     && f_channel_id->type()     ==FieldDescriptor::TYPE_INT32);


    CMsg packet;
    packet.encode((int)M2D::JOIN_CHANNEL, *p_msg_);
    send_to_db(packet);

    CATCH

}


void ClientConn::handle_exit_channel(pb_message_ptr p_msg_)
{
    TRY
    const Reflection* rf = p_msg_->GetReflection();
    const FieldDescriptor* f_req_base = p_msg_->GetDescriptor()->FindFieldByName("req_base");
    assert(f_req_base && f_req_base->type() == FieldDescriptor::TYPE_MESSAGE);


    google::protobuf::Message *pMessage = (rf->MutableMessage(&(*p_msg_), f_req_base));
    const FieldDescriptor* f_user_id = pMessage->GetDescriptor()->FindFieldByName("user_id");
    const FieldDescriptor* f_channel_id = pMessage->GetDescriptor()->FindFieldByName("channel_id");

    assert(f_user_id        && f_user_id->type()        ==FieldDescriptor::TYPE_INT64);
    assert(f_channel_id     && f_channel_id->type()     ==FieldDescriptor::TYPE_INT32);


    CMsg packet;
    packet.encode((int)M2D::EXIT_CHANNEL, *p_msg_);
    send_to_db(packet);

    CATCH

}


void ClientConn::handle_fetch_channel_history(pb_message_ptr p_msg_)
{

    TRY
    const Reflection* rf = p_msg_->GetReflection();

    const FieldDescriptor* f_user_id     = p_msg_->GetDescriptor()->FindFieldByName("user_id");
    const FieldDescriptor* f_channel_id  = p_msg_->GetDescriptor()->FindFieldByName("channel_id");


    assert(f_user_id         && f_user_id->type()         ==FieldDescriptor::TYPE_INT64);
    assert(f_channel_id      && f_channel_id->type()      ==FieldDescriptor::TYPE_INT32);


    CMsg packet;
    packet.encode((int)M2D::FETCH_CHANNEL_HISTORY, *p_msg_);
    send_to_db(packet);

    CATCH


}


void ClientConn::handle_fetch_history(pb_message_ptr p_msg_)
{

    TRY
    const Reflection* rf = p_msg_->GetReflection();

    const FieldDescriptor* f_req_id     = p_msg_->GetDescriptor()->FindFieldByName("req_id");
    const FieldDescriptor* f_target_id  = p_msg_->GetDescriptor()->FindFieldByName("target_id");


    assert(f_req_id         && f_req_id->type()         ==FieldDescriptor::TYPE_INT64);
    assert(f_target_id      && f_target_id->type()      ==FieldDescriptor::TYPE_INT64);


    CMsg packet;
    packet.encode((int)M2D::FETCH_HISTORY, *p_msg_);
    send_to_db(packet);

    CATCH


}



void ClientConn::handle_file_translation(pb_message_ptr p_msg_)
{

    TRY
    const Reflection* rf = p_msg_->GetReflection();

    const FieldDescriptor* f_req_id     = p_msg_->GetDescriptor()->FindFieldByName("req_id");
    const FieldDescriptor* f_target_id  = p_msg_->GetDescriptor()->FindFieldByName("target_id");
    const FieldDescriptor* f_name       = p_msg_->GetDescriptor()->FindFieldByName("name");
    const FieldDescriptor* f_data       = p_msg_->GetDescriptor()->FindFieldByName("data");


    assert(f_req_id         && f_req_id->type()         ==FieldDescriptor::TYPE_INT64);
    assert(f_target_id      && f_target_id->type()      ==FieldDescriptor::TYPE_INT64);
    assert(f_name           && f_name->type()           ==FieldDescriptor::TYPE_STRING);
    assert(f_data           && f_data->type()           ==FieldDescriptor::TYPE_BYTES);



    int64_t recv_id = rf->GetInt64(*p_msg_, f_target_id);


        // 玩家在服务器里？
    ImUser* pImUser = UserManager::get_instance()->get_user(recv_id);
    if (pImUser)
    {
        if (pImUser->get_conn() != nullptr)
        {
            CMsg packet;
            packet.encode((int)C2M::FILE_TRANSLATION, *p_msg_);

            send(packet, pImUser->get_conn()->socket());
        }
        else
        {
            cout << "error!" << endl;
        }
    }
    else
    {

    }






    CATCH


}






void ClientConn::handle_channel_user_update(pb_message_ptr p_msg_)
{
    TRY

    const Reflection* rf = p_msg_->GetReflection();

    const FieldDescriptor* f_req_id     = p_msg_->GetDescriptor()->FindFieldByName("req_id");
    const FieldDescriptor* f_target_id  = p_msg_->GetDescriptor()->FindFieldByName("target_id");
    const FieldDescriptor* f_channel_id = p_msg_->GetDescriptor()->FindFieldByName("channel_id");


    assert(f_req_id         && f_req_id->type()         ==FieldDescriptor::TYPE_INT64);
    assert(f_target_id      && f_target_id->type()      ==FieldDescriptor::TYPE_INT64);
    assert(f_channel_id     && f_channel_id->type()     ==FieldDescriptor::TYPE_INT32);


    CMsg packet;
    packet.encode((int)M2D::CHANNEL_USER_UPDATE, *p_msg_);
    send_to_db(packet);

    CATCH
}




/****************************************
 *
 *
 */


bool  ClientConn::SendChannelChatToUser(int64_t user_id_,const  google::protobuf::Message& msg_)
{



}

















#include "ClientConn.hpp"
#include "LoginConn.hpp"
#include "MsgStruct.hpp"
#include "DbsvrConn.hpp"
#include "ClientMsgTypeDefine.hpp"
#include "RouterConn.hpp"
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

    m_dispatcher.register_message_callback((int)C2M::FETCH_CONTACTS,
        bind(&ClientConn::handle_fetch_contacts,        this, std::placeholders::_1));

    m_dispatcher.register_message_callback((int)C2M::JOIN_CHANNEL,
        bind(&ClientConn::handle_join_channel,          this, std::placeholders::_1));




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


        // 读取离线消息
        IM::Account req_id;
        req_id.set_id(id);

        CMsg fetch_message_pkt;
        fetch_message_pkt.encode((int)M2D::FETCH_OFFLINE_MESSAGE, req_id);
        send_to_db(fetch_message_pkt);


        // 读取频道离线消息





        // 读取玩家信息
        IM::Account id_req;
        id_req.set_id(id);

        CMsg fetch_info_pkt;
        fetch_info_pkt.encode((int)(M2D::READ_INFO), id_req);
        send_to_db(fetch_info_pkt);




    }
    catch (exception& e)
    {
        cout << "# ERR: exception in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what() << endl;
    }
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


        assert(f_send_id && f_send_id->type()==FieldDescriptor::TYPE_INT64);
        assert(f_recv_id && f_recv_id->type()==FieldDescriptor::TYPE_INT64);
        assert(f_content && f_content->type()==FieldDescriptor::TYPE_STRING);



        int64_t send_id = rf->GetInt64(*p_msg_, f_send_id);
        int64_t recv_id = rf->GetInt64(*p_msg_, f_recv_id);
        string  content = rf->GetString(*p_msg_, f_content);


        cout << "chat sendid: " << send_id
             << "chat recvid: " << recv_id
             << "content: "     << content << endl;


        CMsg packet;
        IM::ChatPkt chat;
        chat.set_send_id(send_id);
        chat.set_recv_id(recv_id);
        chat.set_content(move(content));
        // 设置发送时间
        chat.set_send_time(get_cur_time().c_str());





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
                pChatPkt->set_content(chat.content());
                pChatPkt->set_send_time(chat.send_time());


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

    const FieldDescriptor* f_user_id    = p_msg_->GetDescriptor()->FindFieldByName("user_id");
    const FieldDescriptor* f_channel_id = p_msg_->GetDescriptor()->FindFieldByName("channel_id");


    assert(f_user_id    && f_user_id->type()    ==FieldDescriptor::TYPE_INT64);
    assert(f_channel_id && f_channel_id->type() ==FieldDescriptor::TYPE_INT32);

    CMsg packet;
    packet.encode((int)M2D::JOIN_CHANNEL, *p_msg_);
    send_to_db(packet);

    CATCH

}

















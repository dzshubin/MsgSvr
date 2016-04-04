
#include "ClientConnection.hpp"
#include "LoginConnection.hpp"
#include "MsgStruct.hpp"
#include "DbsvrConnection.hpp"
#include "ClientMsgTypeDefine.hpp"
#include "RouterConnection.hpp"
#include "User.hpp"
#include "UserManager.hpp"

#include "chat.pb.h"
#include "login.pb.h"

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
        bind(&ClientConn::handle_client_login, this, std::placeholders::_1));


    m_dispatcher.register_message_callback((int)C2M::CHAT,
        bind(&ClientConn::handle_chat, this, std::placeholders::_1));

    m_dispatcher.register_message_callback((int)C2M::FETCH_CONTACTS,
        bind(&ClientConn::handle_fetch_contacts, this, std::placeholders::_1));



    // 开始从客户端读取消息
    read_head();
}



void ClientConn::on_disconnect()
{
    bool result = UserManager::get_instance()->remove(get_conn_id());
}


void ClientConn::on_recv_msg(int type_, pb_message_ptr p_msg_)
{
    std::cout << "msg type: " <<type_<< std::endl;

    m_dispatcher.on_message(type_, p_msg_);
}



/****************************************
 *
 *
 *
 */

void ClientConn::handle_client_login(pb_message_ptr p_msg_)
{


    GOOGLE_PROTOBUF_VERIFY_VERSION;
    using namespace google::protobuf;

    auto descriptor = p_msg_->GetDescriptor();
    const Reflection* rf = p_msg_->GetReflection();
    const FieldDescriptor* f_id = descriptor->FindFieldByName("id");
    const FieldDescriptor* f_passwd = descriptor->FindFieldByName("passwd");



    try
    {
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

        IM::LoginAccount id_req;
        id_req.set_id(id);
        id_req.set_passwd("");


        CMsg packet;
        packet.encode((int)(M2D::READ_INFO), id_req);
        send_to_db(packet);

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

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    using namespace google::protobuf;


    auto descriptor = p_msg_->GetDescriptor();
    const Reflection* rf = p_msg_->GetReflection();
    const FieldDescriptor* f_send_id = descriptor->FindFieldByName("send_id");
    const FieldDescriptor* f_recv_id = descriptor->FindFieldByName("recv_id");
    const FieldDescriptor* f_content = descriptor->FindFieldByName("content");


    int64_t send_id = 0, recv_id = 0;
    string content;

    if(f_send_id)
    {
        send_id = rf->GetInt64(*p_msg_, f_send_id);
    }

    if (f_recv_id)
    {
        recv_id = rf->GetInt64(*p_msg_, f_recv_id);
    }

    if (f_content)
    {
        content = rf->GetString(*p_msg_, f_content);
    }

    cout << "chat sendid: " << send_id
         << "chat recvid: " << recv_id
         << "content: "     << content << endl;


    IM::ChatPkt chatPkt;
    chatPkt.set_send_id(send_id);
    chatPkt.set_recv_id(recv_id);
    chatPkt.set_content(content);

    CMsg packet;

    // 玩家在服务器里？
    ImUser* pImUser = UserManager::get_instance()->get_user(recv_id);
    if (pImUser)
    {
        // 转发
        connection_ptr conn = pImUser->get_conn();

        if (conn != nullptr)
        {
            packet.encode((int)C2M::CHAT, chatPkt);
            send(packet, conn->socket());
        }
        else
        {
            //  玩家连接不存在！
            cout << "error: " << __FUNCTION__ << ", 玩家连接不存在！" << endl;
        }
    }
    else
    {
        // 发送给routersvr
        packet.encode((int)M2R::DISPATCH_CHAT, chatPkt);
        send_to_router(packet);
    }

}




void ClientConn::handle_fetch_contacts(pb_message_ptr p_msg_)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    using namespace google::protobuf;


    const Reflection* rf = p_msg_->GetReflection();
    const FieldDescriptor* f_user_id = p_msg_->GetDescriptor()->FindFieldByName("id");


    int64_t id = 0;

    if (f_user_id != nullptr)
    {
        id = rf->GetInt64(*p_msg_, f_user_id);
    }
    else
    {
        cout << "f_user_id is null!" << endl;
        return;
    }


    IM::LoginAccount id_req;
    id_req.set_id(id);
    id_req.set_passwd("");


    CMsg packet;
    packet.encode((int)M2D::FETCH_CONTACTS, id_req);
    send_to_db(packet);
}























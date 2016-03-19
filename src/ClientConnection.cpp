
#include "ClientConnection.hpp"
#include "LoginConnection.hpp"
#include "MsgStruct.hpp"
#include "DbsvrConnection.hpp"
#include "ClientMsgTypeDefine.hpp"
#include "RouterConnection.hpp"
#include "User.hpp"
#include "UserManager.hpp"

#include "chat.pb.h"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>



ClientConnection::ClientConnection(io_service& io_)
  :Connection(io_)
{


}


/**********************************************
 *
 *
 */

void ClientConnection::start()
{

    // 开始从客户端读取消息
    read_head();
}



void ClientConnection::process_msg(int type_, string buf_)
{
    std::cout << "start process client msg!" << std::endl;
    std::cout << "msg type: " <<type_<< std::endl;

    switch (type_)
    {
    case (int)C2M::LOGIN:
        std::cout << "client login!" << std::endl;
        handle_client_login(buf_);
        break;

    case (int)C2M::CHAT:
        std::cout << "client chat!" << std::endl;
        handle_chat(buf_);
        break;
    }
}



/****************************************
 *
 *
 *
 */

void ClientConnection::handle_client_login(string buf_)
{


    GOOGLE_PROTOBUF_VERIFY_VERSION;
    using namespace google::protobuf;

    cout << "buf: " << buf_.c_str() << "size：" << buf_.size()<<endl;

    // namelen
    int32_t name_len = AsInt32(buf_.c_str());
    cout << "name_len: " << name_len << endl;

    // type name
    const char* chr_name = buf_.c_str() + sizeof(int32_t);
    string type_name = string(chr_name, name_len);
    cout << "type_name: " << type_name << endl;

    shared_ptr<google::protobuf::Message> p_ms = CreateMessage(type_name);

    if (p_ms == nullptr)
    {
        cout << "fail!" << endl;
        return;
    }

    // 反序列化
    int size = buf_.size();
    p_ms->ParseFromArray(buf_.c_str() + sizeof(int32_t) + name_len,
                       size - sizeof(int32_t)-name_len);


    const Reflection* rf = p_ms->GetReflection();
    const FieldDescriptor* f_id = p_ms->GetDescriptor()->FindFieldByName("id");
    const FieldDescriptor* f_passwd = p_ms->GetDescriptor()->FindFieldByName("passwd");


    int64_t id;
    string passwd;

    try
    {
        id = rf->GetInt64(*p_ms, f_id);
        cout << id  << endl;

        passwd = rf->GetString(*p_ms, f_passwd);
        cout << passwd << endl;
    }
    catch (exception& e)
    {
        cout << "# ERR: exception in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what() << endl;
    }


    ImUser *pImUser = UserManager::get_instance()->get_user(id);
    if (pImUser == nullptr)
    {
        pImUser = new ImUser;
        pImUser->set_id(id);
        pImUser->set_conn(shared_from_this());

        UserManager::get_instance()->insert(pImUser);
    }



    Msg_login_id login_id;
    login_id.m_nId = id;

    CMsg packet;
    packet.set_msg_type(static_cast<int>(M2D::READ_INFO));
    packet.serialization_data_Asio(login_id);
    send_to_db(packet);

}


void ClientConnection::handle_chat(string buf_)
{

    cout << "Client msg chat!" << endl;


    GOOGLE_PROTOBUF_VERIFY_VERSION;
    using namespace google::protobuf;

    cout << "buf: " << buf_.c_str() << "size：" << buf_.size()<<endl;

    // namelen
    int32_t name_len = AsInt32(buf_.c_str());
    cout << "chat name_len: " << name_len << endl;

    // type name
    const char* chr_name = buf_.c_str() + sizeof(int32_t);
    string type_name = string(chr_name, name_len);
    cout << "type_name: " << type_name << endl;

    shared_ptr<google::protobuf::Message> p_ms = CreateMessage(type_name);

    if (p_ms == nullptr)
    {
        cout << "fail!" << endl;
        return;
    }

    // 反序列化
    int size = buf_.size();
    p_ms->ParseFromArray(buf_.c_str() + sizeof(int32_t) + name_len, size - sizeof(int32_t)-name_len);


    const Reflection* rf = p_ms->GetReflection();
    const FieldDescriptor* f_send_id = p_ms->GetDescriptor()->FindFieldByName("send_id");
    const FieldDescriptor* f_recv_id = p_ms->GetDescriptor()->FindFieldByName("recv_id");
    const FieldDescriptor* f_content = p_ms->GetDescriptor()->FindFieldByName("content");


    int64_t send_id = 0, recv_id = 0;
    string content;

    if(f_send_id)
    {
        send_id = rf->GetInt64(*p_ms, f_send_id);
    }

    if (f_recv_id)
    {
        recv_id = rf->GetInt64(*p_ms, f_recv_id);
    }

    if (f_content)
    {
        content = rf->GetString(*p_ms, f_content);
    }

    cout << "chat sendid: " << send_id
         << "chat recvid: " << recv_id
         << "content: "     << content << endl;


    CMsg packet;

    // 玩家在服务器里？
    ImUser* pImUser = UserManager::get_instance()->get_user(recv_id);
    if (pImUser)
    {
        // 转发
        connection_ptr conn = pImUser->get_conn();

        if (conn != nullptr)
        {
            IM::ChatPkt chatPkt;
            chatPkt.set_send_id(send_id);
            chatPkt.set_recv_id(recv_id);
            chatPkt.set_content(content);


            packet.set_msg_type(static_cast<int>(C2M::CHAT));
            packet.serialization_data_protobuf(chatPkt);
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

        Msg_chat chat_info;
        chat_info.m_recv_id = recv_id;
        chat_info.m_send_id = send_id;
        chat_info.m_content = content;

        packet.set_msg_type(static_cast<int>(M2R::DISPATCH_CHAT));
        packet.serialization_data_Asio(chat_info);
        // 发送给routersvr
        send_to_router(packet);
    }

}

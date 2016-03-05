
#include "ClientHandler.hpp"
#include "LoginHandler.hpp"
#include "MsgStruct.hpp"
#include "DBSvrHandler.hpp"
#include "ClientMsgTypeDefine.hpp"
#include "RouterHandler.hpp"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

ClientHandler::ClientHandler(ip::tcp::socket usersock_)
    :Handler(std::move(usersock_))
{


}


/**********************************************
 *
 *
 *
 */

void ClientHandler::start()
{
    // 插入客户端连接信息
//ConnManager::get_instance()->insert_conn(this, m_sock);
    //std::cout << "Insert Conn info!" << std::endl;

    // 开始从客户端读取消息
    read_head();
}

void ClientHandler::process_msg(int type_, string buf_)
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

void ClientHandler::handle_client_login(string buf_)
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

    ConnManager::get_instance()->insert_conn(id, m_sock);

    Msg_login_id login_id;
    login_id.m_nId = id;

    CMsg packet;
    packet.set_msg_type(static_cast<int>(M2D::READ_INFO));
    packet.serialization_data_Asio(Msg_login_id);
    send_to_db(packet);


    google::protobuf::ShutdownProtobufLibrary();

}


void ClientHandler::handle_chat(string buf_)
{

    cout << "Client msg chat!" << endl;

    Msg_chat chat_info;
    deserialization(chat_info, buf_);

    cout << "chat sendid: " << chat_info.m_send_id
         << "chat recvid: " << chat_info.m_recv_id
         << "content: "     << chat_info.m_content << endl;


    CMsg packet;
    packet.serialization_data_Asio(chat_info);


    // 玩家在服务器里？
    bool result = UserManager::get_instance()->find_user(chat_info.m_recv_id);
    if (result)
    {
        // 转发
        Conn_t* pConn = ConnManager::get_instance()->get_conn(chat_info.m_recv_id);

        if (pConn != nullptr)
        {
            packet.set_msg_type(1900);
            send(packet, pConn->socket());
        }
        else
        {
            //  玩家连接不存在！
            cout << "error: " << __FUNCTION__ << ", 玩家连接不存在！" << endl;
        }
    }
    else
    {
        packet.set_msg_type(static_cast<int>(M2R::DISPATCH_CHAT));
        // 发送给routersvr
        send_to_router(packet);
    }

}

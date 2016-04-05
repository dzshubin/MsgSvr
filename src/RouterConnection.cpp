#include "RouterConnection.hpp"
#include "MsgStruct.hpp"
#include "ClientMsgTypeDefine.hpp"
#include "Server.h"
#include "UserManager.hpp"
#include "User.hpp"


#include "allocate.pb.h"


static RouterConn* g_router_handler = nullptr;

/**********************************************
 *
 * Constructor
 *
 */

RouterConn::RouterConn(io_service& io_)
  :Connection(io_)
{

}


/**********************************************
 *
 *
 *
 */

void RouterConn::on_connect()
{


    m_dispatcher.register_message_callback((int)M2R::DISPATCH_CHAT,
        bind(&RouterConn::handle_user_chat, this, std::placeholders::_1));

    m_dispatcher.register_message_callback((int)M2R::ALLOCATE_PORT,
        bind(&RouterConn::handle_allocate_port, this, std::placeholders::_1));



    g_router_handler = this;
    allocate_port();
    read_head();
}


void RouterConn::on_disconnect()
{

}

void RouterConn::allocate_port()
{
    // 向router注册,申请账号
    IM::Allocate msg_port;
    msg_port.set_port(9500);


    CMsg packet;
    packet.encode((int)M2R::ALLOCATE_PORT, msg_port);
    send(packet);
}

void RouterConn::on_recv_msg(int type_, pb_message_ptr p_msg_)
{
    std::cout << "Recv msg type: " << type_ << std::endl;
    m_dispatcher.on_message(type_, p_msg_);
}



/**********************************************
 *
 *  msg process function
 *
 */

void RouterConn::handle_allocate_port(pb_message_ptr p_msg_)
{
    try
    {

        // 获得分配端口, 开始监听
        GOOGLE_PROTOBUF_VERIFY_VERSION;
        using namespace google::protobuf;

        auto descriptor = p_msg_->GetDescriptor();
        const Reflection* rf = p_msg_->GetReflection();
        const FieldDescriptor* f_port = descriptor->FindFieldByName("port");


        assert(f_port && f_port->type()==FieldDescriptor::TYPE_INT32);


        int32_t port = rf->GetInt32(*p_msg_, f_port);

        cout << "allocate port: " << port << endl;
        g->set_listen_port(port);
        g->connect_router_after();

    }
    catch (exception& e)
    {
        cout << "# ERR: exception in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what() << endl;
    }
}



void RouterConn::handle_user_chat(pb_message_ptr p_msg_)
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



        // 玩家在这个服务器？
        ImUser* pImUser = UserManager::get_instance()->get_user(recv_id);
        if (pImUser != nullptr)
        {
            CMsg packet;
            packet.encode(1900, *p_msg_);

            // 找到链接
            connection_ptr conn = pImUser->get_conn();
            if (conn != nullptr)
            {
                send(packet, conn->socket());
            }
            else
            {
                cout << "error: " << __FUNCTION__ <<  "Recv User(" << recv_id << ") conn isn't exists!" << endl;
            }
        }
        else
        {
            cout << "error: " << __FUNCTION__ << " Recv User(" << recv_id << ") isn't exists!" << endl;
        }


    }
    catch (exception& e)
    {
        cout << "# ERR: exception in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what() << endl;
    }


}


void send_to_router (CMsg& msg)
{
    if (g_router_handler)
        g_router_handler->send(msg);
}


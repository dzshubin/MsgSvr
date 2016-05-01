#include "LoginConn.hpp"
#include "ClientMsgTypeDefine.hpp"
#include "Server.h"
#include "MsgStruct.hpp"
#include "User.hpp"
#include "UserManager.hpp"
#include  <boost/archive/binary_oarchive.hpp>


#include "register.pb.h"
#include "look_up_result.pb.h"

static LoginConn* g_login_handler = nullptr;


/****************************************
 *
 *
 */

LoginConn::LoginConn(io_service& io_)
  :Connection(io_)
{

}


void LoginConn::on_connect()
{

    m_dispatcher.register_message_callback((int)M2L::LOOKUP,
        bind(&LoginConn::handle_lookup,                     this, std::placeholders::_1));


    g_login_handler = this;

    register_msgsvr();
    read_head();
}


void LoginConn::on_disconnect()
{

}


void LoginConn::on_recv_msg(int type_,  pb_message_ptr p_msg_)
{
    std::cout << "Recv msg type: " << type_ << std::endl;
    m_dispatcher.on_message(type_, p_msg_);
}



void send_to_login (CMsg& msg)
{
    if (g_login_handler)
        g_login_handler->send(msg);
}




/****************************************
 *
 *
 */

void LoginConn::register_msgsvr()
{

    IM::Register msgsvr;
    msgsvr.set_port(g->get_listen_port());


    CMsg packet;
    packet.encode((int)M2L::REGISTER, msgsvr);
    send(packet);
}


void LoginConn::handle_lookup(pb_message_ptr p_msg_)
{
    TRY

        auto descriptor = p_msg_->GetDescriptor();
        const Reflection* rf = p_msg_->GetReflection();
        const FieldDescriptor* f_req_id = descriptor->FindFieldByName("id");

        assert(f_req_id && f_req_id->type()==FieldDescriptor::TYPE_INT64);


        int id = rf->GetInt64(*p_msg_, f_req_id);
        ImUser* pUser = UserManager::get_instance()->get_user(id);



        IM::LookUpRes res;
        res.set_id(id);
        if(pUser != nullptr)
        {
            res.set_is_logined(1);
        }
        else
        {
            res.set_is_logined(0);
        }

        CMsg packet;
        packet.encode((int)M2L::LOOKUP, res);
        send_to_login(packet);


    CATCH


}



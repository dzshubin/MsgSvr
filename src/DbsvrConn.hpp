#ifndef DBSVRHANDLER_HPP_INCLUDED
#define DBSVRHANDLER_HPP_INCLUDED


#include "Connection.hpp"
#include "MessageDispatcher.h"


#include "channel_base_info.pb.h"
#include "contacts.pb.h"

class DBSvrConn: public Connection
{
public:
    DBSvrConn (io_service& io_);

    virtual void on_connect() override;
    virtual void on_recv_msg(int, pb_message_ptr) override;
    virtual void on_disconnect() override;

private:
    void handle_fetch_info(pb_message_ptr );
    void handle_fetch_contacts(pb_message_ptr);
    void handle_fetch_offline_message(pb_message_ptr);
    /// 加载频道返回
    void handle_fetch_channels(pb_message_ptr);
    /// 加入频道返回
    void handle_join_channel(pb_message_ptr);
    /// 退出频道返回
    void handle_exit_channel(pb_message_ptr);
    /// 读取频道用户信息返回
    void handle_channel_user_update(pb_message_ptr);


private:
    // 加载频道信息
    void LoadChannel();
    void GetInstance(google::protobuf::Message*, IM::User&);

private:
    MessageDispatcher m_dispatcher;
};


void send_to_db(CMsg&);


#endif // DBSVRHANDLER_HPP_INCLUDED

#ifndef CLIENTCONN_HPP_INCLUDED
#define CLIENTCONN_HPP_INCLUDED

#include "Connection.hpp"
#include "MessageDispatcher.h"

#include <boost/asio/ip/tcp.hpp>

using namespace boost::asio;
using namespace std;


class ClientConn: public Connection
{
public:
    ClientConn(io_service&);

public:
    virtual void on_connect() override;
    virtual void on_recv_msg(int, pb_message_ptr) override;
    virtual void on_disconnect() override;


private:
    /// 玩家登陆
    void handle_client_login(pb_message_ptr);
    /// 聊天
    void handle_chat(pb_message_ptr);
    /// 频道聊天
    void handle_channel_chat(pb_message_ptr);
    /// 拉取联系人信息
    void handle_fetch_contacts(pb_message_ptr);
    /// 玩家加入频道
    void handle_join_channel(pb_message_ptr);
    /// 玩家离开频道
    void handle_exit_channel(pb_message_ptr);
    /// 请求频道里玩家信息
    void handle_channel_user_update(pb_message_ptr);
    /// 发送文件
    void handle_file_translation(pb_message_ptr);
    /// 读取用户历史消息
    void handle_fetch_history(pb_message_ptr);
    /// 读取频道历史消息
    void handle_fetch_channel_history(pb_message_ptr);

private:
    ///
    bool SendChannelChatToUser(int64_t, const google::protobuf::Message&);


private:
    MessageDispatcher m_dispatcher;


};

#endif // CLIENTCONN_HPP_INCLUDED

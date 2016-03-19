#ifndef CONNECTION_HPP_INCLUDED
#define CONNECTION_HPP_INCLUDED

#include <array>
#include <string>
#include <iostream>

#include <boost/noncopyable.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/placeholders.hpp>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

#include "CMsg.hpp"


using namespace std;
using namespace boost::asio;





class Connection
    :public enable_shared_from_this<Connection>,
     private boost::noncopyable
{
public:
    typedef boost::system::error_code   err_code;

public:
    Connection (io_service& );
    virtual ~Connection();

    virtual void start() = 0;
    // 消息分发
    virtual void process_msg(int type_, string buf_) = 0;

    void stop();

    void on_connect();

public:
    ip::tcp::socket& socket();

    // 读取头部信息
    void read_head();
    // 读取制定长度数据
    void read_body(int len);

    // 发送数据包到指定 socket
    virtual void send(CMsg&, ip::tcp::socket& sock_);
    void send(CMsg&);



public:
    inline int get_id () { return m_ConnId; }


protected:
    // 反序列数据为指定类型结构体
    template <class T>
    void deserialization(T& t, string buf_)
    {
        istringstream is(buf_);
        boost::archive::text_iarchive ia(is);
        ia & t;
    }

    // 打包数据
    void encode(CMsg&);
    // 解包
    void decode();
    // 将字符串解析为int32
    int32_t AsInt32 (const char* buf);
    // 根据类名构建protobuf消息体
    // 返回nullptr如果没有该类名信息
    shared_ptr<google::protobuf::Message> CreateMessage(const string&);



private:
    void handle_read_head(const err_code&, std::size_t);
    void handle_read_body(const err_code&, std::size_t);

    void handle_write(const err_code&, std::size_t);



private:
    // 关闭socket之后做的事情
    void stop_after();



private:
    // 连接
    ip::tcp::socket m_sock;
    // 确保异步handler调用不会并发执行
    io_service::strand m_strand;
    // 保存从socket读取的数据
    boost::asio::streambuf m_rBuf;
    // 头部信息
    char head_info[sizeof(int32_t)];
    // 发送数据
    string m_strSendData;


private:
    // 连接标志
    int m_ConnId;
};



typedef std::shared_ptr<Connection> connection_ptr;

#endif // Connection_HPP_INCLUDED

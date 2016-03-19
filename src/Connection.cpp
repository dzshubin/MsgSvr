
#include "Connection.hpp"
#include <arpa/inet.h>

#include "ConnManager.hpp"
#include "UserManager.hpp"

#include <boost/bind.hpp>


/**********************************************
 *
 * Constructor and Deconstructor
 *
 */

Connection::Connection(io_service& io_service_)
  :m_sock(io_service_), m_strand(io_service_)
{

}


Connection::~Connection()
{

}


/**********************************************
 *
 *
 *
 */

void Connection::read_head()
{

    cout << "start read head info!" << endl;
    auto self = shared_from_this();


    async_read(m_sock, boost::asio::buffer(head_info),
        m_strand.wrap(
            boost::bind(&Connection::handle_read_head, shared_from_this(),
                      boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
        ));
}

void Connection::read_body(int len_)
{
    auto self = shared_from_this();

    async_read(m_sock, m_rBuf, transfer_exactly(len_),
        m_strand.wrap(
            boost::bind(&Connection::handle_read_body, shared_from_this(),
                      boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
        ));
}



void Connection::send(CMsg& msg, ip::tcp::socket& sock_)
{
    encode(msg);

    cout << "start send msg." << endl;
    auto self = shared_from_this();


    async_write(sock_, boost::asio::buffer(m_strSendData),
        m_strand.wrap(
            boost::bind(&Connection::handle_write, shared_from_this(),
                      boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
        ));
}


void Connection::send(CMsg& msg)
{
    encode(msg);

    cout << "start send msg." << endl;
    auto self = shared_from_this();

    async_write(m_sock, boost::asio::buffer(m_strSendData),
        m_strand.wrap(
            boost::bind(&Connection::handle_write, shared_from_this(),
                      boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
        ));
}



ip::tcp::socket& Connection::socket()
{
    return m_sock;
}



void Connection::stop()
{
    m_sock.close();

    stop_after();
}



void Connection::stop_after()
{
    // 从连接器中删除连接
    ConnManager::get_instance()->remove(shared_from_this());

    int user_id = ConnManager::get_instance()->get_user_id(m_ConnId);
    ImUser *pImUser = UserManager::get_instance()->get_user(user_id);
    if (pImUser)
    {
        // 删除用户
        UserManager::get_instance()->remove(pImUser);
    }
    else
    {

    }
}



void Connection::on_connect()
{
    // 分配连接标志
    m_ConnId = 1;

    ConnManager::get_instance()->insert(shared_from_this());
    start();
}



/**********************************************
 *
 * tool function
 *
 */

int32_t Connection::AsInt32 (const char* buf)
{
    int32_t buf_len = 0;
    std::copy(buf, buf + sizeof(int32_t), reinterpret_cast<char*>(&buf_len));
    return ::ntohl(buf_len);
}


shared_ptr<google::protobuf::Message> Connection::CreateMessage(const string& type_name)
{
    using namespace google::protobuf;

    try
    {
        const DescriptorPool* pDescPool = DescriptorPool::generated_pool();

        if(pDescPool == nullptr)
        {
            return nullptr;
        }

        const Descriptor* dr = pDescPool->FindMessageTypeByName(type_name);
        if (dr == nullptr)
        {
            return nullptr;
        }

        const Message* proto = MessageFactory::generated_factory()->GetPrototype(dr);

        if (proto == nullptr)
        {
            return nullptr;
        }

        shared_ptr<Message> p_message(proto->New());
        return p_message;

    }
    catch (exception ec)
    {
        cout << "# ERR: exception in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << ec.what() << endl;
        return nullptr;
    }
}




void Connection::encode(CMsg& msg)
{
    cout << "start encode msg" << endl;

    m_strSendData.clear();

    // 为头部预留空间
    m_strSendData.resize(sizeof(int32_t));

    // 消息类型
    int32_t type = msg.get_msg_type();
    int32_t be_type = ::htonl(type);
    m_strSendData.append(reinterpret_cast<char*>(&be_type), sizeof(be_type));

    // 序列化数据
    m_strSendData.append(msg.get_send_data().c_str(), msg.send_data_len() + 1);


    // 数据总长度
    int32_t len = sizeof(int32_t) + msg.send_data_len() + 1;
    int32_t be_len = ::htonl(len);
    std::copy(reinterpret_cast<char*>(&be_len),
              reinterpret_cast<char*>(&be_len) + sizeof(be_len),
              m_strSendData.begin());


    cout << "data len: " << len << endl;
    cout << "send str size: " << m_strSendData.size() << endl;
}



void Connection::decode()
{
    ostringstream os;
    os << &m_rBuf;

    // 收到的数据
    string trans_data = os.str();

    // 消息类型
    int32_t type = AsInt32(trans_data.c_str());

    // 序列化数据
    std::string buf = trans_data.substr(sizeof(int32_t));
    process_msg(type, buf);
}





/**********************************************
 *
 *  callback
 */

void Connection::handle_read_head(const err_code& ec, std::size_t byte_trans)
{
    if (!ec)
    {

        int32_t data_len = AsInt32(head_info);
        cout << data_len <<endl;

        // 开始读数据体
        read_body(data_len);
    }
    else
    {
        cout << "# ERR: exception in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << ec.message() << endl;

        // 关闭连接
        stop();
    }
}




void Connection::handle_read_body(const err_code& ec, std::size_t byte_trans)
{

    if (!ec)
    {
        cout << "readed data size: " << byte_trans <<endl;
        decode();

        // 继续读取数据
        read_head();
    }

    else
    {
        cout << "# ERR: exception in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << ec.message() << endl;

        stop();

    }
}



void Connection::handle_write(const err_code& ec, std::size_t byte_trans)
{
    if (!ec)
    {
        cout << "sended data len: " << byte_trans << endl;
    }

    else
    {
        cout << "# ERR: exception in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << ec.message() << endl;

        stop();
    }
}



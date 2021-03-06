#ifndef MSGSTRUCT_HPP_INCLUDED
#define MSGSTRUCT_HPP_INCLUDED

#include <string>
#include <boost/serialization/vector.hpp>



struct MSG_LOGIN_ID
{
    MSG_LOGIN_ID ()
    {

    }

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & m_nId;
    }

    int64_t m_nId;
};

struct Msg_msgsvr_register
{
    Msg_msgsvr_register () :m_port (0)
    {

    }

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & m_port;
    }

    int m_port;   // 监听的端口
};


struct Msg_update_count
{
    Msg_update_count() :m_user_count(0)
    {

    }

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & m_user_count;
    }

    int m_user_count;
};


struct Msg_allocate_port
{
    Msg_allocate_port () :m_port(0)
    {

    }

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & m_port;
    }

    int m_port;
};



struct Msg_chat
{
    Msg_chat ():m_send_id(0), m_recv_id(0)
    {

    }

    int64_t m_send_id;
    int64_t m_recv_id;
    std::string m_content;


    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & m_send_id;
        ar & m_recv_id;
        ar & m_content;
    }
};




struct Msg_user_info
{

    Msg_user_info ()
    {
    }

    int64_t m_nId;
    string m_strName;
    string m_strNickName;

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & m_nId;
        ar & m_strName;
        ar & m_strNickName;
    }
};

struct USER
{
    int64_t     m_user_id;      // id
    string      m_name;         // 姓名
    string      m_nick_name;    // 昵称

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & m_user_id;
        ar & m_name;
        ar & m_nick_name;
    }

};


struct MSG_CONTACTS
{
    MSG_CONTACTS() :m_req_id(0)
    {

    }

    int64_t             m_req_id;       // 请求者id
    std::vector<USER>   m_contacts;


    template< typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & m_req_id;
        ar & m_contacts;
    }
};



#endif // MSGSTRUCT_HPP_INCLUDED

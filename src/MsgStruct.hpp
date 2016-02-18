#ifndef MSGSTRUCT_HPP_INCLUDED
#define MSGSTRUCT_HPP_INCLUDED

#include <string>

struct Msg_login
{
    Msg_login ()
    {

    }

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & m_nId;
    }

    uint64_t m_nId;
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

    uint64_t m_send_id;
    uint64_t m_recv_id;
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

    uint64_t m_nId;
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

#endif // MSGSTRUCT_HPP_INCLUDED

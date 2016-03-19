#ifndef USER_HPP_INCLUDED
#define USER_HPP_INCLUDED



#include <string>
#include "Connection.hpp"

using namespace std;

class ImUser
{
public:
    ImUser();

    inline string get_name ()  { return m_strName; }
    inline void    set_name (string name_) { m_strName = name_; }

    inline const int64_t get_id () const  { return m_nId; }
    inline void  set_id (int64_t id_) { m_nId = id_; }

    inline int64_t get_id () { return m_nId; }

    inline string get_nick_name ()  { return m_strNickName; }
    inline void    set_nick_name (string nick_name_) { m_strNickName = nick_name_; }

    inline connection_ptr get_conn() { return  m_conn; }
    inline void set_conn(connection_ptr conn_) { m_conn = conn_; }


private:
    int64_t m_nId;
    string m_strName;       // 名字
    string m_strNickName;   // 昵称

    connection_ptr m_conn;

};


#endif // USER_HPP_INCLUDED

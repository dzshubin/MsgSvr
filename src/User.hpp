#ifndef USER_HPP_INCLUDED
#define USER_HPP_INCLUDED



#include <string>


using namespace std;

class User
{
public:
    User();

    inline string get_name ()  { return m_strName; }
    inline void    set_name (string name_) { m_strName = name_; }

    inline const uint64_t get_id () const  { return m_nId; }
    inline void  set_id (uint64_t id_) { m_nId = id_; }

    inline uint64_t get_id () { return m_nId; }

    inline string get_nick_name ()  { return m_strNickName; }
    inline void    set_nick_name (string nick_name_) { m_strNickName = nick_name_; }


private:
    uint64_t m_nId;
    string m_strName;       // 名字
    string m_strNickName;   // 昵称
};


#endif // USER_HPP_INCLUDED

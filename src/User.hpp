#ifndef USER_HPP_INCLUDED
#define USER_HPP_INCLUDED



#include <string>
using namespace std;



class User
{
public:
    User();

    inline string& get_name ()  { return m_strName; }
    inline void    set_name (string name_) { m_strName = name_; }

    inline string& get_id ()  { return m_strId; }
    inline void    set_id (string id_) { m_strId = id_; }

    inline string& get_nick_name ()  { return m_strNickName; }
    inline void    set_nick_name (string nick_name_) { m_strNickName = nick_name_; }


private:
    string m_strId;
    string m_strName;       // 名字
    string m_strNickName;   // 昵称
};


#endif // USER_HPP_INCLUDED

#ifndef USERMANAGER_HPP_INCLUDED
#define USERMANAGER_HPP_INCLUDED

#include <vector>
#include <iostream>
#include <algorithm>


#include "User.hpp"
#include "typeinfo.hpp"
//class ClientHandler;

class UserManager
{
public:
    static UserManager* get_instance();

    //void insert_user(ClientHandler* context, int id_);
    void insert_user(User&);

    bool find_user (UL64 id_);
    // 获得玩家总数
    int get_user_size();

private:
    UserManager () {};

private:
    std::vector<User> m_users;
};




#endif // USERMANAGER_HPP_INCLUDED

#ifndef USERMANAGER_HPP_INCLUDED
#define USERMANAGER_HPP_INCLUDED

#include <vector>
#include <iostream>
#include <algorithm>


#include "User.hpp"


class UserManager
{
public:
    static UserManager* get_instance();

    // 添加用户
    void insert_user(User);

    // 玩家是否存在
    bool find_user (int64_t id_);

    // 获得玩家总数
    int size();

private:
    UserManager () {};

private:
    std::vector<User> m_users;
};




#endif // USERMANAGER_HPP_INCLUDED

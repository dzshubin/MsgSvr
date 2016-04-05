#ifndef USERMANAGER_HPP_INCLUDED
#define USERMANAGER_HPP_INCLUDED

#include <set>
#include <tuple>
#include <iostream>
#include <algorithm>


#include "User.hpp"


class UserManager
{
public:
    static UserManager* get_instance();

    // 添加用户
    void insert(ImUser*);

    void remove(ImUser*);
    // 删除与指定connid的玩家
    std::tuple<bool, int64_t> remove(int conn_id);
    // 通过玩家id获得玩家信息
    ImUser* get_user(int64_t id_);

    // 获得玩家总数
    int size();

private:
    UserManager () {};

private:
    std::set<ImUser*> m_users;
};




#endif // USERMANAGER_HPP_INCLUDED

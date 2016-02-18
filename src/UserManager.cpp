#include "UserManager.hpp"


UserManager* UserManager::get_instance()
{
    static UserManager g_user_mananger;
    return &g_user_mananger;
}


void UserManager::insert_user(User& u)
{
    m_users.push_back(u);
}

int UserManager::get_user_size()
{
    return m_users.size();
}

bool UserManager::find_user(uint64_t id_)
{
    auto it = find_if(m_users.cbegin(), m_users.cend(),
                [&id_] (const User& user)
                {
                    return (user.get_id()) == id_;
                });

    return it != m_users.end() ? true : false;
}

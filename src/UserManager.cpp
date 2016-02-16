#include "UserManager.hpp"


UserManager* UserManager::get_instance()
{
    static UserManager g_user_mananger;
    return &g_user_mananger;
}


void UserManager::insert_user(ClientHandler* context_, int id_)
{
    std::cout << "inset user.id: " << id_ << std::endl;
    //m_users.emplace_back(context_, id_);
}


void UserManager::insert_user(User& u)
{
    m_users.push_back(u);
}


//void* UserManager::get_context_by_id(int id_)
//{
//    auto it = std::find_if(m_users.begin(), m_users.end(),
//                           [=] (User& u)
//    {
//        std::cout << "keyid: "<< id_ <<"search id: " << u.get_id() << std::endl;
//        return u.get_user_id() == id_;
//    });
//
//    return it == m_users.end() ? nullptr : it->get_context();
//}

int UserManager::get_user_size()
{
    return m_users.size();
}

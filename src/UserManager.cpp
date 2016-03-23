#include "UserManager.hpp"


UserManager* UserManager::get_instance()
{
    static UserManager g_user_mananger;
    return &g_user_mananger;
}


void UserManager::insert(ImUser* pUser)
{
    m_users.insert(pUser);
    cout << "now users size: " << m_users.size() << endl;
}

void UserManager::remove(ImUser* pUser_)
{
    m_users.erase(pUser_);
    cout << "now users size: " << m_users.size() << endl;
}

bool UserManager::remove(int conn_id_)
{
    auto it = find_if(m_users.begin(), m_users.end(),
        [&] (ImUser* pUser)
        {
            return pUser->get_conn()->get_id() == conn_id_;
        });


    if (it == m_users.end())
    {
        return false;
    }
    else
    {

        cout << "user disconnect! user id: " << ((*it)->get_id())  << endl;

        m_users.erase(it);
        (*it)->free_conn();
        delete (*it);

        return true;
    }
}


int UserManager::size()
{
    return m_users.size();
}


ImUser* UserManager::get_user(int64_t id_)
{
    auto it = find_if(m_users.begin(), m_users.end(),
        [&] (ImUser* user_)
        {
            return user_->get_id() == id_;
        });

    if (it == m_users.end())
    {
        return nullptr;
    }
    else
    {
        return *it;
    }
}

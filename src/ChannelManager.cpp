

#include "ChannelManager.hpp"
#include <algorithm>
#include <iterator>



ChannelManager::ChannelManager()
{


}

ChannelManager* ChannelManager::get_instance()
{
    static ChannelManager g_channel_mananger;
    return &g_channel_mananger;
}




// 加入频道
bool ChannelManager::JoinChannel(int ch_id, const IM::User& r_user_)
{
    auto it = m_mapChannels.find(ch_id);
    if (it != m_mapChannels.end())
    {
        auto vec_it = std::find_if(it->second.m_users.begin(), it->second.m_users.end(),
        [&] (const IM::User& user_)
        {
            return user_.id() == r_user_.id();
        });
        if (vec_it != it->second.m_users.end())
        {
            // 已经加入了频道
            return false;
        }
        else
        {
            it->second.m_users.push_back(r_user_);
        }
    }
    else
    {
        // 频道不存在
        //m_mapChannels[ch_id].push_back(r_user_);
        return false;
    }

    return true;
}

// 退出频道
bool ChannelManager::ExitChannel(int ch_id, int64_t user_id_)
{
    auto it = m_mapChannels.find(ch_id);

    if(it != m_mapChannels.end())
    {
        auto vec_it = std::find_if(it->second.m_users.begin(), it->second.m_users.end(),
        [&] (const IM::User& user_)
        {
            return user_.id() == user_id_;
        });

        if (vec_it != it->second.m_users.end())
        {
            it->second.m_users.erase(vec_it);
        }
        else
        {
            // 玩家不是该频道的成员
            return false;
        }
    }

    else
    {
        // 频道不存在
        return false;
    }
}



bool ChannelManager::CreateChannel(int ch_id_, std::string name_)
{
    auto it = m_mapChannels.find(ch_id_);
    if (it == m_mapChannels.end())
    {
        CHANNEL channel;
        channel.m_name = name_;

        m_mapChannels.insert(std::make_pair(ch_id_, channel));
        return true;
    }
    else
    {
        // 频道已经存在
        return false;
    }
}


bool ChannelManager::RemoveChannel(int ch_id_)
{
    return false;
}



void ChannelManager::LoadChannelBaseInfo(int64_t user_id_, IM::ChannelBaseInfo& base_info_)
{
    auto it = m_mapChannels.cbegin();

    for(; it != m_mapChannels.cend(); ++it)
    {
        IM::ChannelBase* pCB = base_info_.add_channel_base();

        pCB->set_channel_id(it->first);
        pCB->set_channel_name(it->second.m_name);

        auto res_it = std::find_if(it->second.m_users.cbegin(), it->second.m_users.cend(),
        [&](const IM::User& user_)
        {
            return user_.id() == user_id_;
        });

        bool result = (res_it == it->second.m_users.end() ? false : true);
        pCB->set_is_inside(result);

    }


}

void ChannelManager::LoadChannelMembers(IM::ChannelMembersInfo& members)
{
    auto it = m_mapChannels.cbegin();

    for(; it != m_mapChannels.end(); ++it)
    {
        IM::ChannelMember *pMmember = members.add_channel_member();
        pMmember->set_channel_id(it->first);

        std::vector<IM::User> vec_users = it->second.m_users;
        auto user_it = vec_users.cbegin();
        for(; user_it != vec_users.cend(); ++user_it)
        {
            IM::User* pUser = pMmember->add_users();

            pUser->set_id(user_it->id());
            pUser->set_name(user_it->name());
            pUser->set_nick_name(user_it->nick_name());
            pUser->set_sex(user_it->sex());
        }
    }



}


bool ChannelManager::UpdateUser(int channel_id_, const IM::User& user_)
{

    auto it = m_mapChannels.find(channel_id_);
    if (it != m_mapChannels.end())
    {
        int64_t user_id = user_.id();
        auto user_it = find_if (it->second.m_users.begin(), it->second.m_users.end(),
        [&] (IM::User& v_user_)
        {
            return v_user_.id() == user_id;
        });


        if (user_it != it->second.m_users.end())
        {
            user_it->set_id(user_.id());
            user_it->set_name(user_.name());
            user_it->set_nick_name(user_.nick_name());
            user_it->set_sex(user_.sex());

            return true;
        }
        else
        {
            // not find user id
            return false;
        }
    }
    else
    {
        // not find ch_id
        return false;
    }

}


bool ChannelManager::GetUserIds(int ch_id_, std::vector<int64_t>& vec_)
{
    auto it = m_mapChannels.find(ch_id_);
    if (it != m_mapChannels.end())
    {
        std::for_each(it->second.m_users.begin(), it->second.m_users.end(),
        [&] (const IM::User& user_)
        {
            vec_.push_back(user_.id());
        });
        return true;
    }
    else
    {
        //  not find ch_id
        return false;
    }





}







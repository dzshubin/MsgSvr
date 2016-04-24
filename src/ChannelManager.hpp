#ifndef CHANNELMANAGER_HPP_INCLUDED
#define CHANNELMANAGER_HPP_INCLUDED



#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include "contacts.pb.h"
#include "channel_base_info.pb.h"
#include "channel_members.pb.h"

struct CHANNEL
{
    std::string m_name;
    std::vector<IM::User> m_users;
};


// 频道管理器
class ChannelManager
{

public:
    static ChannelManager* get_instance();


    // 创建频道
    bool CreateChannel(int, std::string);
    // 删除频道
    bool RemoveChannel(int);
    // 加入频道
    bool JoinChannel(int, const IM::User&);
    // 退出频道
    bool ExitChannel(int, int64_t);
    // 加载频道基本信息
    void LoadChannelBaseInfo(int64_t, IM::ChannelBaseInfo&);
    // 加载频道成员信息
    void LoadChannelMembers(IM::ChannelMembersInfo&);
    // 更新频道信息
    bool UpdateUser(int, const IM::User&);

private:
    ChannelManager();

private:
    // map<频道号，频道信息>
    std::map<int, CHANNEL>  m_mapChannels;
};



#endif // CHANNELMANAGER_HPP_INCLUDED

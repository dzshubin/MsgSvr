#ifndef CLIENTMSGTYPEDEFINE_HPP_INCLUDED
#define CLIENTMSGTYPEDEFINE_HPP_INCLUDED

/*********************************************************************************
 *Copyright(C),2010-2011,Your Company
 *FileName: // 文件名
 *Author: shub//作者
 *Version: //版本
 *Date: 2015-1-21//创建日期
 *Description: 消息协议定义//用于主要说明此程序文件完成的主要功能
 *********************************************************************************/


//
//

/*
 * 客户端发给服务器的消息类型编号
 * 服务器返回给客户端的消息编号一致
 *
 * 注释格式:
 * 消息用途
 * @parm    客户端请求的消息参数_: 参数意思
 * @return  服务器返回的消息参数 : 参数意思
 */
enum class C2L
{
    /*
     * 玩家登陆
     * @parm       id_ : 玩家id
     * @retruen    res : 登陆结果 0-成功  1-失败
     */
    UserLogin                       = 1000,
    UserChat                        = 1001,
};



// 消息svr发给路由SVr
enum class M2R
{
    UserChat                        = 2000,
    UserLogin                       = 2001,
    AllocatePort                    = 2003,
};


// 消息svr发给登陆SVr
enum class M2L
{
    Register                        = 7000,
    UpdateMsgSvr                    = 7001,     // 更新当前消息服务器总人数
};

// 消息svr发给DBSVr
enum class M2D
{
    /*
     * 通过用户id读取用户信息
     */
    read_info_by_id                 = 4000,
};




#endif // CLIENTMSGTYPEDEFINE_HPP_INCLUDED

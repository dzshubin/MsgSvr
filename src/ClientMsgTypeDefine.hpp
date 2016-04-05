#ifndef CLIENTMSGTYPEDEFINE_HPP_INCLUDED
#define CLIENTMSGTYPEDEFINE_HPP_INCLUDED


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
enum class C2M
{
    /*
     * 玩家登陆
     * @parm       id_ : 玩家id
     * @retruen    res : 登陆结果 0-成功  1-失败
     */
    LOGIN                   = 1000,

    /*
     * 聊天
     * @parm send_id    : 发送者id
     * @parm recv_id    : 接受者id
     * @parm content    : 发送内容
     */
    CHAT                    = 1001,

    /*
     * 请求联系人列表
     * @parm    user_id
     * @return  int 总人数,
     *          循环
     *          [
     *              int     id,         // 用户id
     *              string  name,       // 姓名
     *              string  nick_name   // 昵称
     *          ]
     */
    FETCH_CONTACTS          = 1002,

};



// msgsvr <--> routersvr
enum class M2R
{
    DISPATCH_CHAT                        = 2000,
    LOGIN                                = 2001,
    LOGOUT                               = 2002,
    ALLOCATE_PORT                        = 2003,
};

// routersvr --> msgsvr
enum class R2M
{
    RECV_CHAT                   = 3000,
};


// 消息svr发给登陆SVr
enum class M2L
{
    REGISTER                            = 7000,
    UPDATE                              = 7001,     // 更新当前消息服务器总人数
};


// 消息svr发给DBSVr
enum class M2D
{
    /*
     * 通过用户id读取用户信息
     */
    READ_INFO                   = 5000,

    /*
     * 读取指定用户id的联系人
     */
    FETCH_CONTACTS              = 5001,
};




#endif // CLIENTMSGTYPEDEFINE_HPP_INCLUDED

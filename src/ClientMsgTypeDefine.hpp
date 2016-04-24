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
    LOGIN                           = 1000,

    /*
     * 聊天
     * @parm send_id    : 发送者id
     * @parm recv_id    : 接受者id
     * @parm content    : 发送内容
     */
    CHAT                            = 1001,

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
    FETCH_CONTACTS                  = 1002,

    /*
     *
     * 请求加入某个频道
     * @parm user_id    : 用户id
     * @parm channel_id : 频道id
     *
     */
    JOIN_CHANNEL                    = 1003,
    /*
     *
     * 请求离开某个频道
     * @parm user_id    : 用户id
     * @parm channel_id : 频道id
     *
     */
    EXIT_CHANNEL                    = 1004,


    /*
     *
     * 请求更新某个用户信息
     * @parm user_id    : 用户id
     * @parm channel_id : 频道id
     *
     */
    CHANNEL_USER_UPDATE             = 1005,


    // 以下为服务器主动发给客户端的消息
    SEND_OFFLINE_MESSAGE            = 10000,
    SEND_CHANNELS                   = 10001,
    SEND_CHANNEL_MEMBERS            = 10002,
};



// msgsvr <--> routersvr
enum class M2R
{
    DISPATCH_CHAT                           = 2000,
    LOGIN                                   = 2001,
    LOGOUT                                  = 2002,
    ALLOCATE_PORT                           = 2003,
};

// routersvr --> msgsvr
enum class R2M
{
    RECV_CHAT                               = 3000,
};


// 消息svr发给登陆SVr
enum class M2L
{
    REGISTER                                = 7000,
    UPDATE                                  = 7001,     // 更新当前消息服务器总人数
};


// 消息服务器与数据库服务器之间通信
enum class M2D
{
    /*
     * 通过用户id读取用户信息
     */
    READ_INFO                               = 5000,

    /*
     * 读取指定用户id的联系人
     */
    FETCH_CONTACTS                          = 5001,
    /*
     * 请求离线消息
     * @parm    user_id
     * @return  int 总消息数,
     *          循环
     *          [
     *              int     id,            // 发送者id
     *              string  content,       // 内容
     *          ]
     */
     FETCH_OFFLINE_MESSAGE                  = 5002,

     /*
      *  保存历史信息
      */
     SAVE_TO_HISTORY                        = 5003,

     /*
      * 请求频道信息
      */
     FETCH_CHANNELS                         = 5010,

     /*
      * 请求加入频道
      */
     JOIN_CHANNEL                           = 5011,
     /*
      * 请求频道某个用户信息
      */
     CHANNEL_USER_UPDATE                    = 5012,
     /*
      * 请求离开频道
      */
     EXIT_CHANNEL                           = 5013,

};




#endif // CLIENTMSGTYPEDEFINE_HPP_INCLUDED

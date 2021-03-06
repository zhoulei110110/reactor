/**
 * @file    xftp_client.h
 * <pre>
 * Copyright (c) 2019, Gaaagaa All rights reserved.
 * 
 * 文件名称：xftp_client.h
 * 创建日期：2019年02月18日
 * 文件标识：
 * 文件摘要：xftp 的客户端连接类。
 * 
 * 当前版本：1.0.0.0
 * 作    者：
 * 完成日期：2019年02月18日
 * 版本摘要：
 * 
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 * 版本摘要：
 * </pre>
 */

#ifndef __XFTP_CLIENT_H__
#define __XFTP_CLIENT_H__

#include "xftp_cliworker.h"

////////////////////////////////////////////////////////////////////////////////
// x_ftp_client_t

/**
 * @class x_ftp_client_t
 * @brief xftp 的客户端连接类。
 */
class x_ftp_client_t : public x_ftp_cliworker_t
{
    // common data types
public:
    /**
     * @enum  emIoContextCmid
     * @brief IO 操作的命令 ID 表。
     */
    typedef enum emIoContextCmid
    {
        CMID_WCLI_LOGIN  = 0x0020,  ///< 网络连接类型（建立网络连接）
        CMID_WCLI_HBEAT  = 0x2000,  ///< 心跳
        CMID_WCLI_FLIST  = 0x3010,  ///< 获取文件列表
    } emIoContextCmid;

    /** 文件列表[ 文件名，文件大小 ] */
    using x_list_file_t = std::list< std::pair< std::string, x_int64_t > >;

    // constructor/destructor
public:
    x_ftp_client_t(void);
    virtual ~x_ftp_client_t(void);

    // overrides
public:
    /**********************************************************/
    /**
     * @brief 连接类型。
     */
    virtual x_uint16_t ctype(void) const override { return CMID_WCLI_LOGIN; }

protected:
    /**********************************************************/
    /**
     * @brief 投递心跳数据包信息。
     */
    virtual x_void_t send_heartbeat(void) override;

    /**********************************************************/
    /**
     * @brief 收到 IO 消息的通知接口。
     */
    virtual x_void_t io_recved_msgctxt(const x_io_msgctxt_t & xio_msgctxt) override;

    /**********************************************************/
    /**
     * @brief 完成 IO 消息发送的通知接口。
     */
    virtual x_void_t io_sended_msgctxt(const x_io_msgctxt_t & xio_msgctxt) override;

    /**********************************************************/
    /**
     * @brief IO 操作产生错误的通知接口。
     */
    virtual x_void_t io_error_notify(x_int32_t xit_error, x_bool_t xbt_iosend) override;

    // public interfaces
public:
    /**********************************************************/
    /**
     * @brief 登录操作。
     *
     * @param [in ] xszt_host : 目标主机（服务器）的 IP 地址（四段式 IP 地址字符串）。
     * @param [in ] xwt_port  : 目标主机（服务器）的 端口号。
     *
     * @return x_int32_t
     *         - 成功，返回 0；
     *         - 失败，返回 错误码。
     */
    x_int32_t login(x_cstring_t xszt_host, x_uint16_t xwt_port);

    /**********************************************************/
    /**
     * @brief 登出操作。
     */
    x_void_t logout(void);

    /**********************************************************/
    /**
     * @brief 是否已经登录。
     */
    inline x_bool_t is_login(void) const { return (is_startup() && m_xbt_login); }

    /**********************************************************/
    /**
     * @brief 从缓存中读取文件列表。
     */
    x_void_t get_cache_flist(x_list_file_t & xlst_files);

    // internal invoking
protected:
    /**********************************************************/
    /**
     * @brief 处理 IO 应答命令：建立网络连接。
     */
    x_int32_t iocmd_login(x_uint16_t xut_seqn, x_uchar_t * xct_dptr, x_uint32_t xut_size);

    /**********************************************************/
    /**
     * @brief 处理 IO 应答命令：心跳。
     */
    x_int32_t iocmd_hbeat(x_uint16_t xut_seqn, x_uchar_t * xct_dptr, x_uint32_t xut_size);

    /**********************************************************/
    /**
     * @brief 处理 IO 应答命令：获取文件列表。
     */
    x_int32_t iocmd_flist(x_uint16_t xut_seqn, x_uchar_t * xct_dptr, x_uint32_t xut_size);

    // data members
protected:
    x_bool_t         m_xbt_login;    ///< 标识登录成功（网络连接建立成功）

    x_locker_t       m_xlock_flist;  ///< m_cache_flist 的同步访问锁
    x_list_file_t    m_cache_flist;  ///< 缓存的文件列表
};

////////////////////////////////////////////////////////////////////////////////

#define XMKEY_WCLI_IOERR   ((x_uint32_t)(x_ftp_client_t::CMID_WCLI_LOGIN << 16) | (x_uint32_t)x_ftp_client_t::CMID_WCLI_LOGIN)
#define XMKEY_WCLI_FLIST   ((x_uint32_t)(x_ftp_client_t::CMID_WCLI_LOGIN << 16) | (x_uint32_t)x_ftp_client_t::CMID_WCLI_FLIST)

////////////////////////////////////////////////////////////////////////////////

#endif // __XFTP_CLIENT_H__

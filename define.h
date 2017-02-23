#ifndef DEFINE_H
#define DEFINE_H

#include <QApplication>

#define TCP_CMD_CLIENT_LOGIN        1000 //登录
#define TCP_CMD_CLIENT_LOGIN_ERROR  1001 //登录出错
#define TCP_CMD_SERVER_GET_HEAD     1002 //服务器获取文件信息
#define TCP_CMD_CLIENT_GET_HEAD     1003 //客户端获取文件信息
#define TCP_CMD_SOCKET_DISPLAY      1004
#define TCP_CMD_CLIENT_DROPPED      1005 //客户端下线
#define TCP_CMD_LOCAL_LOGIN         1006 //客户端登录
#define TCP_CMD_LOGIN_ERROR         1007 //
#define TCP_CMD_LOGIN_SUCCESS       1008
#define TCP_CMD_HEAD_DATA           2000
#define TCP_CMD_HEAD_ERROR          2009
#define TCP_CMD_FILE_DATA           2001
#define TCP_CMD_FILE_ERROR          2003
#define TCP_CMD_FILE_SUCCESS        2002
#define TCP_CMD_SHEEL_CMD           2004
#define TCP_CMD_SHEEL_RETURN        2005
#define TCP_CMD_LIST_GET            2006
#define TCP_CMD_LIST_ERROR          2007
#define TCP_CMD_FILE_LIST           2008
#define TCP_CMD_HEART               6000

#define ADDR 6000
#define WIN_CMD_SWITCH  7000
#define WIN_CMD_SHOW    7001
#define WIN_CMD_RESULT  7002
#define WIN_CMD_JUDGE   7003
#define WIN_CMD_TEMP    7004
#define WIN_WAVE_BYTE   7005
#define WIN_WAVE_TEST   7006
#define WIN_WAVE_ITEM   7007
#define WIN_CMD_DEBUG   7008
#define WIN_CMD_WAVE    7009

#define CAN_DAT_GET 8000
#define CAN_DAT_PUT 8001

#define CTRL_CMD_START 9000
#define CTRL_CMD_STOP  9001

#define  ResCu    0.0039  //  铜材料
#define  ResCu_Al 0.0041  //  铜铝材料
#define  ResAl    0.0043  //  铝材料


#define NET "./network/"
#define TMP "./temp/"
#define CON "./config/"

#ifdef __arm__
#define GLOBAL_SET "/mnt/nandflash/AIP/Sys.ini"
#else
#define GLOBAL_SET "settings/global.ini"
#endif

#define LOCAL

#define CAN_ID_DLR       0x41         // 电阻板
#define CAN_ID_DLR_WAVE  0x441        // 电阻板反嵌波形
#define CAN_ID_IR        0x61         // 耐压板
#define CAN_ID_IMP       0x81         // 匝间板
#define CAN_ID_IMP_WAVE  0x481        // 匝间板波形
#define CAN_ID_13OUT     0x261        // 输出板13
#define CAN_ID_14OUT     0x281        // 输出板14
#define CAN_ID_15OUT     0x2A1        // 输出板15
#define CAN_ID_16OUT     0x2C1        // 输出板16
#define CAN_ID_17OUT     0x2E1        // 输出板17

#define CAN_CMD_STATE        0x00     //上传状态
#define CAN_CMD_RESULT_ONE   0x01     //上传结果1
#define CAN_CMD_RESULT_TWO   0x02     //上传结果2
#define CAN_CMD_WAVE         0x03     //上传波形
#define CAN_CMD_DEBUG        0x06     //上传调试参数
#define CAN_CMD_VERSION      0x08     //上传版本

//extern

#endif // DEFINE_H

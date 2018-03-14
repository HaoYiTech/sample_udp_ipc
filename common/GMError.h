/******************************************************
 * 浩一科技，提供云录播、云监控的全平台无插件解决方案。
 * 支持按需直播，多点布控，分布式海量存储，动态扩容；
 * 支持微信扫码登录，全平台帐号统一，关联微信小程序；
 * 支持多种数据输入：摄像头IPC、rtmp、rtsp、MP4文件；
 * 支持全实时、全动态、全网页管理，网页前后台兼容IE8；
 * 支持多终端无插件自适应播放，flvjs/hls/rtmp自动适配；
 ******************************************************
 * 官方网站 => https://myhaoyi.com
 * 技术博客 => http://blog.csdn.net/haoyitech
 * 开源代码 => https://github.com/HaoYiTech/
 * 云监控群 => 483663026（QQ群）
 * 云录播群 => 630379661（QQ群）
 ******************************************************
 * 这个头文件是开源代码中经常用到的错误号码汇总
 ******************************************************/

#pragma once

enum
{
	GM_NoErr				= 0,			// OK...
	GM_Continue				= 1,			// Continue...
	GM_NoReply				= 2,			// 不要回复

	GM_Video_Wnd_Exist		= 10001,		// 视频窗口已经存在

	GM_FDFS_Status_Err		= 20001,		// 服务器状态错误
	GM_FDFS_Data_Err		= 20002,		// 服务器数据错误
	
	GM_DVR_VEnc_Err			= 30001,		// 视频压缩类型错误
	GM_DVR_AEnc_Err			= 30002,		// 音频压缩类型错误
	GM_DVR_VType_Err		= 30003,		// 视频类型错误

	GM_Err_Config			= 40007,		// 加载配置文件失败
	GM_BadInputValue		= 42001,		// 错误的输入参数
	GM_NetClose				= 42002,		// 网络已经关闭
	GM_NetNoData			= 42003,		// 没有网络数据
	GM_DiskErr				= 42004,		// 读取磁盘错误
	GM_Net_Err				= 42006,		// 通常是由于IP未配置或网络不通所引起
	GM_None_IPV6			= 42007,		// 没有安装IPV6协议

	GM_BadInputParam		= 50003,		// 输入参数无效
	GM_ProtocolErr			= 50006,		// 无法支持的协议
	GM_URIErr				= 50007,		// 解析URL时错误
	GM_NotImplement			= 50008,		// 没有实现的接口
	GM_Snap_Jpg_Err			= 50009,		// 截图失败
	GM_File_Del_Err			= 50012,		// 删除文件失败
	GM_File_Not_Open		= 50013,		// 打开文件失败
	GM_File_Read_Err		= 50014,		// 读取文件失败
	GM_File_Complete		= 50015,		// 文件读取结束
	GM_File_Path_Err		= 50017,		// 解析文件路径错误
	GM_File_Write_Err		= 50018,		// 写入文件失败
	GM_Save_Xml_Err			= 50019,		// 保存XML失败
	GM_No_Xml_Node			= 50020,		// 没有XML节点
	GM_No_Command			= 50021,		// 没有命令内容

	GM_Err_Json				= 52004,		// Json解析错误
	GM_Session_Exist		= 52005,		// 会话已经存在
	GM_Session_None			= 52006,		// 会话不存在
	GM_Err_Timeout			= 52008,		// 超时错误
	GM_Thread_Close			= 52010,		// 线程已经退出了
	GM_Push_Fail			= 52011,		// 上传失败

	GM_OverFlow				= 74020,		// 超载了...
	GM_Err_Exception		= 74025,		// 抛出异常
};

typedef DWORD GM_Error;

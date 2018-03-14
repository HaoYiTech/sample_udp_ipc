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
 * VS2010自动生成的对话框类头文件
 ******************************************************/

#pragma once

#include "HyperLink.h"

class CHKUdpThread;
class Csample_udp_ipcDlg : public CDialogEx
{
public:
	Csample_udp_ipcDlg(CWnd* pParent = NULL);	// 标准构造函数
	~Csample_udp_ipcDlg();
	enum { IDD = IDD_SAMPLE_UDP_IPC_DIALOG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
protected:
	HICON			m_hIcon;
	CHyperLink		m_ctrlHome;
	CHKUdpThread *	m_lpHKUdpThread;			// 海康自动搜索线程(不隔几秒发送UDP命令)...
};

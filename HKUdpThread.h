/******************************************************
 * ��һ�Ƽ����ṩ��¼�����Ƽ�ص�ȫƽ̨�޲�����������
 * ֧�ְ���ֱ������㲼�أ��ֲ�ʽ�����洢����̬���ݣ�
 * ֧��΢��ɨ���¼��ȫƽ̨�ʺ�ͳһ������΢��С����
 * ֧�ֶ����������룺����ͷIPC��rtmp��rtsp��MP4�ļ���
 * ֧��ȫʵʱ��ȫ��̬��ȫ��ҳ������ҳǰ��̨����IE8��
 * ֧�ֶ��ն��޲������Ӧ���ţ�flvjs/hls/rtmp�Զ����䣻
 ******************************************************
 * �ٷ���վ => https://myhaoyi.com
 * �������� => http://blog.csdn.net/haoyitech
 * ��Դ���� => https://github.com/HaoYiTech/
 * �Ƽ��Ⱥ => 483663026��QQȺ��
 * ��¼��Ⱥ => 630379661��QQȺ��
 ******************************************************
 * CHKUdpThread => ����(IPC)����������Զ�̽���ࡣ
 * ע�⣺������̻߳������м̳У���������̽�������
 ******************************************************/

#pragma once

#include "OSThread.h"
#include "UDPSocket.h"

#define DEF_MCAST_ADDRV4	"239.255.255.250"	// �鲥��ַ
#define	DEF_MCAST_PORT		37020				// �鲥�˿�
#define	DEF_MCAST_STEP		30					// ���ʱ��(��)
#define DEF_MCAST_TIME		5000				// Ĭ���鲥�¼��̵߳ȴ�ʱ��(����)

class CHKUdpThread : public OSThread
{
public:
	CHKUdpThread();
	~CHKUdpThread();
public:
	GM_Error		InitMulticast();
private:
	virtual void	Entry();
private:
	GM_Error		ForRead();						// ��Ӧ�����ȡ�¼�
	GM_Error		SendCmdQuiry();					// ���Ͳ�ѯָ�� - �鲥����
	GM_Error		WaitAndProcMulticast();			// �ȴ��������鲥����
	GM_Error		ProcessEvent(int eventBits);	// ���������¼�
private:
	UDPSocket		m_UDPSocket;					// UDP����
};

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
 * ���ͷ�ļ��ǿ�Դ�����о����õ��Ĵ���������
 ******************************************************/

#pragma once

enum
{
	GM_NoErr				= 0,			// OK...
	GM_Continue				= 1,			// Continue...
	GM_NoReply				= 2,			// ��Ҫ�ظ�

	GM_Video_Wnd_Exist		= 10001,		// ��Ƶ�����Ѿ�����

	GM_FDFS_Status_Err		= 20001,		// ������״̬����
	GM_FDFS_Data_Err		= 20002,		// ���������ݴ���
	
	GM_DVR_VEnc_Err			= 30001,		// ��Ƶѹ�����ʹ���
	GM_DVR_AEnc_Err			= 30002,		// ��Ƶѹ�����ʹ���
	GM_DVR_VType_Err		= 30003,		// ��Ƶ���ʹ���

	GM_Err_Config			= 40007,		// ���������ļ�ʧ��
	GM_BadInputValue		= 42001,		// ������������
	GM_NetClose				= 42002,		// �����Ѿ��ر�
	GM_NetNoData			= 42003,		// û����������
	GM_DiskErr				= 42004,		// ��ȡ���̴���
	GM_Net_Err				= 42006,		// ͨ��������IPδ���û����粻ͨ������
	GM_None_IPV6			= 42007,		// û�а�װIPV6Э��

	GM_BadInputParam		= 50003,		// ���������Ч
	GM_ProtocolErr			= 50006,		// �޷�֧�ֵ�Э��
	GM_URIErr				= 50007,		// ����URLʱ����
	GM_NotImplement			= 50008,		// û��ʵ�ֵĽӿ�
	GM_Snap_Jpg_Err			= 50009,		// ��ͼʧ��
	GM_File_Del_Err			= 50012,		// ɾ���ļ�ʧ��
	GM_File_Not_Open		= 50013,		// ���ļ�ʧ��
	GM_File_Read_Err		= 50014,		// ��ȡ�ļ�ʧ��
	GM_File_Complete		= 50015,		// �ļ���ȡ����
	GM_File_Path_Err		= 50017,		// �����ļ�·������
	GM_File_Write_Err		= 50018,		// д���ļ�ʧ��
	GM_Save_Xml_Err			= 50019,		// ����XMLʧ��
	GM_No_Xml_Node			= 50020,		// û��XML�ڵ�
	GM_No_Command			= 50021,		// û����������

	GM_Err_Json				= 52004,		// Json��������
	GM_Session_Exist		= 52005,		// �Ự�Ѿ�����
	GM_Session_None			= 52006,		// �Ự������
	GM_Err_Timeout			= 52008,		// ��ʱ����
	GM_Thread_Close			= 52010,		// �߳��Ѿ��˳���
	GM_Push_Fail			= 52011,		// �ϴ�ʧ��

	GM_OverFlow				= 74020,		// ������...
	GM_Err_Exception		= 74025,		// �׳��쳣
};

typedef DWORD GM_Error;

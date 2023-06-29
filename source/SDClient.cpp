//***************************************************************************//
//* 版权所有  www.mediapro.cc
//*
//* 内容摘要：客户端对象类.
//*	
//* 当前版本：V1.0		
//* 作    者：mediapro
//* 完成日期：2018-6-20
//**************************************************************************//
#include "SDClient.h"
#include "SDLog.h"
#include <time.h>



CSDClient::CSDClient(TerminalEncodeParams *ptEncParams, TerminalLogonParams *ptLogonParams, TerminalTransParams *ptTransParams, void* pWindowHandle)
{
	m_bClosed = TRUE;

	memcpy(&m_tEncParams, ptEncParams, sizeof(TerminalEncodeParams));
	memcpy(&m_tLogonParams, ptLogonParams, sizeof(TerminalLogonParams));
	memcpy(&m_tTransParams, ptTransParams, sizeof(TerminalTransParams));

	//SDT-SDK
	m_pTerminal = SDTerminal_New(&m_tEncParams, RemoteVideoYuvData, RemoteAudioPcmData, SystemStatus, this);

	//音频采集渲染3A处理模块
	m_pAudioCapRender = SD3ACapRender_New();

	//视频渲染
	m_pVideoRender = SDVideoRender_New(pWindowHandle, TRUE);
}

CSDClient::~CSDClient()
{
	SDTerminal_Delete(&m_pTerminal);
	SD3ACapRender_Delete(&m_pAudioCapRender);
	SDVideoRender_Delete(&m_pVideoRender);
}


BOOL CSDClient::Start()
{
	if (m_pTerminal == NULL)
	{
		return FALSE;
	}

	//开始音频采集、渲染、3A处理
	//本DEMO使用系统默认的扬声器、麦克风，实际产品中用户可以调用SD3ACapRender_GetAudioInputDeviceList等接口枚举设备供用户选择
	int nCapDeviceID = -1;
	int nRenderDeviceID = -1;
	BOOL bEnableAec = TRUE;
	BOOL bEnableAgc = FALSE;
	BOOL bEnableAns = TRUE;
	BOOL bEnableVad = TRUE;

	BOOL bRet = SD3ACapRender_Start(m_pAudioCapRender, nCapDeviceID, nRenderDeviceID, m_tEncParams.unAudioSampleRate, m_tEncParams.unAudioChannelNum,
									bEnableAec, bEnableAgc, bEnableAns, bEnableVad, Output3AProcessedCaptureDataFunc, this);
	if (bRet == FALSE)
	{
		return FALSE;
	}

	//开始视频渲染
	//播放第0号位置码流
	unsigned int unStreamId = 0;
	bRet = SDVideoRender_AddStream(m_pVideoRender, 0, 0, 0.0, 0.0, 1.0, 1.0);
	if (bRet == FALSE)
	{
		SD3ACapRender_Stop(m_pAudioCapRender);
		return FALSE;
	}
	SDVideoRender_StartRender(m_pVideoRender);
	if (bRet == FALSE)
	{
		SD3ACapRender_Stop(m_pAudioCapRender);
		return FALSE;
	}


	//创建音视频编解码资源并登录服务器
	bRet = SDTerminal_Online(m_pTerminal, &m_tLogonParams, &m_tTransParams);
	if (bRet == FALSE)
	{
		SD3ACapRender_Stop(m_pAudioCapRender);
		SDVideoRender_StopRender(m_pVideoRender);
		return FALSE;
	}

	m_bClosed = FALSE;
	return TRUE;
}


void CSDClient::Stop()
{
	m_bClosed = TRUE;

	if (m_pTerminal)
	{
		SDTerminal_Offline(m_pTerminal);
	}

	if (m_pAudioCapRender)
	{
		SD3ACapRender_Stop(m_pAudioCapRender);
	}

	if (m_pVideoRender)
	{
		SDVideoRender_StopRender(m_pVideoRender);
	}
}


// SDK回调接口实现
//输出接收并解码后的YUV数据
void CSDClient::RemoteVideoYuvData(unsigned char byIndex, unsigned char* data, unsigned int unWidth, unsigned int unHeight, unsigned int unPts, void *pObject)
{
	CSDClient* pClient = (CSDClient*)pObject;
	if (byIndex == 0)
	{
		SDVideoRender_RenderYuv420Frame(pClient->m_pVideoRender, byIndex, data, unWidth, unHeight);
	}
}

//输出接收并解码后的PCM数据
void CSDClient::RemoteAudioPcmData(unsigned char byIndex, unsigned char* data, unsigned int unLen, unsigned int unPts, void *pObject)
{
	CSDClient* pClient = (CSDClient*)pObject;
	SD3ACapRender_Play(pClient->m_pAudioCapRender, data, (int)unLen);
}

//输出系统状态，包括异步登录时的登录结果、在线状态变更通知等，具体见TERMINAL_STATUS_TYPE定义
void CSDClient::SystemStatus(unsigned int unUid, unsigned int unStatus, void *pObject)
{

}

//音频3A回调接口实现
void CSDClient::Output3AProcessedCaptureDataFunc(unsigned char *pucData, int nLen, BOOL bInVoiceStatus, void *pObject)
{
	CSDClient* pClient = (CSDClient*)pObject;	
	SDTerminal_SendAudioData(pClient->m_pTerminal, pucData, nLen);
	//SDLOG_PRINTF("Test", SD_LOG_LEVEL_INFO, "voice capture status:%d", bInVoiceStatus);

}


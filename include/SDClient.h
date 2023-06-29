//***************************************************************************//
//* 版权所有  www.mediapro.cc
//*
//* 内容摘要：客户端对象类.
//*	
//* 当前版本：V1.0		
//* 作    者：mediapro
//* 完成日期：2018-6-20
//**************************************************************************//

#if !defined(_SDCLIENT_H)
#define _SDCLIENT_H

#include "SDCommon.h"
#include "SDTerminalSdk.h"
#include "SDAudioCapRender3ASdk.h"
#include "SDVideoRenderSdk.h"

class CSDClient
{
public:
	CSDClient(TerminalEncodeParams *ptEncParams, TerminalLogonParams *ptLogonParams, TerminalTransParams *ptTransParams, void* pWindowHandle);
	virtual ~CSDClient();

public:
	BOOL Start();
	void Stop();


private:
	// SDK回调接口实现
	//输出接收并解码后的YUV数据
	static void RemoteVideoYuvData(unsigned char byIndex, unsigned char* data, unsigned int unWidth, unsigned int unHeight, unsigned int unPts, void *pObject);

	//输出接收并解码后的PCM数据
	static void RemoteAudioPcmData(unsigned char byIndex, unsigned char* data, unsigned int unLen, unsigned int unPts, void *pObject);

	//输出系统状态，包括异步登录时的登录结果、在线状态变更通知等，具体见TERMINAL_STATUS_TYPE定义
	static void SystemStatus(unsigned int unUid, unsigned int unStatus, void *pObject);

	//音频3A回调接口实现
	static void Output3AProcessedCaptureDataFunc(unsigned char *pucData, int nLen, BOOL bInVoiceStatus, void *pObject);

private:
	TerminalEncodeParams	m_tEncParams;
	TerminalLogonParams		m_tLogonParams;
	TerminalTransParams		m_tTransParams;

	void*					m_pTerminal;
	void*					m_pAudioCapRender;
	void*					m_pVideoRender;
	BOOL					m_bClosed;

};

#endif // !defined(_SDCLIENT_H)

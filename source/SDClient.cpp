//***************************************************************************//
//* ��Ȩ����  www.mediapro.cc
//*
//* ����ժҪ���ͻ��˶�����.
//*	
//* ��ǰ�汾��V1.0		
//* ��    �ߣ�mediapro
//* ������ڣ�2018-6-20
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

	//��Ƶ�ɼ���Ⱦ3A����ģ��
	m_pAudioCapRender = SD3ACapRender_New();

	//��Ƶ��Ⱦ
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

	//��ʼ��Ƶ�ɼ�����Ⱦ��3A����
	//��DEMOʹ��ϵͳĬ�ϵ�����������˷磬ʵ�ʲ�Ʒ���û����Ե���SD3ACapRender_GetAudioInputDeviceList�Ƚӿ�ö���豸���û�ѡ��
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

	//��ʼ��Ƶ��Ⱦ
	//���ŵ�0��λ������
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


	//��������Ƶ�������Դ����¼������
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


// SDK�ص��ӿ�ʵ��
//������ղ�������YUV����
void CSDClient::RemoteVideoYuvData(unsigned char byIndex, unsigned char* data, unsigned int unWidth, unsigned int unHeight, unsigned int unPts, void *pObject)
{
	CSDClient* pClient = (CSDClient*)pObject;
	if (byIndex == 0)
	{
		SDVideoRender_RenderYuv420Frame(pClient->m_pVideoRender, byIndex, data, unWidth, unHeight);
	}
}

//������ղ�������PCM����
void CSDClient::RemoteAudioPcmData(unsigned char byIndex, unsigned char* data, unsigned int unLen, unsigned int unPts, void *pObject)
{
	CSDClient* pClient = (CSDClient*)pObject;
	SD3ACapRender_Play(pClient->m_pAudioCapRender, data, (int)unLen);
}

//���ϵͳ״̬�������첽��¼ʱ�ĵ�¼���������״̬���֪ͨ�ȣ������TERMINAL_STATUS_TYPE����
void CSDClient::SystemStatus(unsigned int unUid, unsigned int unStatus, void *pObject)
{

}

//��Ƶ3A�ص��ӿ�ʵ��
void CSDClient::Output3AProcessedCaptureDataFunc(unsigned char *pucData, int nLen, BOOL bInVoiceStatus, void *pObject)
{
	CSDClient* pClient = (CSDClient*)pObject;	
	SDTerminal_SendAudioData(pClient->m_pTerminal, pucData, nLen);
	//SDLOG_PRINTF("Test", SD_LOG_LEVEL_INFO, "voice capture status:%d", bInVoiceStatus);

}


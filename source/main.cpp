//***************************************************************************//
//* ��Ȩ����  www.mediapro.cc
//*
//* ����ժҪ��SDK������������
//*	
//* ��ǰ�汾��V1.0		
//* ��    �ߣ�mediapro
//* ������ڣ�2020-12-26
//**************************************************************************//

#include "SDCommon.h"
#include "SDLog.h"
#include "SDIniFile.h"
#include "SDClient.h"
#include "SDConsoleIFace.h"
#include "SDTerminalSdk.h"

#include <windows.h>										// We need to include windows.h
#include <stdio.h>											// Include stdio.h for the basics

#define SCREEN_WIDTH  1280									//��ʼ���ڴ�С
#define SCREEN_HEIGHT 720									//

#define SECTION_CONFIG					"Config"
#define KEY_NAME_MEDIA_SERVER_IP		"ServerIp"			//Զ�˷�����IP
#define KEY_NAME_ROOM_ID				"RoomId"			//�����
#define KEY_DOMAIN_ID					"DomainId"			//��������ID	
#define KEY_NAME_UP_POSITION			"UpPosition"		//����λ��
#define KEY_NAME_DOWN_POSITION			"DownPosition"		//����λ��
#define KEY_NAME_JITTER_BUFF_TIME		"JitterBuffTimeMs"	//���ջ���ʱ���ʼֵms

BOOL bFullScreen = FALSE;									// Create a boolean and set it to false.  If we choose full screen, set it to TRUE

///////////////////////////////// CHANGE TO FULL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This changes the screen to FULL SCREEN
/////
///////////////////////////////// CHANGE TO FULL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void ChangeToFullScreen(int width, int height)
{
	DEVMODE dmSettings;									// Device Mode variable - Needed to change modes

	memset(&dmSettings,0,sizeof(dmSettings));			// Makes Sure Memory's Cleared

	// Get the current display settings.  This function fills our the settings.
	if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dmSettings))
	{
		// Display error message if we couldn't get display settings
		MessageBox(NULL, "Could Not Enum Display Settings", "Error", MB_OK);
		return;
	}

	dmSettings.dmPelsWidth	= width;					// Set the desired Screen Width
	dmSettings.dmPelsHeight	= height;					// Set the desired Screen Height
	dmSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;	// Set the flags saying we're changing the Screen Width and Height
	
	// This function actually changes the screen to full screen
	// CDS_FULLSCREEN Gets Rid Of Start Bar.
	// We always want to get a result from this function to check if we failed
	int result = ChangeDisplaySettings(&dmSettings,CDS_FULLSCREEN);	

	// Check if we didn't receive a good return message From the function
	if(result != DISP_CHANGE_SUCCESSFUL)
	{
		// Display the error message and quit the program
		MessageBox(NULL, "Display Mode Not Compatible", "Error", MB_OK);
		PostQuitMessage(0);
	}
}

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);		// Create a function prototype for the wndProc.
															// Here is our "main()" equivalent in windows, WinMain().	
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)			
{														
    HWND        hwnd;										// Create a variable that holds a handle to our window.
    MSG         msg;										// Create a variable to hold the message information
    WNDCLASSEX  wndclass;									// This variable will hold all the information about the window (The name, icon, cursor, color, menu bar...)
	DWORD dwStyle;

    wndclass.cbSize        = sizeof (wndclass);				// Here we set the size of the wndclass. 
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;		// The style we want is Vertical-Redraw and Horizontal-Redraw
    wndclass.lpfnWndProc   = WndProc;						// Here is where we assing our CALLBACK function. (The function to handle the messages)
    wndclass.cbClsExtra    = 0;								// We want zero extra bytes
    wndclass.cbWndExtra    = 0;								// Init this useless thing to 0
    wndclass.hInstance     = hInstance;						// We assign our hInstance to our window.  
    wndclass.hIcon         = LoadIcon (NULL, IDI_WINLOGO);	// Load a logo for our window
    wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW);	// Load a arrow cursor for our window.
															// Set the background color for the window
    wndclass.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wndclass.lpszMenuName  = NULL;							// We pass NULL because we don't want a menu.
    wndclass.lpszClassName = "Remote Video Play";			// Create a name that we identify this window class with.
    wndclass.hIconSm       = LoadIcon (NULL, IDI_WINLOGO);	// Create an icon for the top left of the window.

	RegisterClassEx (&wndclass);							// We need to register the wndclass.
															

	if(MessageBox(NULL, "�Ƿ�ȫ������(Esc�˳�)��", "Options", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		dwStyle = WS_OVERLAPPEDWINDOW;						// If we don't want full screen, open a simple window
	} 
	else													// If we chose YES
	{			
		bFullScreen = TRUE;									// Set our boolean to TRUE, we wanted full screen

															// This is the style that we need our window to have in order to be windowless full screen
		dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		ChangeToFullScreen(SCREEN_WIDTH, SCREEN_HEIGHT);	// This changes our screen to full screen with our desired resolution
	}

															// Here we create the window returning our handle to the window.
    hwnd = CreateWindow ("Remote Video Play",				// window class name 
						 "Remote Video Play",		  		// window's Title    
						 dwStyle,							// window style		 
						 0,									// initial x position
						 0,									// initial y position
						 SCREEN_WIDTH,						// initial x size - Our resolution width
						 SCREEN_HEIGHT,					    // initial y size - Our resolution height	 
						 NULL,								// Pass NULL for the parent window
						 NULL,								// Pass NULL for a menu
						 hInstance,						    // Pass in our hInstance
						 NULL);								// Pass NULL to the wndProc, we don't want to pass it any variables address's.

    ShowWindow (hwnd, iCmdShow);							// Show the window
    UpdateWindow (hwnd);									// Update our window and paint it to the screen
							
							
	//********************************************************************************************	
	//��ʼ������
	//********************************************************************************************	
	char strIniFileName[MAX_PATH];
	GetSameExeFile(strIniFileName, ".ini");
	SDLOG_INIT("./log", SD_LOG_LEVEL_INFO, strIniFileName);

	//�ͻ���SDK������ʼ��
	SDTerminal_Enviroment_Init("./log", TERMINAL_LOG_OUTPUT_LEVEL_INFO);
	SDVideoRender_Enviroment_Init("./log", VIDEO_RENDER_LOG_LEVEL_INFO);
	SD3ACapRender_Enviroment_Init("./log", CAP_RENDER_3A_LOG_LEVEL_INFO);

	//********************************************************************************************	
	//��ȡ����
	//********************************************************************************************	
	CSDIniFile *pIniFile = new CSDIniFile;
	pIniFile->ReadIniFile(strIniFileName);

	//������IP
	char strServerIp[64];
	memset(strServerIp, 0x0, sizeof(strServerIp));
	pIniFile->SDGetProfileString(SECTION_CONFIG, KEY_NAME_MEDIA_SERVER_IP, strServerIp, 64);

	//��������ID
	UINT unDomainId = (UINT)pIniFile->SDGetProfileInt(SECTION_CONFIG, KEY_DOMAIN_ID, 3);

	//�����
	UINT unRoomId = pIniFile->SDGetProfileInt(SECTION_CONFIG, KEY_NAME_ROOM_ID, 1);

	//����λ��
	BYTE bySendPosition = (BYTE)pIniFile->SDGetProfileInt(SECTION_CONFIG, KEY_NAME_UP_POSITION, 2);

	//����λ��
	BYTE byRecvPosition = (BYTE)pIniFile->SDGetProfileInt(SECTION_CONFIG, KEY_NAME_DOWN_POSITION, 0);

	//�û�ID����DEMOʹ�����ֵ
	UINT unSec = 0;
	UINT unUsec = 0;
	SD_GetCurrentTime(unSec, unUsec);
	UINT unSeed = unSec^unUsec;
	SD_srandom(unSeed);
	UINT unUserId = SD_random32();

	//���ջ���ʱ��
	UINT unJitterBuffTime = pIniFile->SDGetProfileInt(SECTION_CONFIG, KEY_NAME_JITTER_BUFF_TIME, 200);

	delete pIniFile;


	//********************************************************************************************	
	//������Ч�Լ�У��
	//********************************************************************************************	
	if (strServerIp[0] == 0)
	{
		SDLOG_PRINTF("Test", SD_LOG_LEVEL_ERROR, "Please setup right server ip!");
		SDLOG_CLOSE();
		SDTerminal_Enviroment_Free();
		SDVideoRender_Enviroment_Free();
		SD3ACapRender_Enviroment_Free();
		return FALSE;
	}

	SDLOG_PRINTF("Test", SD_LOG_LEVEL_INFO, "ServerIp:%s UserId:%u  RoomId:%u  JitterBuff:%d!!", 
				strServerIp, unUserId, unRoomId, unJitterBuffTime);



	TerminalEncodeParams tEncodeParams;
	memset(&tEncodeParams, 0x0, sizeof(TerminalEncodeParams));
	//��ҵ�񳡾�������Ϊ2ͨ��
	tEncodeParams.unAudioChannelNum = 2;
	//��ҵ�񳡾�������Ϊ44100
	tEncodeParams.unAudioSampleRate = 44100;
	//��ҵ�񳡾��½�������Ƶ��SDK�ڲ��������Ƶ������ز�����ʹ������Ĭ��ֵ
	tEncodeParams.unVideoBitrateKbps = 4000;
	tEncodeParams.unVideoFramerate = 25;
	tEncodeParams.unVideoIdrIntervalSecond = 3;
	tEncodeParams.unVideoQualityLevelFrom0To5 = 2;
	tEncodeParams.unVideoTransHeight = 1080;
	tEncodeParams.unVideoTransWidth = 1920;
	tEncodeParams.bEnableHardwareEnc = FALSE;

	TerminalLogonParams tLogonParams;
	memset(&tLogonParams, 0x0, sizeof(TerminalLogonParams));
	//��ҵ�񳡾������
	tLogonParams.unDomainId = unDomainId;
	//��ҵ�񳡾��¸���ʵ��������÷���ţ���������Ϊ2
	tLogonParams.unRoomId = unRoomId;
	//��ҵ�񳡾��¸���ʵ����������û�ID����������Ϊ1000���豣֤Ψһ��
	tLogonParams.unUserId = unUserId;
	//��ҵ�񳡾�������Ϊ�շ�һ������
	tLogonParams.unUserType = TERMINAL_USER_TYPE_SEND_RECV;
	//��ҵ�񳡾�������Ϊ�첽��¼ģʽ�������û���UI�߳��е���Online�ӿ�����UI
	tLogonParams.bAsyncLogonMode = TRUE;
	//����λ�ã�ҵ�����о��ߣ��������ͻ��˳�ͻ
	tLogonParams.unAvUpIndex = bySendPosition;

	tLogonParams.unAudioDownMask = 0x1 << (byRecvPosition);
	tLogonParams.unVideoDownMask = 0x1 << (byRecvPosition);
	strcpy(tLogonParams.acMediaServerIP, strServerIp);

	TerminalTransParams tTransParams;
	memset(&tTransParams, 0x0, sizeof(TerminalTransParams));
	//��ҵ�񳡾��¿���NACK
	tTransParams.bEnableNack = TRUE;
	//��ҵ�񳡾��¿��������������
	tTransParams.bFreezeFrameWhenLost = TRUE;
	//��ҵ�񳡾��¿�����������Ӧ����
	tTransParams.unAutoBitrateMethod = TERMINAL_AUTO_BITRATE_ADJUST_BITRATE_FIRST;
	//��ҵ�񳡾���ʹ������FEC GROUP����
	tTransParams.unFecMaxGroupSize = 64;
	tTransParams.unFecMinGroupSize = 28;
	tTransParams.unFecRedunMethod = TERMINAL_FEC_TYPE_AUTO;
	tTransParams.unFecRedunRatio = 30;
	tTransParams.unJitterBuffDelay = unJitterBuffTime;

	CSDClient client(&tEncodeParams, &tLogonParams, &tTransParams, (void*)hwnd);

	client.Start();
	
	while(1)												// Loop forever
	{	
															
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))		// Get any messages the OS has for us
		{
			
			if(msg.message == WM_QUIT)						// If the message says to quit, quit!
				break;

			TranslateMessage(&msg);							// Translate from virtual keys to character messages						
			DispatchMessage(&msg);							// Send the message to the WndProc
		}		

		SD_Sleep(3);		
	}

	UnregisterClass("Full Screen App",hInstance);			// We need to unregister the wndclass.
    
	client.Stop();
	
	SDLOG_PRINTF("Test", SD_LOG_LEVEL_INFO, "sdk test over success...");
	SDLOG_CLOSE();
	SDTerminal_Enviroment_Free();
	SDVideoRender_Enviroment_Free();
	SD3ACapRender_Enviroment_Free();
	return msg.wParam ;										// Quit the program
}
															// Start our window procedure (wndProc).  This handles the messages
LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{	
	switch (iMsg)											// This checks what the message is.  Below is some of the message that windows might return.  There of course is HUNDREDS.
    {
		case WM_KEYDOWN:									// If we pressed a key
			switch (wParam) 								// Check what the wParam holds.  The wParam holds which key we pressed.
			{		
				case VK_ESCAPE:								// If we pressed the ESCAPE key.
					if(bFullScreen)							// If we went to full screen mode
					{										// Calling the same function with NULL and 0 reset the screen settings and resolution
						ChangeDisplaySettings(NULL, 0);
					}
					SendMessage(hwnd, WM_CLOSE, 0, 0);		// Close the program
					break;
			}
			break;

		case WM_DESTROY:									// This message is sent when the user closes the window.
			PostQuitMessage(0);								// Post a WM_QUIT to our message loop to quit the application
			break;											// Break from the switch statement
	}													

	return DefWindowProc (hwnd, iMsg, wParam, lParam);		// Return generic information to our message loop
}															// End of the WndProc


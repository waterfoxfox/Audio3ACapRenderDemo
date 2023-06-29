//***************************************************************************//
//* 版权所有  www.mediapro.cc
//*
//* 内容摘要：windows SDT SDK，内置音视频编解码
//*			  
//*
//*	说    明：
//*			  
//*				
//* 当前版本：V1.0		
//* 作    者：mediapro
//* 完成日期：2020-8-6
//**************************************************************************//

#ifndef _SD_MEDIAPRO_TERMINAL_SDK_H_
#define _SD_MEDIAPRO_TERMINAL_SDK_H_


#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN32 || defined __CYGWIN__
#ifdef DLL_EXPORTS
#ifdef __GNUC__
#define DLLIMPORT_SDT_SDK __attribute__ ((dllexport))
#else
#define DLLIMPORT_SDT_SDK __declspec(dllexport) 
#endif
#else
#ifdef __GNUC__
#define DLLIMPORT_SDT_SDK 
#else
#define DLLIMPORT_SDT_SDK 
#endif
#endif
#else
#if __GNUC__ >= 4
#define DLLIMPORT_SDT_SDK __attribute__ ((visibility ("default")))
#else
#define DLLIMPORT_SDT_SDK
#endif
#endif

#ifdef WIN32
	typedef signed __int64       int64_t;
#endif

#ifdef __APPLE__
#ifndef OBJC_BOOL_DEFINED
	typedef int BOOL;
#endif 
#else
#ifndef BOOL
	typedef int BOOL;
#endif
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

//同一房间最大码流数量，即位置数
#define TERMINAL_MAX_STREAMS_PER_ROOM	12


//FEC GROUP的最小值、最大值
#define TERMINAL_FEC_GROUP_MIN			8
#define TERMINAL_FEC_GROUP_MAX			72


//FEC 固定冗余度时可选冗余度的最小值、最大值
#define TERMINAL_FEC_REDUN_RATIO_MIN	0
#define TERMINAL_FEC_REDUN_RATIO_MAX	100	

//最大Jitter Buff缓存时长,ms
#define TERMINAL_JITTER_BUFF_MAX		600

//编码码率最小值、最大值,kbps
#define TERMINAL_ENC_BITRATE_MIN		50
#define TERMINAL_ENC_BITRATE_MAX		8000

//编码帧率最小值、最大值
#define TERMINAL_ENC_FRAMERATE_MIN		1
#define TERMINAL_ENC_FRAMERATE_MAX		30

//编码分辨率最小值、最大值
#define	TERMINAL_ENC_WIDTH_MIN		    176
#define	TERMINAL_ENC_HEIGHT_MIN			144
#define	TERMINAL_ENC_WIDTH_MAX		    3840
#define	TERMINAL_ENC_HEIGHT_MAX			2160

//IDR间隔的最小值、最大值
#define	TERMINAL_ENC_IDR_INTERVAL_MIN	1
#define	TERMINAL_ENC_IDR_INTERVAL_MAX	10

//音频采样率支持的最小值、最大值
#define	TERMINAL_SAMPLE_RATE_MIN		8000
#define	TERMINAL_SAMPLE_RATE_MAX		48000

//音频声道数支持的最小值、最大值
#define	TERMINAL_AUDIO_CHANNELS_MIN		1
#define	TERMINAL_AUDIO_CHANNELS_MAX		2



//日志输出的级别
typedef enum TERMINAL_LOG_OUTPUT_LEVEL
{
	TERMINAL_LOG_OUTPUT_LEVEL_DEBUG		= 1,
	TERMINAL_LOG_OUTPUT_LEVEL_INFO		= 2,
	TERMINAL_LOG_OUTPUT_LEVEL_WARNING	= 3,	
	TERMINAL_LOG_OUTPUT_LEVEL_ERROR		= 4,
	TERMINAL_LOG_OUTPUT_LEVEL_ALARM		= 5,
	TERMINAL_LOG_OUTPUT_LEVEL_FATAL		= 6,
	TERMINAL_LOG_OUTPUT_LEVEL_NONE		= 7
} TERMINAL_LOG_OUTPUT_LEVEL;

//用户类型
typedef enum TERMINAL_USER_TYPE
{
	//未定义，保留
	TERMINAL_USER_TYPE_UNDEFINED		= 0,
	//收发一体
	TERMINAL_USER_TYPE_SEND_RECV		= 1,
	//纯接收方
	TERMINAL_USER_TYPE_RECV_ONLY		= 2,
	//纯发送方
	TERMINAL_USER_TYPE_SEND_ONLY		= 3
} TERMINAL_USER_TYPE;


//系统对外通知消息
typedef enum TERMINAL_STATUS_TYPE
{
	//保留未使用
	TERMINAL_STATUS_EXIT_NORMAL			= 0,
	//保留未使用
	TERMINAL_STATUS_EXIT_ABNORMAL		= 1,
	//保留未使用
	TERMINAL_STATUS_EXIT_LOSTCONNECT	= 2,
	//账号在其他位置登录，被迫下线
	TERMINAL_STATUS_EXIT_KICKED			= 3,
	//登录成功[异步登录时使用]
	TERMINAL_STATUS_ONLINE_SUCCESS		= 4,
	//登录失败[异步登录时使用]
	TERMINAL_STATUS_ONLINE_FAILED		= 5,
	//客户端TCP掉线，内部开始自动重连，仅用于提示
	TERMINAL_STATUS_RECON_START			= 6,
	//内部自动重连成功,恢复TCP连接，仅用于提示
	TERMINAL_STATUS_RECON_SUCCESS		= 7,
	//因其他客户端抢占位置，本客户端的上发流将被丢弃，仅用于提示
	TERMINAL_STATUS_MYPOSITION_PAUSE	= 10,
	//因其他客户端离开位置，本客户端的上发流将被恢复，仅用于提示
	TERMINAL_STATUS_MYPOSITION_RESUME	= 11,
}TERMINAL_STATUS_TYPE;


//FEC 冗余方法
typedef enum TERMINAL_FEC_METHOD_TYPE
{
	//自动冗余度
	TERMINAL_FEC_TYPE_AUTO				= 0,
	//固定冗余度
	TERMINAL_FEC_TYPE_FIX				= 1,
} TERMINAL_FEC_METHOD_TYPE;


//视频编码图像质量档次，对应X264 X265 preset
typedef enum TERMINAL_ENCODE_QUALITY
{
	//"ultrafast" - baseline
	TERMINAL_ENC_QUALITY_0				= 0,
	//"superfast" - baseline
	TERMINAL_ENC_QUALITY_1				= 1,
	//"veryfast"  - baseline
	TERMINAL_ENC_QUALITY_2				= 2,
	//"faster"    - baseline
	TERMINAL_ENC_QUALITY_3				= 3,
	//"fast"      - baseline
	TERMINAL_ENC_QUALITY_4				= 4,
	//"medium" - high profile
	TERMINAL_ENC_QUALITY_5				= 5,
} TERMINAL_ENCODE_QUALITY;


//输入视频格式
typedef enum TERMINAL_VIDEO_INPUT_FORMAT
{
	TERMINAL_VIDEO_FORMAT_INVALID = -1, 
	TERMINAL_VIDEO_FORMAT_YUV420P,       
	TERMINAL_VIDEO_FORMAT_NV12,             
	TERMINAL_VIDEO_FORMAT_NV21,        
	TERMINAL_VIDEO_FORMAT_RGB24,
	TERMINAL_VIDEO_FORMAT_RGBA32
} TERMINAL_VIDEO_INPUT_FORMAT;


typedef enum TERMINAL_AUTO_BITRATE_METHOD
{
	//关闭码率自适应
	TERMINAL_AUTO_BITRATE_ADJUST_DISABLE = 0,
	//优先降低帧率，其次降低码率
	TERMINAL_AUTO_BITRATE_ADJUST_FRAME_FIRST = 1,
	//优先降低码率，其次降低帧率
	TERMINAL_AUTO_BITRATE_ADJUST_BITRATE_FIRST = 2
} TERMINAL_AUTO_BITRATE_METHOD;


//音视频编码相关参数
typedef struct TerminalEncodeParams
{
	//编码宽度
	unsigned int unVideoTransWidth;
	//编码高度
	unsigned int unVideoTransHeight;
	//编码帧率
	unsigned int unVideoFramerate;
	//编码码率
	unsigned int unVideoBitrateKbps;
	//编码质量，越大计算复杂度越高，画质相对更好，取值见：TERMINAL_ENCODE_QUALITY
	unsigned int unVideoQualityLevelFrom0To5;
	//编码IDR帧间隔秒数
	unsigned int unVideoIdrIntervalSecond;
	//是否启用硬编码，硬编码不支持则自动切回软编码
	unsigned int bEnableHardwareEnc;
	//音频编码发送、解码输出的采样率
	unsigned int unAudioSampleRate;
	//音频编码发送、解码输出的声道数
	unsigned int unAudioChannelNum;
}TerminalEncodeParams;


//登录相关参数
typedef struct TerminalLogonParams
{
	//用户ID，外层自行保证唯一的非0正整数
	unsigned int unUserId;
	//用户类型，取值见：TERMINAL_USER_TYPE
	unsigned int unUserType;
	//房间ID，非0正整数
	unsigned int unRoomId;
	//域ID，目前需指定为1
	unsigned int unDomainId;
	//音视频发送的目的位置。设置为254时，登录时将不会进行上位操作，用户可在登录成功后自行调用SDTerminal_OnPosition执行上位操作。
	//设置成255时，登录时将由系统分配当前可用的位置。
	//设置成小于TERMINAL_MAX_STREAMS_PER_ROOM时，登录时将上位到该指定位置[抢占式]。
	//对于纯接收类客户端TERMINAL_USER_TYPE_RECV_ONLY，将忽略本值，不会执行上位操作。
	unsigned int unAvUpIndex;
	//音频接收掩码，对应bit为1表示接收该位置数据
	unsigned int unAudioDownMask;
	//视频接收掩码，对应bit为1表示接收该位置数据	
	unsigned int unVideoDownMask;
	//是否异步方式登录，异步时SDTerminal_Online即刻返回，登录结果通过系统状态回调输出
	unsigned int bAsyncLogonMode;
	//媒体服务器IP地址
	char acMediaServerIP[128];
}TerminalLogonParams;


//传输相关参数
typedef struct TerminalTransParams
{
	//发送使用的FEC方法，取值见：TERMINAL_FEC_METHOD_TYPE
	unsigned int unFecRedunMethod;
	//发送使用的FEC冗余度
	unsigned int unFecRedunRatio;
	//发送使用的FEC min group size
	unsigned int unFecMinGroupSize;
	//发送使用的FEC max group size
	unsigned int unFecMaxGroupSize;
	//是否启用NACK功能
	unsigned int bEnableNack;
	//接收JitterBuff缓存时间，单位毫秒，设置为0时可获得最低延时
	unsigned int unJitterBuffDelay;
	//是否启用接收丢包冻结机制，启用后丢包可冻结画面避免用户感知花屏
	unsigned int bFreezeFrameWhenLost;
	//发送采用的码率自适应方法，取值见：TERMINAL_AUTO_BITRATE_METHOD
	unsigned int unAutoBitrateMethod;
	//是否在码率自适应时启用分辨率自适应
	unsigned int bEnableAutoResolution;
	//是否在码率自适应时启用编码器长期参考帧机制
	unsigned int bEnableLongTimeRef;
}TerminalTransParams;

//	回调函数【注意事项】
//	1、通知型回调函数应尽可能快的退出，不进行耗时、阻塞操作。
//  2、数据型回调函数中允许进行解码处理

//输出接收并解码后的YUV数据
typedef void (*RemoteVideoYuvDataCallback)(unsigned char byIndex, unsigned char* data, unsigned int unWidth, unsigned int unHeight, unsigned int unPts, void *pObject);

//输出接收并解码后的PCM数据
typedef void (*RemoteAudioPcmDataCallback)(unsigned char byIndex, unsigned char* data, unsigned int unLen, unsigned int unPts, void *pObject);

//输出系统状态，包括异步登录时的登录结果、在线状态变更通知等，具体见TERMINAL_STATUS_TYPE定义
typedef void (*SystemStatusCallback)(unsigned int unUid, unsigned int unStatus, void *pObject);


//////////////////////////////////////////////////////////////////////////
// 客户端类封装接口

/***
* 环境初始化，系统只需调用一次，主要用于日志模块的初始化
* @param: outputPath表示日志存放路径，支持相对路径和绝对路径，若目录不存在将自动创建
* @param: outputLevel表示日志输出的级别，只有等于或者高于该级别的日志输出到文件，取值范围参考TERMINAL_LOG_OUTPUT_LEVEL
* @return: 
*/
DLLIMPORT_SDT_SDK void  SDTerminal_Enviroment_Init(const char* outputPath, int outputLevel);

DLLIMPORT_SDT_SDK void  SDTerminal_Enviroment_Free();


/***
* 创建SDTerminal
* @param ptEncodeParams: 编码相关参数
* @param pfOutputVideo: 解码后视频输出回调接口
* @param pfOutputAudio: 解码后音频输出回调接口
* @param pfSysStatus: 系统状态输出回调接口
* @param pObject: 上述输出回调接口的透传指针，将通过回调函数形参方式透传外层
* @return: 返回模块指针，为NULL则失败
*/
DLLIMPORT_SDT_SDK void*  SDTerminal_New(TerminalEncodeParams* ptEncodeParams, RemoteVideoYuvDataCallback pfOutputVideo, 
								RemoteAudioPcmDataCallback pfOutputAudio, SystemStatusCallback pfSysStatus, void* pObject);


/***
* 从服务器下线并销毁SDTerminal，使用者应该做好与其他API之间的互斥保护
* 互斥保护方法1：业务保证其他API接口停止调用后，才会调用本接口
* 互斥保护方法2：创建两把锁，分别用于音频发送和视频发送等API与本接口的互斥
*				 一定要对音视频分别使用不同的锁，这样才不会变成二者串行
* @param ppTerminal: 模块指针指针
* @return:
*/
DLLIMPORT_SDT_SDK void  SDTerminal_Delete(void** ppTerminal);


/***
* 准备推送和接收音视频环境
* @param pTerminal: 模块指针
* @param ptLogonParams: 登录相关参数
* @param ptTransParams: 传输相关参数
* @return: 接口调用是否成功
*/
DLLIMPORT_SDT_SDK BOOL  SDTerminal_Online(void* pTerminal, TerminalLogonParams* ptLogonParams, TerminalTransParams* ptTransParams);


/***
* 停止推送和接收音视频
* @param pTerminal: 模块指针
* @return:
*/
DLLIMPORT_SDT_SDK void  SDTerminal_Offline(void* pTerminal);


/***
* 编码发送音频
* @param pTerminal: 模块指针
* @param buf: 输入音频PCM存放区
* @param unLen: 输入音频PCM字节数
* @return:
*/
DLLIMPORT_SDT_SDK void  SDTerminal_SendAudioData(void* pTerminal, unsigned char* buf, unsigned int unLen);


/***
* 编码发送视频
* @param pTerminal: 模块指针
* @param buf: 待编码发送的视频数据存放区
* @param unLen: 视频数据存放区字节数
* @param unWidth: 视频宽度
* @param unHeight: 视频高度
* @param eFormat: 视频格式
* @return:
*/
DLLIMPORT_SDT_SDK void  SDTerminal_SendVideoData(void* pTerminal, unsigned char* buf, unsigned int unWidth, unsigned int unHeight, TERMINAL_VIDEO_INPUT_FORMAT eFormat);



/***
* 获得硬编码支持情况,Online之后调用生效
* @param pTerminal: 模块指针
* @param pbWillUseHwEnc: 返回当前是否采用硬编码
* @param peHwRecomInputFormat: 若采用硬编码，硬编码推荐的输入格式
* @return: 接口调用是否成功
*/
DLLIMPORT_SDT_SDK BOOL  SDTerminal_GetHardwareEncInfo(void* pTerminal, BOOL* pbWillUseHwEnc, TERMINAL_VIDEO_INPUT_FORMAT *peHwRecomInputFormat);


/***
* Online接口后获取AV的上传位置，若不在位置上返回255
* @param pTerminal: 模块指针
* @return: 当前时刻的位置
*/
DLLIMPORT_SDT_SDK unsigned int SDTerminal_GetUpPosition(void* pTerminal);


/***
* 登录时未进行上位操作(unAvUpIndex设置为254)，后续可根据业务需要调用本API上位。
* 调用本API要求处于登录成功状态。上位成功后，服务器将停止下发该位置流给本客户端(自己无需看自己)
* @param pTerminal: 模块指针
* @param punAvUpIndex: 上位位置，小于TERMINAL_MAX_STREAMS_PER_ROOM或等于255(由服务器分配当前可用位置，若分配成功将通过本变量返回分配的位置)
* @return: 接口调用是否成功
*/
DLLIMPORT_SDT_SDK BOOL  SDTerminal_OnPosition(void* pTerminal, unsigned int* punAvUpIndex);


/***
* 从位置下位
* @param pTerminal: 模块指针
* @return:
*/
DLLIMPORT_SDT_SDK void  SDTerminal_OffPosition(void* pTerminal);


/***
* 登录后也可以通过本API动态调整接收MASK，调用本API要求处于登录成功状态。
* @param pTerminal: 模块指针
* @param unAudioMask: 音频接收掩码，对应bit为1表示接收该位置数据
* @param unVideoMask: 视频接收掩码，对应bit为1表示接收该位置数据
* @return: 接口调用是否成功
*/
DLLIMPORT_SDT_SDK BOOL  SDTerminal_SetAvDownMasks (void* pTerminal, unsigned int unAudioMask, unsigned int unVideoMask);


/***
* 获取当前接收MASK设置，调用本API要求处于登录成功状态。
* @param pTerminal: 模块指针
* @param punAudioMask: 存放获取到的当前音频接收掩码
* @param punVideoMask: 存放获取到的当前视频接收掩码
* @return: 接口调用是否成功
*/
DLLIMPORT_SDT_SDK BOOL  SDTerminal_GetAvDownMasks (void* pTerminal, unsigned int *punAudioMask, unsigned int *punVideoMask);


/***
* 获取SDK版本信息
* @param pTerminal: 模块指针
* @return: SDK版本号
*/
DLLIMPORT_SDT_SDK unsigned int  SDTerminal_GetVersion(void* pTerminal);



/***
* 启动上行码流的TS文件录制,Online之后调用生效
* @param pTerminal: 模块指针
* @param strTsFileUrl: TS文件URL
* @return: 接口调用是否成功
*/
DLLIMPORT_SDT_SDK BOOL  SDTerminal_StartCapUpStream(void* pTerminal, const char* strTsFileUrl);


/***
* 停止上行码流的TS文件录制
* @param pTerminal: 模块指针
* @return: 接口调用是否成功
*/
DLLIMPORT_SDT_SDK void  SDTerminal_StopCapUpStream(void* pTerminal);


/***
* 启动下行码流的TS文件录制,Online之后调用生效
* @param pTerminal: 模块指针
* @param unAvDownIndex: 需要录制的下行码流位置
* @param strTsFileUrl: TS文件URL
* @return: 接口调用是否成功
*/
DLLIMPORT_SDT_SDK BOOL  SDTerminal_StartCapDownStream(void* pTerminal, unsigned int unAvDownIndex, const char* strTsFileUrl);


/***
* 停止下行码流的TS文件录制
* @param pTerminal: 模块指针
* @param unAvDownIndex: 需要停止录制的下行码流位置
* @return: 
*/
DLLIMPORT_SDT_SDK void  SDTerminal_StopCapDownStream(void* pTerminal, unsigned int unAvDownIndex);



/***
* 启动下行码流的RAW文件录制,Online之后调用生效
* @param pTerminal: 模块指针
* @param unAvDownIndex: 需要录制的下行码流位置
* @param strRawFileUrl: RAW文件URL
* @return: 接口调用是否成功
*/
DLLIMPORT_SDT_SDK BOOL  SDTerminal_StartCapDownRawStream(void* pTerminal, unsigned int unAvDownIndex, const char* strRawFileUrl);


/***
* 停止下行码流的RAW文件录制
* @param pTerminal: 模块指针
* @param unAvDownIndex: 需要停止录制的下行码流位置
* @return: 
*/
DLLIMPORT_SDT_SDK void  SDTerminal_StopCapDownRawStream(void* pTerminal, unsigned int unAvDownIndex);


/***
* 获取当前时刻音视频上下行丢包率情况，内部已经乘100.0转换为百分比
* @param pTerminal: 模块指针
* @param pfVideoUpLostRatio: 视频上行丢包率
* @param pfVideoDownLostRatio: 视频下行丢包率
* @param pfAudioUpLostRatio: 音频上行丢包率
* @param pfAudioDownLostRatio: 音频下行丢包率
* @return: 
*/
DLLIMPORT_SDT_SDK void  SDTerminal_GetVideoAudioUpDownLostRatio(void* pTerminal, float *pfVideoUpLostRatio, float *pfVideoDownLostRatio, 
														float *pfAudioUpLostRatio, float *pfAudioDownLostRatio);


/***
* 获取当前时刻RTT
* @param pTerminal: 模块指针
* @return: 当前时刻的RTT值，单位毫秒
*/
DLLIMPORT_SDT_SDK unsigned int  SDTerminal_GetCurrentRtt(void* pTerminal);



#ifdef __cplusplus
}
#endif

#endif // _SD_MEDIAPRO_TERMINAL_SDK_H_

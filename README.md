# DEMO说明
本DEMO用于演示音频采集渲染3A处理一体库的使用，DEMO使用SDT协议接入SDT媒体服务器，接收来自远端的音视频，同时发出本地经过3A处理后的音频。<br>

可通过DEMO配置文件设置服务器IP、房间号、上下行位置等，对于SDT协议相关疑问可以查看相关文档或咨询技术。


```js
[Config]
;流媒体服务器IP
ServerIp=47.106.195.225
;流媒体服务器域号
DomainId=3
;房间号
RoomId=777
;本端上行音频到房间中的位置
UpPosition=2
;本端接收房间中指定位置的音视频
DownPosition=0
;播放JitterBuff初始值ms
JitterBuffTimeMs=200
```
<br>

SDK API的调用集中在SDClient.cpp中
<br>


<br>
测试工程使用VS2010或更高版本编译



---

# 相关资源
跟多文档、代码资源见：https://mediapro.apifox.cn

SDK 商用及定制化、技术支持服务可联系：[http://www.mediapro.cc/](http://www.mediapro.cc/)


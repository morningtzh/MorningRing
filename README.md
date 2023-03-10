# MorningRing

## 背景
Moodmoon实在是太好看了，我非常喜欢光影效果，当发现他可以如此简介的进行视觉表达的时候就非常想拥有一个。
目前拥有的方式是自己做。他的实现实在是太简单了，以至于Bilibili上有很多复刻的视频，属于有手就行的项目了。

它由两部分组成，分为圆形光圈和方形遮罩两部分。圆形光圈使用内外两条灯带，能有更细腻的效果；方形遮罩采用带纹理的纸张或布，作为柔光罩。

[满月灯渲染视频](https://www.bilibili.com/video/BV1z8411E7oL)

![满月灯渲染图](https://i.imgur.com/AWUCLpw.png)

## 系统设计

### 实体部分

- ESP32开发板
- 铝合金圆环
   - 尺寸：直径40cm
   - 以下所有数据请根据圆环大小进行计算
- WS2812灯带
   - 选型：144颗灯珠/米的裸板灯带，购买3m
   - 灯珠密度请根据实际尺寸及预算选择，价格差距较大
   - 尺寸计算公式：
     $圆环直径 \cdot Pi \cdot 每米灯珠个数 \cdot 2（内外两条）+ 手工误差$ 
   - 灯珠最大功率约为0.3w，会有电压衰减, 不足没关系，末端会变暗
   - [WS2812B 5050数据整理](https://blog.csdn.net/weixin_39591031/article/details/110437574)
   - [WS2812 Datasheet](https://cdn-shop.adafruit.com/datasheets/WS2812.pdf)
- 开关电源
   - 选型：100w 5V
   - 选型公式：
     $总灯珠数 \cdot 0.3w \cdot 0.7（你能接受的变暗比例，不能太小）$
- 带纹理的纸张或布
   - 尺寸：
     $64.7cm \cdot 64.7cm$
   - 计算公式：
     $圆环直径 \div 你觉得合适的比例（比如0.618）$

## 软件部分

软件分为上位机和单片机系统。

上位机由Unity3D制作，包含参数配置 / 灯光模拟 / 参数下发 / OTA升级功能，单片机系统主要是灯光控制，易用性方面需要增加wifi链接 / 蓝牙连接 / OTA升级功能。另外经历足够的情况下增加小程序简易控制功能。

- 上位机
   - 使用Unity3D制作
   - 参数配置功能
      - 基础功能配置
         - 时间
         - 睡眠及人体感应
      - 灯光配置
         - 预设及参数配置
         - 视频解析功能
   - 灯光模拟功能
   - 局域网查找设备功能
   - OTA升级功能
- 单片机系统
   - 系统功能
      - WI-FI连接，初期考虑直接写死wifi SSID及密码
      - 蓝牙连接
      - OTA升级
      - WEB简易控制端
   - 灯光控制
      - 同上位机模拟器

## 灯光控制

灯光控制分为两种:

 - 一个是根据各类计算函数进行颜色变化，可以参考各类开源软件
 - 另一个考虑到摈弃工科生思维，从艺术创作角度考虑，需要提供更自由的视觉表达能力，因此增加短视频 / GIF解析能力，素材建议是2D色块动画，需要制作processing / unity / cocos2d 素材模板，最终会根据素材圆环内外进行计算，组成变换矩阵，直接显示。

## 多设备交互流程图

``` mermaid
sequenceDiagram
    MorningRing A->>MorningRing A: 启动
    MorningRing A--xMorningRing B: UDP广播A设备信息并期待返回
    MorningRing A-->>终端设备: mDNS组播 MorningRing.local 域名
    MorningRing B->>MorningRing B: 启动
    MorningRing B-->>+MorningRing A: UDP广播B设备信息并期待返回
    MorningRing A->>MorningRing A: 记录B设备信息
    MorningRing A->>-MorningRing B: UDP发送A设备信息
    MorningRing B->>MorningRing B: 记录A设备信息,自身序号递增
    MorningRing B-->>终端设备: mDNS组播 MorningRing.local 域名
    终端设备->>+MorningRing B:访问MorningRing.local
    MorningRing B->>-终端设备: 基础html
    终端设备->OBS or Github: 下拉js / css资源文件
    终端设备->终端设备: 单页Web应用渲染
    终端设备->>+MorningRing B: Req:/api/device-list
    MorningRing B->>-终端设备: Res:/api/device-list
    终端设备->>+MorningRing B: Req:/api/device-info
    MorningRing B->>-终端设备: Res:/api/device-info
    终端设备->>+MorningRing A: Req:/api/device-info
    MorningRing A->>-终端设备: Res:/api/device-info
    MorningRing A-->>MorningRing B: UDP广播A设备信息
```



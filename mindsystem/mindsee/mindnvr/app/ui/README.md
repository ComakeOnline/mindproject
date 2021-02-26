2.为了学习亮哥的代码而做的备份，学习结束之后，使用mv ui02 ui 恢复。

3.为了使用新控件滑动窗口类型而做的备份，如果滑动窗口做出来的效果不满足需求，则使用 mv ui03 ui

4.成功做出了滑动图标效果，没有闪烁现象，然而新BUG为：滑动太快时，有丢帧现象。
	准备使用双窗口绘制，学习前先备份，恢复时使用mv ui04 ui

5.移动滑动窗口有BUG，滑动太快会丢失部分细节，Bitblt() 的图像也会丢。
	解BUG的过程中想到，如果将多个图标整个BitBlt() 能不能提高性能。
	新思路施行前先备份。恢复: mv ui05 ui

#6.发现是printf 语句导致程序运行卡顿时，都无语了。
	好吧，问题解决了，备份一个有用的版本ui06.

7.修正了左右端填充重绘部分的代码，修正了无效区域矩形+1-1的代码。
	备份至ui07

8.完成了滑动图标的制作，将代码合入了yume 的工程，修改了若干CMakelists.txt
	修改了manager.cpp，增加了函数 UI_MatchInputDev() 适配输入设备。
	备份至ui08

#9.符合james需求的版本，备份为ui09, 接下来准备做两方面的内容：滑动字体和二级页面

10.详见ui10

11.制作弹窗和对讲功能，为了同步jerry 的代码而做的备份。
	mv ui11 ui

12.将thirdparty/lib 中的所有库更换为静态库，新增res/uicfg 配置。
	将nvr_demo 和 res 拷贝到开发的/customer 下，然后在开发板的/customer/demo.sh 
	中增加如下内容后，即可开机自动运行nvr_demo.
	内容开始：
	export TSLIB_ROOT=/customer/res/uicfg
	export TSLIB_CONFFILE=$TSLIB_ROOT/ts.conf
	export TSLIB_PLUGINDIR=$TSLIB_ROOT/ts
	export TSLIB_CALIBFILE=$TSLIB_ROOT/pointercal
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TSLIB_ROOT/
	cd /customer                           
	./nvr_demo
	内容结束。
	保存这份代码到ui12

13.应james 要求，将smarttalk 中的背景图扣掉，保留图标。
	做出了效果，保留至ui13.

14.基于smarttalk 和13，保留icon 和icon 背景。
	优化了触摸控制：长按打开导航栏，在导航栏上3击空白处关闭。
	cp ui ui14

#15.基于smarttalk, 移植各图标功能，做二级菜单，目前做好了call 图标的二级页面。
	后面都仿造该方法制作。备份至ui15

16.smarttalk 基本移植完成。setting 图标对应的页面中，仅仅滑动控件没有添加。
	cp ui ui16

17.用photoshop 做了网络设置和时间设置的界面。比minigui 控件做出来的效果要好一点。
	修改了MiniGUI.cfb 中的truetypefonts 字段，新增幼圆simyou.ttf 字体。
	cp ui ui17 

18.制作存储空间查看页面。
	cp ui ui8

#19.制作了视频回放列表页面、系统信息页面。精简了ttf 字体。
	准备做圆角图标。
	cp ui ui19

#20.做了圆角图标，主要是修改了homepage.cpp 和resource.cpp.*
	cp ui ui20

21.做了OSD 弹窗显示；利用yume 提供的接口做了通话按钮、音量设置。
	cp ui ui21

22.修改了MiniGUI.cfg, 剪裁不必要的配置文件。
	cp ui ui22

#23.精简的minigui 配置文件，替换了字体，压缩了png 图片，删除了不必要的库文件。*
	cp ui ui23

24.保存，准备做小界面的UI.
	cp ui ui24

25.小界面完成了，备份一下。
	cp ui ui25

26.ui release v1.0
	cp ui ui26

27.优化了单画面的图标显示方式，优化了触摸体验
	cp ui ui27

28.内存使用情况：
status	free(M)	available(M)
开机	22.8	33.1
yume	18		30
ui-min	14.6	29.8
ui-mid	7		23
ui-max	4		20
ui-pthread	14	30

一张1024*600 的图*, load 进内存后占用空间1.8M，算出一个像素点为3字节。
全部资源采用动态加载的方式，可以看出，ui 线程跑起来后只占不到1M的内存。
cp ui ui28

29.ui release v1.1
	可以demo, 但存在bug, 连续快速点击屏幕两下，UI 不再响应。
	cp ui ui29

30.连续触摸间隔 < 100ms 时，导致ui卡掉的BUG 已经修复。
	cp ui ui30

31.优化触摸体验，不响应连续的快速点击。
   减少图标数量。
   优化窗口退出时对资源的清理。
   cp ui ui31

32.完成OSD 弹窗的接口，明日实现。
	更改代码结构，homepage 调整为iconscreen, minigui_entry 和mainscreen 分离。
	cp ui ui32

33.单画面重入问题已经解决，准备解决滑动图标界面下，不检测人形的问题。
	cp ui ui33

#34.发布版。ui release v2.0
	OSD 弹窗。
	HC 人形检测。
	双向对讲。
	滑动图标。
	cp ui ui34
	准备应用mgncs.

35.注意minigui_test.cpp 文件，是mgncs 的应用。
	cp ui ui35

36.根据james 需求，提高滑动图标的触摸体验。
	思路：触摸抬起之后，在定时器中逐渐递减偏移量。
	cp ui ui36

37.OSD 字体更改为红色；提升活动图标的触摸体验。
	cp ui ui37

#38.增加了开机启动界面，倒数计时。		ui release v2.1
	修复了触摸失效的BUG, 不能投递MSG_QUIT. 
	minigui-Thread 同一线程下的窗口共享同一窗口消息。
	精简了minigui 库。
	cp ui ui38

39.增加WLAN 设置页面。
	逻辑部分已经完成，下一步，对接接口以及UI 美化。

40.WLAN 页面完成，已经应用了yume 提供的接口。准备做版本信息显示。
	cp ui ui40

41.完成系统信息显示界面。增加WLAN 设置界面的WIFI 图标。
	cp ui ui41

42.新增开关界面：人形检测开关、人形检测结果弹窗开关、语音识别开关、网络状态显示开关。
	cp ui ui 42

43.完成程序注释，十分详尽！小幅度调整部分代码。
	cp ui ui43

44.增加UI 打印信息的开关宏。
	修复短时间内连续收到HC_DETECT 时UI 的BUG.
	解决时间测试模块中的BUG.
	cp ui ui44

45.修复了HC_DETECT 频繁重复发送时的BUG.
	升级到MINIGUI4.0.4
	cp ui ui45

46.MINIGUI4.0.4 修复了若干错误后的稳定一版：
	cp ui ui46

47.MINIGUI4.0.4 的最后一版，因为内存开销太大，准备回退到3.0.12.
	修复了网络信息显示的BUG. 完成了RSSI 的显示。
	cp ui ui47

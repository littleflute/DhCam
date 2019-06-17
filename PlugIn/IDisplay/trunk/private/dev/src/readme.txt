 
..\..\..\..\..\..\..\demo\bin\hvperf_cnD.exe
..\..\..\..\..\..\..\demo\bin\PlugIns\IDisplay.plx
 
//	GetWhiteBalanceRatio(pSrce, nWid, nHei, dRatioR, dRatioG, dRatioB);

//	SetGammaLut(pLutGamma, dGammaRatio);
//	ConvertBayer2Rgb(pDst, pSrc, nWid, nHei, &m_pParam);

//
// Project: 色彩校正插件改进
// Author : XuYibo (xyb), ZhangXiaoSong(Dimission) 2006-08-31
//

1. 简介

  在使用中发现，相机演示程序中使用的色差较正插件有些地方不方便，需要改进。

2. 需求

  1，界面改进，从附录1的原来界面改为附录2界面；
  2，Bayer插值使用Raw2Rgb动态库；
  3，只使用Bayer插值算法1（即原来的Fast算法）；
  4，白平衡不能手动改变；选择"白平衡"，使用原来记忆的白平衡系数；不选择
     "白平衡"，白平衡系数的RGB值恢复为100；在选择"白平衡"时，在活动画面
     上选择区域，则计算改区域的白平衡值作为当前的白平衡系数；
  5，界面上增加"彩色"选择项，不选择该项，界面的其余部分变灰，默认使用
     "数据传输"插件，同时不做下面3件事情：
     1），对采集图像做Bayer插值；
     2），色彩校正；
     3），选择数据类型；
     如果选择"彩色"项，界面的其余部分正常，同时允许作上述3件事情。

3. 修改日志
[2008-1-14]
        1.当采图为8位时，不能做白平衡。
        版本升级为1.6.1.0
[2007-9-21]
	1.增加支持SV400FCTYPE、SV1410FCTYPE、SV1420FCTYPE、SV2000FCTYPE相机
	2.增加了英文版本
	3.版本升级为1.6.0.3
[2007-3-19]
	* 翻译为英文版本

[2006-10-30]
	* Fix a bug, that in 8 bits mode, have no image.
	  This is cause by CDlgSetParameter::OnShowFrame
	* Fix a bug, click the cancel button, the m_DoColor have no effect.

[2006-09-08]
	Manager ZhangCe said it's should change this plugin UI, so i change some
	codes and UIs.

	I delete many code, which are only need in the old version of UI.

	CImage::Draw(...)   xuyibo
    //
    // Display image
    // When used with COLORONCOLOR mode, the gray image has many bad pixels,
    // i test STRETCH_HALTONE, it's good, so i changed this code.
    //

    //dc.SetStretchBltMode(COLORONCOLOR);
    dc.SetStretchBltMode(STRETCH_HALFTONE);

[2006-09-02 xyb]
	First internal release, change release version to 1.5.0.0

	In this release, i implement all the characters in [Project Context],
	because it's hard to use raw2rgb.dll, so i don't change the interfix
	codes.

	Because the it's hard to list all modify place, it's really hard, it's
	change many many places, i suggest you'd better use bcompare.exe to find
	the many tiny change places. Now i list some big place that i changed:
	(1) Ctl+W invoke the class wizard, i delete some variable that doesn't 
		need, and in resource panel, i modify the panel layout, delete some
		unused control, like three scoll bar...

	(2) The main added variables are in class CDlgSetParameter and class
		CParameter. Including m_pRatioR, m_pRatioB in class CDlgSetParameter,
		m_bDoColor in class CParameter. you also can find them by comment format:
		general my comment format is:
    
		//
		// Comment here
		//
    
		Some codes, or variables.

	(3) There is a point i should say, that the modified code maybe not the 
		best one. I think the design of this software was not very good, if
		using C, it can be more simple and elegent, I don't like C++.

[2006-08-31 xyb]
	Start Project.

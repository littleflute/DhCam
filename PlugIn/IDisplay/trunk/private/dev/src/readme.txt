//
// Project: ɫ��У������Ľ�
// Author : XuYibo (xyb), ZhangXiaoSong(Dimission) 2006-08-31
//

1. ���

  ��ʹ���з��֣������ʾ������ʹ�õ�ɫ����������Щ�ط������㣬��Ҫ�Ľ���

2. ����

  1������Ľ����Ӹ�¼1��ԭ�������Ϊ��¼2���棻
  2��Bayer��ֵʹ��Raw2Rgb��̬�⣻
  3��ֻʹ��Bayer��ֵ�㷨1����ԭ����Fast�㷨����
  4����ƽ�ⲻ���ֶ��ı䣻ѡ��"��ƽ��"��ʹ��ԭ������İ�ƽ��ϵ������ѡ��
     "��ƽ��"����ƽ��ϵ����RGBֵ�ָ�Ϊ100����ѡ��"��ƽ��"ʱ���ڻ����
     ��ѡ����������������İ�ƽ��ֵ��Ϊ��ǰ�İ�ƽ��ϵ����
  5������������"��ɫ"ѡ�����ѡ������������ಿ�ֱ�ң�Ĭ��ʹ��
     "���ݴ���"�����ͬʱ��������3�����飺
     1�����Բɼ�ͼ����Bayer��ֵ��
     2����ɫ��У����
     3����ѡ���������ͣ�
     ���ѡ��"��ɫ"���������ಿ��������ͬʱ����������3�����顣

3. �޸���־
[2008-1-14]
        1.����ͼΪ8λʱ����������ƽ�⡣
        �汾����Ϊ1.6.1.0
[2007-9-21]
	1.����֧��SV400FCTYPE��SV1410FCTYPE��SV1420FCTYPE��SV2000FCTYPE���
	2.������Ӣ�İ汾
	3.�汾����Ϊ1.6.0.3
[2007-3-19]
	* ����ΪӢ�İ汾

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

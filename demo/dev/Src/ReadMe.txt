
--2019.6.15 sat--hyl--usa--
6. void CDevice::xdMakeDat2Show(BYTE *p1, BYTE *p2, int w, int h)// : p1 => p2
5. m_pCurrentDevice->GrabFromFile( FileName);
4. ::CopyMemory(*m_pBitmap,*m_pCurrentBuffer,1392*1024); //xdTest2:

3. int CALLBACK CDevice::SnapCallbackProc(HV_SNAP_INFO *pInfo)
2. if(UNKNOWN_TYPE==type){ type = SV400FCTYPE} //xdTest1:
1. debug: 	::HVGetDeviceTotal(&m_nDeviceNum);



--------2006.11.10-----hyl-------
建立本文档,当做本工程的日志文件.

1.修正bug: 
	CCD摄像机刚上电启动本程序时,Lut是默认值(Lut4),与程序属性页上显示的当前值不符!
  原因:
      程序初始化时,没有对Lut项目进行初始化.
  解决办法:
      在void CDevice::Refresh()中,(祥见工程中代码)
	  /*
		if (!(IS_CCD_CAMERA(m_pInfo->DeviceType()))) {
	    m_pADCLevel->Refresh(); //LUT of CCD.
		}
	 */
	   m_pADCLevel->Refresh();

版本升级到: v2.4.1.1

--------2006.12.22-----hyl-------
1.本版本,经初步测试,好象,对HV3100FC不存在无法采集图像的问题.
  有待进一步测试,以确认!!!
  V2.4.1.1

2. //Added for Test by HYL 2006.12.22
    : WM_DH_TEST_1
	: #define ID_CMD_TEST_ON			70004


 版本升级到: V2.4.2.3

--------2007.02.05-----hyl-------
1. M: 
	CSV1310FCSnapSpeed   //modified by HYL 2007.02.05 
2. M:    HVDef.h    //*****
	#define HV_SUCCESS(status) ( ((status) == STATUS_OK) && ((status) == STATUS_NOT_SUPPORT_INTERFACE) )

V2.4.2.3

--------2007.02.06-----hyl-------
1. A: 
	CChildFrame::On_Dh_Test_1  中:
	m_MainFrame.CurrentDeviceChanged(m_DeviceManager.GetCurrentDevice());	
	return 0;

2. T:
	Xd_Set_PackeSize()

--------2007.02.07-----hyl-------
1. M,A: 
	修正包长变化步长为4.
	代码见: 
	//added by HYL 2007.02.07
	//Modified by Hyl 2007.02.07
V2.4.2.3

--------2007.02.13-----hyl-------
1. M,A:
	SV1311FCTYPE = 22,
	SV400FCTYPE  = 23,
	SV400FC
V2.4.2.3

--------2007.03.02-----hyl-------
1. U:  v2.4.2.4

--------2007.03.23-----hyl-------
1. M: 
	CMainFrame::OnCreate()
	(1)
	m_Dock.DockWindow(m_BusView, DOCK_LEFT/*, iBusCy*/); 
	m_Dock.DockWindow(m_PropertyView, DOCK_LEFT/*, iPropertyCy*/);
	-->
	m_Dock.DockWindow(m_BusView, DOCK_LEFT, iBusCy); 
	m_Dock.DockWindow(m_PropertyView, DOCK_LEFT, iPropertyCy);

	(2) 注释
	//	m_Dock.SetPaneSize(DOCK_LEFT, 200);
	//	m_Dock.SetPaneSize(DOCK_RIGHT, 120);
	//	m_Dock.SetPaneSize(DOCK_TOP, 50);


v2.4.2.4

--------2007.03.26-----hyl-------
1. A: HHV	GetHandle(){return m_hDevice;}
   A: Spec_Fun_Interface_1
   M: CString strNodeId = pDevice->m_pInfo->NodeId();
   M: m_BlueGain,...
   A: GetDeviceInfo


v2.4.2.4
=====================================


---3.28----
DESC_DEVICE_FPGA_VERSION  //FPGA版本
v2.4.2.4----------->v2.4.2.5


----2007.4.2----------

修正v2.4.2.5的bug   写曝光单位	
pDevice->SetShutterUnit(m_CurrentUnit);//4.2
增加对SV1311FC的支持...

v2.4.2.5---------->v2.4.2.6


-----2007.4.3-----
1.增加恢复摄像机配置停-采流程
  CMainFrame::OnFileRestoreConfiguration

2.1311 最小包长为 400

v2.4.2.6---------->v2.4.2.7
----


-----2007.4.4-----
1. A: 初始时不展开某个属性项：
	CString str;
	str.LoadString(IDS_INFOPROPERTY);	
	HPROPERTY prp=m_ListView.FindProperty(str);
	prp->Activate(PACT_EXPAND,0);	
v2.4.2.7
----
-----2007.4.6-----
1. A:补充对接口函数返回值的检查if ( ! HV_SUCCESS(status))
			                     throw HVBaseException(status, "HVDAILT.dll : HVCommand"); 	
v2.4.2.7
----


----2007.4.10-----
加入SV1311FM

v2.4.2.7--------->v2.4.2.8

----2007.4.11-----
1. M: #define HV_SUCCESS(status) ( ((status) == STATUS_OK) )
   CSV1311FMSnapSpeed ，CSV1300FMSnapSpeed,，CSV1300FCSnapSpeed....... 不支持
2.  //m_pBlank->Refresh();
v2.4.2.8
----------------

----2007.4.17----------

1.界面上不显示8位和12位选择功能
2.lut取值选择方式改为打开窗口选择lut文件的方式
  为得到lut文件的绝对路径修改接口CPropertyFileNameItem::GetDisplayValue
3.m_LutPath

v2.4.2.8-------------->v2.4.2.9

=======================

----2007.5.22---
V2.4.2.9----->v2.5.0.0

---2007.6.5---
DWORD	 m_dwLut[4*1024];
12位图像显示

v2.5.0.0----->v2.5.0.1

---2007.6.6--
1.增加延迟传输功能
  ListAddTransfersDelay
2.测试图对于1311只保留三项


v2.5.0.1

--2007.6.8---
增加对SV400FM的支持
IS_CCD_CAMERA(type)

v2.5.0.1

---2007.6.14---
1.界面上不显示8位和12位选择功能
2.修改SV1311FC 红蓝通道初始化值
v2.5.0.1---->v2.5.0.2

--2007.6.20----
为SV1300FC增加闪光灯开关
v2.5.0.2--->v2.5.0.3

--2007.7.27----
修正bug  SV400FC/M界面中未显示出闪光灯开关一项
IS_SV400(pDevice->m_pInfo->DeviceType())
v2.5.0.3--->v2.5.0.4

--2007.8.10---
增加菜单项设置默认值
OnSetDefaultValue
v2.5.0.4

-------- 2007.8.20 --- HYL -------
1. A: #pragma comment(lib,"..//lib//hvdailt.lib")
   *同时去掉hvdailt.lib文件的直接引用
   
   A: IS_SV1410,

2. V: 2.5.0.5

-------- 2007.8.20 --- HYL -------
1. A: bFindRaw2RGB (实现默认情况一定选上 IDisplay.plx 插件)
   A: ID_DEFT_VAL 工具条按钮
2. M:
	try{
				m_LutPath        = ptrBag->GetBag("LutPath")->ReadString("String");
			}
			catch ( HVBaseException& /*e*/)
			{
			//	m_MainFrame.ReportError(e);
			}

V: 2.5.0.5

-------- 2007.8.23 --- HYL -------
1. A: 支持SV1410FC.
2. V: 2.5.0.6
3. M: 
	if(IS_SV1410(pDevice->m_pInfo->DeviceType()))
		 {
			 Set_Def_AOI(CRect(CPoint(0, 0), CSize(1392,1040)));
		 }

   M: OnSetDefaultValue
   
		if (IS_SV1410(pDevice->m_pInfo->DeviceType())||
			IS_SV2000(pDevice->m_pInfo->DeviceType())
			)
		{
			pDevice->m_PacketSize=2800;
			pDevice->m_pShutter->Set(60);
		}
		else if (IS_SV400(pDevice->m_pInfo->DeviceType()))
		{
			pDevice->m_PacketSize=2800;
			pDevice->m_pShutter->Set(20);
		}
V:2.5.0.6

-------- 2007.8.29 --- HYL -------
1. V: 2.5.1.0
   A: ListAdd8or12BitMode(pDevice);
   M: IDS_8or12BitMode        "8位和16位图像可选功能"
   M: raw12 -> raw16

v2.5.1.0

-------- 2007.8.30 --- HYL -------
1. V: 2.5.1.2
   M: IDS_8or12BitMode        "8位和16位图像可选功能""
       CString str = "数据格式";
	   m_ListView.AddItem( PropCreateCategory(str) );

2.  M: OnSetDefaultValue
	
		 if(IS_SV2000(pDevice->m_pInfo->DeviceType()))
		 {
			 Set_Def_AOI(CRect(CPoint(0, 0), CSize(1628,1236)));
		 }
		 if(IS_SV1410(pDevice->m_pInfo->DeviceType()))
		 {
			 Set_Def_AOI(CRect(CPoint(0, 0), CSize(1392,1040)));
		 }
		 if(IS_SV1311(pDevice->m_pInfo->DeviceType()))
		 {
			 Set_Def_AOI(CRect(CPoint(0, 0), CSize(1280,1024)));
		 }
		 if(IS_SV400(pDevice->m_pInfo->DeviceType()))
		 {
			 Set_Def_AOI(CRect(CPoint(0, 0), CSize(780,582)));
		 }

3. M: 最大包长 4092 -> 4096

4. A: #define _WIN32_WINNT	0x0400

v2.5.1.2
-------- 2007.8.31 --- HYL -------
1. V:2.5.1.3
   A: SetDefaultValue

v2.5.1.3
-------- 2007.9.3 --- HYL -------
1. V:2.5.1.4
2. M: void CPropertyView::Update_PRP(CDevice* pDevice)  // 原来是: CurrentDeviceChanged
3. A: SV2000FX,SV1420FX

v2.5.1.4
-------- 2007.9.6 --- HYL -------
1. V:2.5.1.5
2. M: HV3100FC.h HV2000FC.h
     以前这两款摄像机的曝光时间计算有误,现改正过来了.

V2.5.1.5

-------- 2007.9.10 --- HYL -------
1. V:2.5.1.6
2. M: 
	void CDevice::InitializeDevice()
	{
		SetResolution(m_Resolution);
		m_SensorSize = SensorSize(m_Resolution);
		m_ImageSize = m_SensorSize; //A:
		SetOutputWindow(m_Origin.x, m_Origin.y, m_ImageSize.cx, m_ImageSize.cy);
	}
	void CDevice::MaximizeAOI()
	{
	
		SetAOI(CRect(CPoint(0, 0), Format.SensorSize()));
		Refresh(); //A:
	}



V2.5.1.6
-------- 2007.9.11 --- HYL -------
1. V:2.5.1.7
   M: 
	
				DWORD dwFPGAVer;
				m_pDevice->GetDeviceInfo(DESC_DEVICE_FPGA_VERSION, &dwFPGAVer, &size);
	//只对CCD

V2.5.1.7

-------- 2007.9.17 --- HYL -------
1. V:2.5.1.8
2. A: SV1420FM,SV2000FM

v2.5.1.8

--2007.9.19---HYL---
1. M: HVPerf.exe -> HVPerfFW.exe;
3. V2.5.1.9 (与 cn 同步)

--2007.9.25---HYL---
1. M: 
	assert(m_pBlackLevelEnable == NULL);
	m_pBlackLevelEnable = new CHV1300FMBlackLevelEnable(1, (CDevice *)this);
	assert(m_pBlackLevel == NULL);
	m_pBlackLevel = new CHV1300FMBlackLevel(-20, (CDevice *)this);
	assert(m_pBlackLevelEnable == NULL);
	m_pBlackLevelEnable = new CHV1300FCBlackLevelEnable(1, (CDevice *)this);
	assert(m_pBlackLevel == NULL);
	m_pBlackLevel = new CHV1300FCBlackLevel(-20, (CDevice *)this);
	assert(m_pBlackLevelEnable == NULL);
	m_pBlackLevelEnable = new CHV2000FCBlackLevelEnable(1, (CDevice *)this);
	assert(m_pBlackLevel == NULL);
	m_pBlackLevel = new CHV2000FCBlackLevel(-20, (CDevice *)this);
	assert(m_pBlackLevelEnable == NULL);
	m_pBlackLevelEnable = new CHV3100FCBlackLevelEnable(1, (CDevice *)this);
	assert(m_pBlackLevel == NULL);
	m_pBlackLevel = new CHV3100FCBlackLevel(-20, (CDevice *)this);

	hExclusion = CreateMutex(NULL, FALSE, "{8FA6E763-BAC1-4db0-AFC3-FA53ED373513}");
	->	hExclusion = CreateMutex(NULL, FALSE, "{8FA6E763-1394-4db0-AFC3-FA53ED373513}");

	SV1420FC.h SV1420FM.h :
		virtual CString ModelName()
		{
			char lpTemp[256];		  
			LoadString(NULL,IDS_STRING_SV,lpTemp,MAX_PATH);	
			CString str;
			str.Format("%s%s", lpTemp,"1420FC");
			return str;
		}

2. M: 修正BUG:
		1. 显示AOI 不对
		原因: m_ImageSize = m_SensorSize; //A: (2007.9.10)

void CDevice::InitializeDevice()
{
	SetResolution(m_Resolution);
	m_SensorSize = Get_Sensor_Size(m_Resolution);
//	m_ImageSize = m_SensorSize;
	SetOutputWindow(m_Origin.x, m_Origin.y, m_ImageSize.cx, m_ImageSize.cy);
}
3. V: 2.5.2.0 

v2.5.2.0
------------------------------
--2007.9.26---HYL---
1. V:2.5.2.1
2. A: Set_Default_AOI
	使第一次安装启动程序时AOI默认值正确,而不是 400*300.

3. M: 修正BUG: Ctrl + N 可以打开新的显示窗口.
	LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: add code to initialize document
	if (m_DeviceManager.GetCurrentDevice() != NULL && \
		!m_DeviceManager.ExistMDIChild())
	{
		m_DeviceManager.AddMDIChild();
	}
	
	return 0;
}

4. A:  m_bClearReg , CMainFrame::OnFileClearReg()
   M: void CDeviceManager::AppExit(bool b)
		
				if(!b){
					CPropertyBagPtr cameraBag = CRegistryPropertyBag::Create(APPLICATION_KEY + pDevice->m_pInfo->DeviceName() + "\\Device");
					pDevice->SaveConfiguration(cameraBag);
				}
				else{
					CString keyName = APPLICATION_KEY;
					SHDeleteKey(HKEY_CURRENT_USER,keyName);
				}
   *这是新添功能: 退出时清除历史记录,下次再启动程序时,将没有历史记录

v2.5.2.1

------------------------------
--2007.9.30---HYL---
1. V:2.5.2.2
2. M:
	包长默认值及取值范围(参考李永怀文档 V1.12)
	m_pShutter = new CSV2000FCShutter(70, (CDevice *)this);
	m_pShutter = new CSV2000FMShutter(70, (CDevice *)this);
	m_pShutter = new CHV2000FCShutter(100, (CDevice *)this);
	m_pShutter = new CHV3100FCShutter(152, (CDevice *)this);

	m_pADCLevel = new CHV3100FCADCLevel(ADC_LEVEL3, (CDevice *)this);
	m_pADCLevel = new CHV1300FCADCLevel(ADC_LEVEL3, (CDevice *)this);
	m_pADCLevel = new CHV1300FMADCLevel(ADC_LEVEL3, (CDevice *)this);
	m_pADCLevel = new CHV2000FCADCLevel(ADC_LEVEL3, (CDevice *)this);

	void CPropertyView::OnSnapModeChanged(CDevice *pDevice, int index);
	void CPropertyView::ListAddSnapMode(CDevice* pDevice);

	m_StrobeOnOff=1; 
	m_Brightness=16;

v2.5.2.2

--2007.10.12---HYL---
1. V:2.5.2.3
   A: SV1400FC,

V2.5.2.3

--2007.10.19---HYL---
1. M: CSV1400FC::CreateFeatures()

V2.5.2.3

--2007.10.22---HYL---
1. M: 
		else if (IS_SV1410(pDevice->m_pInfo->DeviceType())||
			IS_SV1420(pDevice->m_pInfo->DeviceType())
			)
		{
			pDevice->m_PacketSize=2800;
			pDevice->m_pShutter->Set(60);
		}

2. V: 2.5.2.4

--2007.10.24---HYL---
1. V: 2.5.2.5

2. M: 程序启动时,设备列表的高度由设备数量的多少决定. 
	
	::HVGetDeviceTotal(&m_nDeviceNum);		
	iBusCy = 80 + 40 * GetDeviceNum();  
	int nTotal = m_MainFrame.GetDeviceNum(); 

3. A: 在子窗口的 title 中显示 FPS.
	XD_Show_All_FPS, m_WinName, XD_Get_FPS

4. M: [包长设置拉杆条]在放开 Mouse 时设置才生效
	OnTracekEditKeyDown,OnTracekEditKeyDown, OnTrackKillFocus
	if(nLs!=4)
	  {
		  ::SendMessage(GetParent(), WM_USER_PROP_SCALARPROPERTY, nPos, (LPARAM) m_hWnd);
	  }
   LRESULT OnReleasedCapture(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
   {
	  ATLTRACE("CPropertyTrackWindow::OnReleasedCapture\n");
	  int nPos=m_wndTrack.GetPos();
	  ::SendMessage(GetParent(), WM_USER_PROP_SCALARPROPERTY, nPos, (LPARAM) m_hWnd);
      SetFocus();
      return 0;
   } 

5. M:
	// Set Video mode
void CDevice::SetVideoMode(HVResolution mode)
{
	if ( ReconfigurationRequest() )
	{
		CHVBase::SetResolution(mode);
		m_Resolution = mode;
		m_SensorSize = Get_Sensor_Size(mode);
				
		m_Origin.SetPoint(0, 0);
		m_ImageSize = m_SensorSize;
		CHVBase::SetOutputWindow(m_Origin.x, m_Origin.y, m_ImageSize.cx, m_ImageSize.cy);

		m_pShutter->Refresh();
	
		ConfigurationChanged();
		MaximizeAOI(); //新增
	}
}

6. M: 使[速度单位]和[快门速度]的属性页调节界面符合需求
     HV1300FM,HV1300FC,HV2000FC,HV3100FC
	 SV1300FM,SV1300FC,SV1310FC,SV1310FM
	 SV1310FC,SV1310FM,SV1410FC,SV1410FM
	 SV1400FC,SV1400FM
	 SV1420FC,SV1420FM
	 SV2000FC,SV2000FM
	 SV400FC,SV400FM

		void CPropertyView::OnShutterUnitChanged(CDevice *pDevice, int index)
			Update_PRP(pDevice);
			pDevice->m_pShutter->Refresh();

		virtual unsigned long Min(){ 
			ULONG ulRet;
			TUnit u = m_pDevice->m_pShutter->m_Unit.Value();
			switch(u){
			case UNIT_MICROSECOND:
				ulRet = 20; //各型号不一样
				break;
			case UNIT_MILLISECOND:				
			case UNIT_SECOND:
				ulRet = 1;
				break;
			}
			return ulRet;
		}
		virtual unsigned long Max(){ return 3000;}
    
		virtual void Set(unsigned long Value, void *pContext = NULL)
		{
		//	if (Value > Max() || Value < Min())
		//		throw HVBaseException(STATUS_PARAMETER_INVALID, "CShutter::Set()");
			Value = Value<Min()?Min():Value;
			Value = Value>Max()?Max():Value;
			
		 A: virtual unsigned long Value() //CShutter
		  { 
			  ULONG ulRet;
			  if (m_fAbsControl)
				  ulRet = m_AbsValue;
			  else
				  ulRet = m_RawValue;

			  ulRet = ulRet<Min()?Min():ulRet;
			  ulRet = ulRet>Max()?Max():ulRet;
			  return ulRet;
		  }


v2.5.2.5

--2007.10.31--HYL---
1. V: 2.5.2.6

2. M: 
	修正 v2.5.2.5 的BUG: 多机时,当有一机采集,另一机不打开子窗口,但打开属性页时,有异常.
	修正如下:
	LRESULT CMainFrame::OnTimer(UINT, WPARAM wParam, LPARAM, BOOL&)
	CChildFrame* pChildFrame = m_DeviceManager.GetCurrentChild();
		if(pChildFrame){ // 保证有效
			TCHAR szCaption[128] = { 0 };    // max text length is 127 for floating caption
			::GetWindowText(pChildFrame->m_hWnd, szCaption, (sizeof(szCaption) / sizeof(TCHAR)) - 1);
		}

3. A:	m_MainFrame.m_bToClose
	保证程序退出时不运行 XD_Show_All_FPS()
	if(!m_MainFrame.m_bToClose)		m_MainFrame.m_DeviceManager.XD_Show_All_FPS();

4. M:  class CBusNode 
	
		  /// get Win32 device name
		  int GetDeviceNumber() const {return m_nNumber;}
		  /// get Win32 device name
		  CString GetDeviceName() const {return m_DeviceName;}
		  /// get type of node
		  HVTYPE GetType() const {return m_Type;}

			int n = pNode->GetDeviceNumber();
			DWORD error = m_DeviceManager.CurrentDeviceChanged(n);

5. M:  void CPropertyView::ListAdd8or12BitMode(CDevice* pDevice)
		如果正在采集,不能停止采集

V 2.5.2.6

--2007.11.2--HYL---
1. V: 2.5.2.7

2. M: 
	iBusCy = 80 + 40 * GetDeviceNum();

	m_Dock.SetPaneSize(DOCK_LEFT, 250);//调整宽度

v2.5.2.7

--2007.11.16--HYL---
1. V: 2.5.2.8

2. M: 
	 if(IS_SV1400(pDevice->m_pInfo->DeviceType())||    //Added 
			 IS_SV1410(pDevice->m_pInfo->DeviceType())||
			 IS_SV1420(pDevice->m_pInfo->DeviceType()))
		 {
			 Set_Def_AOI(CRect(CPoint(0, 0), CSize(1392,1040)));
		 }

v2.5.2.8

--2007.11.21--HYL---
1. V: 2.5.2.9

2. A: 
	 
	SetDefaultValue();
    CurrentDeviceChanged(pDevice);
	SetDefaultValue();	//这样的代码不好看,但暂时这么用
    CurrentDeviceChanged(pDevice); //这样的代码不好看,但暂时这么用

	*恢复出厂值有问题,暂时这么凑合修改.

v2.5.2.9

--2007.11.28--HYL---
1. V: 2.5.3.0
   定版与en 同步
v2.5.3.0

============================================


---2008.1.14---JiaPeng----
1.  HVDef中增加
    PD1420ECTYPE = 35,
	PD1420EMTYPE = 36,
	SV1420GCTYPE = 37,

    v2.5.3.0
---2008.1.15---JiaPeng----
1. 增加PD1420EC.H,PD1420EC.CPP
       PD1420EM.H,PD1420EM.CPP
	   SV1420GC.H,SV1420GC.CPP
2.
  #define IS_PD1420(type)          ((type) == (PD1420ECTYPE) || (type) == (PD1420EMTYPE)) 
  #define IS_SV1420GC(type)          ((type) == (SV1420GCTYPE))

   v2.5.3.0->v2.5.3.1

--2008.1.24 Thu --HYL--
1. V:2.5.3.2
2. M: SV1420GCTYPE -> SV1410GCTYPE
       SV1410GCTYPE = 37,


--2008.2.3 Sun --HYL--
1. V:2.5.3.3
2. A:	
	#define IDS_STRING_HV1300FC             61450
	#define IDS_STRING_HV1300FM             61451

	#define IDS_STRING_HV2000FC             61469
	#define IDS_STRING_HV3100FC             61470

--2008.2.4 Mon --HYL--
1. V:2.5.3.4
2. A:	
#define IDS_STRING_SV1300FC             61471
#define IDS_STRING_SV1300FM             61472
#define IDS_STRING_SV1310FC             61473
#define IDS_STRING_SV1310FM             61474
#define IDS_STRING_SV1400FC             61475
#define IDS_STRING_SV1400FM             61476
#define IDS_STRING_SV1410FC             61477
#define IDS_STRING_SV1410FM             61478
#define IDS_STRING_SV1420FC             61479
#define IDS_STRING_SV1420FM             61480
#define IDS_STRING_SV2000FC             61481
#define IDS_STRING_SV2000FM             61482
#define IDS_STRING_SV400FC              61483
#define IDS_STRING_SV400FM              61484
#define IDS_STRING_SV1311FC             61485
#define IDS_STRING_SV1311FM             61486
#define IDS_STRING_SV1410FC             61487
#define IDS_STRING_SV1410FM             61488
#define IDS_STRING_PD1420EC             61489
#define IDS_STRING_PD1420FM             61490

--2008.2.25 Mon --HYL--
1. V: 2.5.3.5
2. M: #define IS_SV1410(type)			((type) == (SV1410FCTYPE) || (type) == (SV1410FMTYPE)|| (type) == (SV1410GCTYPE))
	if (IS_PD1420(pDevice->m_pInfo->DeviceType())||IS_SV1410GC(pDevice->m_pInfo->DeviceType())) // M
	{
		return;
	}

--2008.2.27 Wed --HYL--
1. V: 2.5.3.6
2. D: ITEM_SNAPSPEED CSnapSpeed *m_pSnapSpeed;
		// camera frame rate control
		int			    m_CurrentSpeed;

--2008.2.28 Thu --HYL--
1. 调试: 双机属性项联动问题
		
		D: unsigned long	m_CurrentGainLevel;
		恢复以下文件到以前版本(此问题没出现的版本)
		//File : PropertyItemEditors.h
		这样一来,包长调节时又是不断的刷新了(旧问题)

2. V2.5.3.7




--2008.2.28 Thu --JiaPeng--
1.A:    恢复出厂值时,重新初始化查找表数据(让8位与12位一致)
        DWORD* pNew=pDevice->m_dwLut;
		DWORD y=0;
		for (int x=0;x<4096;x++)
		{
			y=(DWORD)(1.0*x/16.0);   //初始化查找表数据为4-11
			if(y>255)
			{
				y=255;
			}
			*pNew++=y<<24;
			
		}
  M:   m_MainFrame(MainFrame)
       初始化查找表数据为4-11(让8位与12位一致)

2. V2.5.3.8
       

--2008.2.28 Thu --HYL--
1. M: #define IS_CCD_CAMERA(type) #define IS_SV1410(type)	
	  增加 (type) == (SV1410GCTYPE)
	  为[SV1410GC]项目调试用

2. V:2.5.3.9

3. M: void CPropertyView::Update_PRP(CDevice* pDevice)
	if(HV1302UCTYPE == CamType)
	{
		ListAddGain(pDevice);
		return;
	}

	D:		
    if (!(IS_CMOS_1394_CAM(pDevice->m_pInfo->DeviceType()))
		&&!(IS_CCD_CAMERA(pDevice->m_pInfo->DeviceType()))) {
        ListAddBlank(pDevice);
    }

4. V:2.5.4.0

5. A: (胡静已经实现的功能)
	// 初始化时最大的方法 
	//MaximizeAOI();
 
	//禁止AOI 方法,修改以下函数:
	LRESULT CImageView::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)

    *现在的需求是只对某几种摄像机实现以上功能.

--2008.2.28 Thu --JiaPeng--
1.M:修改延迟曝光时间值范围1-4095
    ListAddTriggerDelay
    TRACKBARINFO info;
	info.RangeMax = 4095;
	info.RangeMin = 1;
	info.LineSize = 1;
2.V2.5.4.0

--2008.2.29 Fri --HYL--
1. V: 2.5.4.1

--2008.3.13 Thu --HYL--
1. V: 2.5.4.2
2. M: void CMainFrame::UpdateUI()
     去掉以下内容:	

	//Added for Test by HYL 2006.12.22
    if((pDevice != NULL ) && GrabActive&&(snapmode!=CONTINUATION) )
	{
#define ID_CMD_TEST_ON			70004
		CChildFrame* pChildFrame = m_DeviceManager.GetCurrentChild();
		try
			{
				pChildFrame->m_PluginManager.OnUserCommand(ID_CMD_TEST_ON);
			} CATCH_REPORT();
#undef ID_CMD_TEST_ON
	}
	
	else{
#define ID_CMD_TEST_OFF		70005
		CChildFrame* pChildFrame = m_DeviceManager.GetCurrentChild();
		try
			{
				if(pChildFrame)	pChildFrame->m_PluginManager.OnUserCommand(ID_CMD_TEST_OFF);
			} CATCH_REPORT();
#undef ID_CMD_TEST_OFF

	}	
	//Test===================

--2008.3.27 Thu --HYL--
1. V: 2.5.4.3
2. M: ID_DEFT_VAL 更新该ICON


--2008.4.1 Tue --HYL--
1. V: 2.5.4.4
2. A:
     滤波时间调节接口   
	   
	   (0－2000us DefV: 10us)
      
	 12位数据传输模式选择接口
	   1：Big-Endian（默认）
	   0：Little-Endian

3. A: virtual BOOL SetRange(int nMin, int nMax)  = 0;
	  void SetItemRangeValue(HPROPERTY prop, int nMin, int nMax)
   M: void CPropertyView::OnShutterUnitChanged(CDevice *pDevice, int index)

--2008.4.2 Wed --HYL--
1. V: 2.5.4.5
2. A:
     滤波时间调节接口               FILTER_TIME
	   
	   (0－2000us DefV: 10us)
      
	 12位数据传输模式选择接口       LE_BE_MODE
	   1：Big-Endian（默认）
	   0：Little-Endian

--2008.4.3 Thu --HYL--
1. V: 2.5.4.6
2. M: IDS_SHUTTERUNIT -> IDS_TIME_UNIT
3. A: ITEM_FILTER_TIME_UNIT
     滤波时间调节接口               FILTER_TIME
	   
	   (0－2000us DefV: 10us)
	   时间单位选择	1－ns
	   0－us（默认）
4. A:
         if (IS_SV1400(pDevice->m_pInfo->DeviceType())||
			 IS_SV1410(pDevice->m_pInfo->DeviceType())||
			 IS_SV1420(pDevice->m_pInfo->DeviceType())||
			 IS_SV2000(pDevice->m_pInfo->DeviceType())||
			 IS_SV400(pDevice->m_pInfo->DeviceType())) 
		 {
			 ListAddFilterTime(pDevice);
		 }
		
--2008.4.7 Mon --HYL--
1. V: 2.5.4.7
2. M: #include "SV1400FM.h" 中有笔误语句,已经改正 (IDS_STRING_SV1400FC)

--2008.4.10 Thu --HYL--
1. V: 2.5.4.8
2. A: 
        ptrBag->CreateBag("TransDelayValue")->WriteLong("Value",m_nTransfersDelayValue);
	//之前没有以上语句


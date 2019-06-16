# Microsoft Developer Studio Project File - Name="HVPerf_cn" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=HVPerf_cn - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HVPerf_cn.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HVPerf_cn.mak" CFG="HVPerf_cn - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HVPerf_cn - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "HVPerf_cn - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HVPerf_cn - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "..\Inc" /I "..\inc\ATL\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "STRICT" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib HVDAILT.lib Raw2Rgb.lib HVUtil.lib /nologo /subsystem:windows /machine:I386 /out:"../../..\public\hvperf_cn.exe" /libpath:"..\Lib"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "HVPerf_cn - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Inc" /I "..\inc\ATL\Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "STRICT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib HVDAILT.lib Raw2Rgb.lib HVUtil.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../../bin/hvperf_cnD.exe" /pdbtype:sept /libpath:"../Lib"

!ENDIF 

# Begin Target

# Name "HVPerf_cn - Win32 Release"
# Name "HVPerf_cn - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Camera"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\Camera\ENET.CPP
# End Source File
# Begin Source File

SOURCE=.\Camera\GIGE.CPP
# End Source File
# Begin Source File

SOURCE=.\Camera\SV2000FC.CPP
# End Source File
# End Group
# Begin Source File

SOURCE=.\aboutdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoTest.cpp
# End Source File
# Begin Source File

SOURCE=.\BusView.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Device.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceManager.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBSECTN.CPP
# End Source File
# Begin Source File

SOURCE=.\ErrorBox.cpp
# End Source File
# Begin Source File

SOURCE=.\HVBase.cpp
# End Source File
# Begin Source File

SOURCE=.\HVPerf.cpp
# End Source File
# Begin Source File

SOURCE=.\HVPerf_cn.rc
# End Source File
# Begin Source File

SOURCE=.\Image.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageView.cpp
# End Source File
# Begin Source File

SOURCE=.\mainfrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Plugin.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyBag.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyView.cpp
# End Source File
# Begin Source File

SOURCE=.\RectTracker.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\UpdatePrpView.cpp
# End Source File
# Begin Source File

SOURCE=.\utility.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Controls"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\atldock.h
# End Source File
# Begin Source File

SOURCE=.\atldock2.h
# End Source File
# Begin Source File

SOURCE=.\AtlZoom.h
# End Source File
# Begin Source File

SOURCE=.\PropertyItem.h
# End Source File
# Begin Source File

SOURCE=.\Controls\PropertyItemEditors.h
# End Source File
# Begin Source File

SOURCE=.\Controls\PropertyItemImpl.h
# End Source File
# Begin Source File

SOURCE=.\PropertyList.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\aboutdlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoTest.h
# End Source File
# Begin Source File

SOURCE=.\BusView.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\Device.h
# End Source File
# Begin Source File

SOURCE=.\DeviceManager.h
# End Source File
# Begin Source File

SOURCE=.\DialogView.h
# End Source File
# Begin Source File

SOURCE=.\DIBSECTN.H
# End Source File
# Begin Source File

SOURCE=.\ErrorBox.h
# End Source File
# Begin Source File

SOURCE=.\Exception.h
# End Source File
# Begin Source File

SOURCE=..\Inc\HVAPI.h
# End Source File
# Begin Source File

SOURCE=.\HVBase.h
# End Source File
# Begin Source File

SOURCE=.\Image.h
# End Source File
# Begin Source File

SOURCE=.\ImageView.h
# End Source File
# Begin Source File

SOURCE=.\Camera\ITS1410FC.H
# End Source File
# Begin Source File

SOURCE=.\Log.h
# End Source File
# Begin Source File

SOURCE=.\mainfrm.h
# End Source File
# Begin Source File

SOURCE=.\MDIMenuControl.h
# End Source File
# Begin Source File

SOURCE=.\Camera\PD2000EC.H
# End Source File
# Begin Source File

SOURCE=.\Plugin.h
# End Source File
# Begin Source File

SOURCE=.\PluginManager.h
# End Source File
# Begin Source File

SOURCE=.\pluginmanagerdlg.h
# End Source File
# Begin Source File

SOURCE=.\PluginSpec.h
# End Source File
# Begin Source File

SOURCE=.\PropertyView.h
# End Source File
# Begin Source File

SOURCE=.\RectTracker.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\utility.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Arrow4Way.cur
# End Source File
# Begin Source File

SOURCE=.\res\ArrowEW.cur
# End Source File
# Begin Source File

SOURCE=.\res\ArrowNE.cur
# End Source File
# Begin Source File

SOURCE=.\res\ArrowNS.cur
# End Source File
# Begin Source File

SOURCE=.\res\ArrowNW.cur
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buttons.bmp
# End Source File
# Begin Source File

SOURCE=.\res\child.ico
# End Source File
# Begin Source File

SOURCE=.\res\cross.cur
# End Source File
# Begin Source File

SOURCE=.\res\down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HVPerf_cn.exe.manifest
# End Source File
# Begin Source File

SOURCE=.\res\HVPerf_cn.ico
# End Source File
# Begin Source File

SOURCE=.\res\HVPerf_cndoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\move4way.cur
# End Source File
# Begin Source File

SOURCE=.\res\shoot03.ico
# End Source File
# Begin Source File

SOURCE=.\res\toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\up.bmp
# End Source File
# Begin Source File

SOURCE=.\res\up.ico
# End Source File
# Begin Source File

SOURCE=.\res\updown.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Video1.ico
# End Source File
# End Group
# Begin Group "Lib"

# PROP Default_Filter "lib"
# Begin Source File

SOURCE=..\Lib\htmlhelp.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\Make.bat
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section HVPerf_cn : {06FB3FB3-D800-4E08-9118-21AB84EC92FC}
# 	1:21:IDD_CAMERAFEATUREVIEW:103
# 	2:24:CLASS: CWhiteBalanceView:CWhiteBalanceView
# 	2:16:Resource Include:resource.h
# 	2:25:CLASS: CScalarFeatureView:CScalarFeatureView
# 	2:25:CLASS: CBytePerPacketView:CBytePerPacketView
# 	2:19:CameraFeatureView.h:CameraFeatureView1.h
# 	2:21:CameraFeatureView.cpp:CameraFeatureView1.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:HVPerf_cn.h
# 	2:29:CLASS: CCameraFeatureViewBase:CCameraFeatureViewBase
# 	2:21:IDD_CAMERAFEATUREVIEW:IDD_CAMERAFEATUREVIEW
# End Section
# Section HVPerf_cn : {198400C9-710C-4315-A1A9-37B796DA7C4F}
# 	1:22:IDD_FORMATPROPERTYPAGE:106
# 	2:27:CLASS: CTriggerPropertyPage:CTriggerPropertyPage
# 	2:16:Resource Include:resource.h
# 	2:24:CLASS: CInfoPropertyPage:CInfoPropertyPage
# 	2:15:PropertyDlg.cpp:PropertyDlg2.cpp
# 	2:10:ENUM: enum:enum
# 	2:28:CLASS: CAdvancedPropertyPage:CAdvancedPropertyPage
# 	2:13:PropertyDlg.h:PropertyDlg2.h
# 	2:19:CLASS: CPropertyDlg:CPropertyDlg
# 	2:19:Application Include:HVPerf_cn.h
# 	2:26:CLASS: CFormatPropertyPage:CFormatPropertyPage
# 	2:22:IDD_FORMATPROPERTYPAGE:IDD_FORMATPROPERTYPAGE
# End Section
# Section HVPerf_cn : {1EC11FFD-ED8E-4D8C-B13D-C5F23DA7673A}
# 	1:23:IDD_TRIGGERPROPERTYPAGE:107
# 	2:27:CLASS: CTriggerPropertyPage:CTriggerPropertyPage
# 	2:16:Resource Include:resource.h
# 	2:24:CLASS: CInfoPropertyPage:CInfoPropertyPage
# 	2:15:PropertyDlg.cpp:PropertyDlg3.cpp
# 	2:10:ENUM: enum:enum
# 	2:23:IDD_TRIGGERPROPERTYPAGE:IDD_TRIGGERPROPERTYPAGE
# 	2:28:CLASS: CAdvancedPropertyPage:CAdvancedPropertyPage
# 	2:13:PropertyDlg.h:PropertyDlg3.h
# 	2:19:CLASS: CPropertyDlg:CPropertyDlg
# 	2:19:Application Include:HVPerf_cn.h
# 	2:26:CLASS: CFormatPropertyPage:CFormatPropertyPage
# End Section
# Section HVPerf_cn : {AE8FE4A7-2C74-425B-AF2E-F1F8501A0B1F}
# 	1:20:IDD_INFOPROPERTYPAGE:104
# 	2:27:CLASS: CTriggerPropertyPage:CTriggerPropertyPage
# 	2:16:Resource Include:resource.h
# 	2:24:CLASS: CInfoPropertyPage:CInfoPropertyPage
# 	2:15:PropertyDlg.cpp:PropertyDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:20:IDD_INFOPROPERTYPAGE:IDD_INFOPROPERTYPAGE
# 	2:28:CLASS: CAdvancedPropertyPage:CAdvancedPropertyPage
# 	2:13:PropertyDlg.h:PropertyDlg.h
# 	2:19:CLASS: CPropertyDlg:CPropertyDlg
# 	2:19:Application Include:HVPerf_cn.h
# 	2:26:CLASS: CFormatPropertyPage:CFormatPropertyPage
# End Section
# Section HVPerf_cn : {77E90497-92FE-4661-A8EB-42969975A22D}
# 	1:24:IDD_ADVANCEDPROPERTYPAGE:105
# 	2:27:CLASS: CTriggerPropertyPage:CTriggerPropertyPage
# 	2:16:Resource Include:resource.h
# 	2:24:CLASS: CInfoPropertyPage:CInfoPropertyPage
# 	2:15:PropertyDlg.cpp:PropertyDlg1.cpp
# 	2:10:ENUM: enum:enum
# 	2:28:CLASS: CAdvancedPropertyPage:CAdvancedPropertyPage
# 	2:13:PropertyDlg.h:PropertyDlg1.h
# 	2:19:CLASS: CPropertyDlg:CPropertyDlg
# 	2:19:Application Include:HVPerf_cn.h
# 	2:26:CLASS: CFormatPropertyPage:CFormatPropertyPage
# 	2:24:IDD_ADVANCEDPROPERTYPAGE:IDD_ADVANCEDPROPERTYPAGE
# End Section
# Section HVPerf_cn : {146BC63A-2D37-4875-B511-0223144CEB77}
# 	1:12:IDD_ERRORBOX:101
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:12:ErrorBox.cpp:ErrorBox.cpp
# 	2:10:ErrorBox.h:ErrorBox.h
# 	2:12:IDD_ERRORBOX:IDD_ERRORBOX
# 	2:19:Application Include:HVPerf_cn.h
# 	2:16:CLASS: CErrorBox:CErrorBox
# End Section
# Section HVPerf_cn : {007963FA-BACB-4881-AA7C-26EDBCF15C10}
# 	1:20:IDD_WHITEBALANCEVIEW:102
# 	2:16:Resource Include:resource.h
# 	2:24:CLASS: CWhiteBalanceView:CWhiteBalanceView
# 	2:20:IDD_WHITEBALANCEVIEW:IDD_WHITEBALANCEVIEW
# 	2:25:CLASS: CScalarFeatureView:CScalarFeatureView
# 	2:19:CameraFeatureView.h:CameraFeatureView.h
# 	2:25:CLASS: CBytePerPacketView:CBytePerPacketView
# 	2:21:CameraFeatureView.cpp:CameraFeatureView.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:HVPerf_cn.h
# 	2:29:CLASS: CCameraFeatureViewBase:CCameraFeatureViewBase
# End Section

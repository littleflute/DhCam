# Microsoft Developer Studio Project File - Name="HVColor_cn" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=HVColor_cn - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HVColor_cn.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HVColor_cn.mak" CFG="HVColor_cn - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HVColor_cn - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "HVColor_cn - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HVColor_cn - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\..\..\public\bin_cn\_IConvert.plx"
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "HVColor_cn - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /out:"..\..\..\..\..\..\..\demo\bin\PlugIns\_IConvert.plx" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "HVColor_cn - Win32 Release"
# Name "HVColor_cn - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\CDib.cpp
# End Source File
# Begin Source File

SOURCE=..\ColorRender.cpp
# End Source File
# Begin Source File

SOURCE=..\ColorRender_Ex.cpp
# End Source File
# Begin Source File

SOURCE=..\CorrectMatrix.cpp
# End Source File
# Begin Source File

SOURCE=..\DlgSetParameter.cpp
# End Source File
# Begin Source File

SOURCE=..\HVColor.cpp
# End Source File
# Begin Source File

SOURCE=..\HVColor.def
# End Source File
# Begin Source File

SOURCE=..\ImageRectTracker.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageView.cpp
# End Source File
# Begin Source File

SOURCE=..\Parameter.cpp
# End Source File
# Begin Source File

SOURCE=..\Placement.cpp
# End Source File
# Begin Source File

SOURCE=..\PropertyBag.cpp
# End Source File
# Begin Source File

SOURCE=..\raw2rgb_chs.rc
# End Source File
# Begin Source File

SOURCE=..\Raw2Rgb_Plug.cpp
# End Source File
# Begin Source File

SOURCE=..\SetParamImageView.cpp
# End Source File
# Begin Source File

SOURCE=..\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\inc\CDib.h
# End Source File
# Begin Source File

SOURCE=..\ColorRender.h
# End Source File
# Begin Source File

SOURCE=..\ColorRender_Ex.h
# End Source File
# Begin Source File

SOURCE=..\CorrectMatrix.h
# End Source File
# Begin Source File

SOURCE=..\DlgSetParameter.h
# End Source File
# Begin Source File

SOURCE=..\Exception.h
# End Source File
# Begin Source File

SOURCE=..\HVColor.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\HVDAILT.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\HVDef.h
# End Source File
# Begin Source File

SOURCE=..\ImageRectTracker.h
# End Source File
# Begin Source File

SOURCE=..\ImageView.h
# End Source File
# Begin Source File

SOURCE=..\Parameter.h
# End Source File
# Begin Source File

SOURCE=..\Placement.h
# End Source File
# Begin Source File

SOURCE=..\PluginModule.h
# End Source File
# Begin Source File

SOURCE=..\PropertyBag.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\Raw2Rgb.h
# End Source File
# Begin Source File

SOURCE=..\Resource.h
# End Source File
# Begin Source File

SOURCE=..\SetParamImageView.h
# End Source File
# Begin Source File

SOURCE=..\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Arrow4Way.cur
# End Source File
# Begin Source File

SOURCE=..\res\Arrow4Way.cur
# End Source File
# Begin Source File

SOURCE=.\res\ArrowEW.cur
# End Source File
# Begin Source File

SOURCE=..\res\ArrowEW.cur
# End Source File
# Begin Source File

SOURCE=.\res\ArrowNE.cur
# End Source File
# Begin Source File

SOURCE=..\res\ArrowNE.cur
# End Source File
# Begin Source File

SOURCE=.\res\ArrowNS.cur
# End Source File
# Begin Source File

SOURCE=..\res\ArrowNS.cur
# End Source File
# Begin Source File

SOURCE=.\res\ArrowNW.cur
# End Source File
# Begin Source File

SOURCE=..\res\ArrowNW.cur
# End Source File
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\HVColor.rc2
# End Source File
# Begin Source File

SOURCE=.\res\move4way.cur
# End Source File
# Begin Source File

SOURCE=..\res\move4way.cur
# End Source File
# End Group
# Begin Source File

SOURCE=.\readme.txt
# End Source File
# Begin Source File

SOURCE=..\..\lib\HVDAILT.LIB
# End Source File
# End Target
# End Project

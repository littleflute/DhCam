# Microsoft Developer Studio Project File - Name="HVColor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=HVColor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HVColor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HVColor.mak" CFG="HVColor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HVColor - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "HVColor - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HVColor - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"Release/IDisplay.plx"

!ELSEIF  "$(CFG)" == "HVColor - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"Debug/IDisplay.plx"
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "HVColor - Win32 Release"
# Name "HVColor - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
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
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
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

SOURCE=..\resource.h
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
# End Group
# Begin Source File

SOURCE=..\..\lib\HVDAILT.LIB
# End Source File
# Begin Source File

SOURCE=..\..\lib\Raw2Rgb.lib
# End Source File
# End Target
# End Project

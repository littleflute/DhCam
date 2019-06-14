# Microsoft Developer Studio Project File - Name="MDI" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MDI - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HVMViewer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HVMViewer.mak" CFG="MDI - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MDI - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MDI - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MDI - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Fr /Yu"Stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"..\..\bin\UTest.exe"

!ELSEIF  "$(CFG)" == "MDI - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Fr /Yu"Stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\bin\UTest.exe"

!ENDIF 

# Begin Target

# Name "MDI - Win32 Release"
# Name "MDI - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\AllCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\bounce.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPrp.cpp
# End Source File
# Begin Source File

SOURCE=.\mainfrm.cpp
# End Source File
# Begin Source File

SOURCE=.\mdi.cpp
# End Source File
# Begin Source File

SOURCE=.\mdi.rc
# End Source File
# Begin Source File

SOURCE=.\mdimac.r
# End Source File
# Begin Source File

SOURCE=.\scbarg.cpp
# End Source File
# Begin Source File

SOURCE=.\sizecbar.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp

!IF  "$(CFG)" == "MDI - Win32 Release"

# ADD CPP /Yc"Stdafx.h"

!ELSEIF  "$(CFG)" == "MDI - Win32 Debug"

# ADD CPP /Yc

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WBDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\XdTreeCtrl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;inl;fi;fd"
# Begin Source File

SOURCE=.\AllCtrl.h
# End Source File
# Begin Source File

SOURCE=.\bounce.h
# End Source File
# Begin Source File

SOURCE=.\DlgPrp.h
# End Source File
# Begin Source File

SOURCE=.\mainfrm.h
# End Source File
# Begin Source File

SOURCE=.\mdi.h
# End Source File
# Begin Source File

SOURCE=.\MyCtrl.h
# End Source File
# Begin Source File

SOURCE=.\scbarg.h
# End Source File
# Begin Source File

SOURCE=.\sizecbar.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\WBDlg.h
# End Source File
# Begin Source File

SOURCE=.\XdTreeCtrl.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\hello.ico
# End Source File
# Begin Source File

SOURCE=.\mdi.ico
# End Source File
# Begin Source File

SOURCE=.\toolbar1.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=".\工程日志.txt"
# End Source File
# End Target
# End Project
# Section MDI : {6DBF05C2-4E72-4AE0-8836-5C4AD3179A69}
# 	1:10:IDD_DLG_WB:109
# 	2:16:Resource Include:resource.h
# 	2:13:CLASS: CWBDlg:CWBDlg
# 	2:10:IDD_DLG_WB:IDD_DLG_WB
# 	2:10:ENUM: enum:enum
# 	2:9:WBDlg.cpp:WBDlg.cpp
# 	2:19:Application Include:mdi.h
# 	2:7:WBDlg.h:WBDlg.h
# End Section
# Section MDI : {688DD233-C394-432B-8A08-12AD2E420D32}
# 	1:19:IDD_DIALOG_SV1300FM:106
# 	2:16:Resource Include:resource.h
# 	2:13:SV1300FMDlg.h:SV1300FMDlg.h
# 	2:19:CLASS: CSV1300FMDlg:CSV1300FMDlg
# 	2:10:ENUM: enum:enum
# 	2:19:IDD_DIALOG_SV1300FM:IDD_DIALOG_SV1300FM
# 	2:19:Application Include:mdi.h
# 	2:15:SV1300FMDlg.cpp:SV1300FMDlg.cpp
# End Section
# Section MDI : {72167389-6702-4A2E-A68D-C7AB0B2D9897}
# 	1:19:IDD_DIALOG_HV2000FC:103
# 	2:16:Resource Include:resource.h
# 	2:15:HV2000FCDlg.cpp:HV2000FCDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:13:HV2000FCDlg.h:HV2000FCDlg.h
# 	2:19:Application Include:mdi.h
# 	2:19:IDD_DIALOG_HV2000FC:IDD_DIALOG_HV2000FC
# 	2:19:CLASS: CHV2000FCDlg:CHV2000FCDlg
# End Section
# Section MDI : {FE9202E5-E5D6-4711-BF90-28B72A66B3BD}
# 	1:19:IDD_DIALOG_SV1410FC:107
# 	Section MDI : {FE9202E5-E5D6-4711-BF90-28B72A66B3BD}
# 	1:19:IDD_DIALOG_SV1410FC:107
# 	2:16:Resource Include:resource.h
# 	2:19:IDD_DIALOG_SV1410FC:IDD_DIALOG_SV1410FC
# 	2:16:SV1410FCDlgg.cpp:SV1410FCDlgg.cpp
# 	2:20:CLASS: CSV1410FCDlgg:CSV1410FCDlgg
# 	2:10:ENUM: enum:enum
# 	2:14:SV1410FCDlgg.h:SV1410FCDlgg.h
# 	2:19:Application Include:mdi.h
# End Section

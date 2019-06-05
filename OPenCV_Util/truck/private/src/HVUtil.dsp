# Microsoft Developer Studio Project File - Name="HVUtil" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=HVUtil - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HVUtil.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HVUtil.mak" CFG="HVUtil - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HVUtil - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "HVUtil - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HVUtil - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Lib"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HVUtil_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\inc" /I "..\..\inc" /I "..\..\inc\private" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HVUtil_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ddraw.lib /nologo /dll /machine:I386 /out:"..\..\Bin\HVUtil.dll"

!ELSEIF  "$(CFG)" == "HVUtil - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HVUtil_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\inc" /I "..\..\inc" /I "..\..\inc\private" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HVUtil_EXPORTS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ddraw.lib /nologo /dll /debug /machine:I386 /out:"..\Bin\HVUtil.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "HVUtil - Win32 Release"
# Name "HVUtil - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "opencv_cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\CV\BITSTRM.CPP
# End Source File
# Begin Source File

SOURCE=..\CV\CVALLOC.CPP
# End Source File
# Begin Source File

SOURCE=..\CV\CVARRAY.CPP
# End Source File
# Begin Source File

SOURCE=..\CV\CVCOLOR.CPP
# End Source File
# Begin Source File

SOURCE=..\CV\cvconvert.cpp
# End Source File
# Begin Source File

SOURCE=..\CV\CVCOPY.CPP
# End Source File
# Begin Source File

SOURCE=..\CV\cvdatastructs.cpp
# End Source File
# Begin Source File

SOURCE=..\CV\CVERROR.CPP
# End Source File
# Begin Source File

SOURCE=..\CV\CVTABLES.CPP
# End Source File
# Begin Source File

SOURCE=..\CV\grfmt_base.cpp
# End Source File
# Begin Source File

SOURCE=..\CV\grfmt_jpeg.cpp
# End Source File
# Begin Source File

SOURCE=..\CV\LOADSAVE.CPP
# End Source File
# Begin Source File

SOURCE=..\CV\UTILS.CPP
# End Source File
# End Group
# Begin Source File

SOURCE=.\HVUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\HVUtil.def
# End Source File
# Begin Source File

SOURCE=.\HVUtil.rc
# End Source File
# Begin Source File

SOURCE=.\OpenCV.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "opencv_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\CV\_CV.H
# End Source File
# Begin Source File

SOURCE=..\CV\_CVARR.H
# End Source File
# Begin Source File

SOURCE=..\CV\_cvdatastructs.h
# End Source File
# Begin Source File

SOURCE=..\CV\_CVERROR.H
# End Source File
# Begin Source File

SOURCE=..\CV\_CVFUNCN.H
# End Source File
# Begin Source File

SOURCE=..\CV\_cvmatrix.h
# End Source File
# Begin Source File

SOURCE=..\CV\_cvoptions.h
# End Source File
# Begin Source File

SOURCE=..\CV\_cvtables.h
# End Source File
# Begin Source File

SOURCE=..\CV\_CVUTILS.H
# End Source File
# Begin Source File

SOURCE=..\CV\_HIGHGUI.H
# End Source File
# Begin Source File

SOURCE=..\CV\_IPCV.H
# End Source File
# Begin Source File

SOURCE=..\CV\_OPTCV.H
# End Source File
# Begin Source File

SOURCE=..\CV\BITSTRM.H
# End Source File
# Begin Source File

SOURCE=..\CV\CV.H
# End Source File
# Begin Source File

SOURCE=..\CV\CVCOMPAT.H
# End Source File
# Begin Source File

SOURCE=..\CV\CVERROR.H
# End Source File
# Begin Source File

SOURCE=..\CV\CVTYPES.H
# End Source File
# Begin Source File

SOURCE=..\CV\grfmt_base.h
# End Source File
# Begin Source File

SOURCE=..\CV\grfmt_jpeg.h
# End Source File
# Begin Source File

SOURCE=..\CV\GRFMTS.H
# End Source File
# Begin Source File

SOURCE=..\CV\HIGHGUI.H
# End Source File
# Begin Source File

SOURCE=..\CV\UTILS.H
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Inc\HVDef.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

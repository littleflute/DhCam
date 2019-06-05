# Microsoft Developer Studio Generated NMAKE File, Based on HVDAILT.dsp
!IF "$(CFG)" == ""
CFG=HVDAILT - Win32 Debug
!MESSAGE No configuration specified. Defaulting to HVDAILT - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "HVDAILT - Win32 Release" && "$(CFG)" != "HVDAILT - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HVDAILT.mak" CFG="HVDAILT - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HVDAILT - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "HVDAILT - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HVDAILT - Win32 Release"

OUTDIR=.\..\..\common\Lib
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\..\..\common\Lib
# End Custom Macros

ALL : "..\..\Bin\Fre\HVDAILT.dll" "$(OUTDIR)\HVDAILT.bsc"


CLEAN :
	-@erase "$(INTDIR)\HVDAILT.obj"
	-@erase "$(INTDIR)\HVDAILT.pch"
	-@erase "$(INTDIR)\HVDAILT.res"
	-@erase "$(INTDIR)\HVDAILT.sbr"
	-@erase "$(INTDIR)\local.obj"
	-@erase "$(INTDIR)\local.sbr"
	-@erase "$(INTDIR)\Standard.obj"
	-@erase "$(INTDIR)\Standard.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\Util.obj"
	-@erase "$(INTDIR)\Util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\HVDAILT.bsc"
	-@erase "$(OUTDIR)\HVDAILT.exp"
	-@erase "$(OUTDIR)\HVDAILT.lib"
	-@erase "..\..\Bin\Fre\HVDAILT.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\inc" /I "..\..\inc" /I "..\..\inc\private" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HVDAILT_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\HVDAILT.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\HVDAILT.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\HVDAILT.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\HVDAILT.sbr" \
	"$(INTDIR)\local.sbr" \
	"$(INTDIR)\Standard.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\Util.sbr"

"$(OUTDIR)\HVDAILT.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\HVDAILT.pdb" /machine:I386 /def:".\HVDAILT.def" /out:"../../Bin/Fre/HVDAILT.dll" /implib:"$(OUTDIR)\HVDAILT.lib" 
DEF_FILE= \
	".\HVDAILT.def"
LINK32_OBJS= \
	"$(INTDIR)\HVDAILT.obj" \
	"$(INTDIR)\local.obj" \
	"$(INTDIR)\Standard.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Util.obj" \
	"$(INTDIR)\HVDAILT.res"

"..\..\Bin\Fre\HVDAILT.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "HVDAILT - Win32 Debug"

OUTDIR=.\..\..\common\Lib
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\..\..\common\Lib
# End Custom Macros

ALL : "..\..\Bin\Chk\HVDAILT.dll" "$(OUTDIR)\HVDAILT.bsc"


CLEAN :
	-@erase "$(INTDIR)\HVDAILT.obj"
	-@erase "$(INTDIR)\HVDAILT.pch"
	-@erase "$(INTDIR)\HVDAILT.res"
	-@erase "$(INTDIR)\HVDAILT.sbr"
	-@erase "$(INTDIR)\local.obj"
	-@erase "$(INTDIR)\local.sbr"
	-@erase "$(INTDIR)\Standard.obj"
	-@erase "$(INTDIR)\Standard.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\Util.obj"
	-@erase "$(INTDIR)\Util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\HVDAILT.bsc"
	-@erase "$(OUTDIR)\HVDAILT.exp"
	-@erase "$(OUTDIR)\HVDAILT.lib"
	-@erase "$(OUTDIR)\HVDAILT.pdb"
	-@erase "..\..\Bin\Chk\HVDAILT.dll"
	-@erase "..\..\Bin\Chk\HVDAILT.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\inc" /I "..\..\inc" /I "..\..\inc\private" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HVDAILT_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\HVDAILT.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\HVDAILT.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\HVDAILT.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\HVDAILT.sbr" \
	"$(INTDIR)\local.sbr" \
	"$(INTDIR)\Standard.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\Util.sbr"

"$(OUTDIR)\HVDAILT.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\HVDAILT.pdb" /debug /machine:I386 /def:".\HVDAILT.def" /out:"../../Bin/Chk/HVDAILT.dll" /implib:"$(OUTDIR)\HVDAILT.lib" /pdbtype:sept 
DEF_FILE= \
	".\HVDAILT.def"
LINK32_OBJS= \
	"$(INTDIR)\HVDAILT.obj" \
	"$(INTDIR)\local.obj" \
	"$(INTDIR)\Standard.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Util.obj" \
	"$(INTDIR)\HVDAILT.res"

"..\..\Bin\Chk\HVDAILT.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("HVDAILT.dep")
!INCLUDE "HVDAILT.dep"
!ELSE 
!MESSAGE Warning: cannot find "HVDAILT.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "HVDAILT - Win32 Release" || "$(CFG)" == "HVDAILT - Win32 Debug"
SOURCE=.\HVDAILT.cpp

"$(INTDIR)\HVDAILT.obj"	"$(INTDIR)\HVDAILT.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVDAILT.pch"


SOURCE=.\Res\HVDAILT.rc

!IF  "$(CFG)" == "HVDAILT - Win32 Release"


"$(INTDIR)\HVDAILT.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x804 /fo"$(INTDIR)\HVDAILT.res" /i "Res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "HVDAILT - Win32 Debug"


"$(INTDIR)\HVDAILT.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x804 /fo"$(INTDIR)\HVDAILT.res" /i "Res" /d "_DEBUG" $(SOURCE)


!ENDIF 

SOURCE=.\local.cpp

"$(INTDIR)\local.obj"	"$(INTDIR)\local.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVDAILT.pch"


SOURCE=.\Standard.cpp

"$(INTDIR)\Standard.obj"	"$(INTDIR)\Standard.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVDAILT.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "HVDAILT - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /I "..\inc" /I "..\..\inc" /I "..\..\inc\private" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HVDAILT_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\HVDAILT.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\HVDAILT.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "HVDAILT - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\inc" /I "..\..\inc" /I "..\..\inc\private" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HVDAILT_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\HVDAILT.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\HVDAILT.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Util.cpp

"$(INTDIR)\Util.obj"	"$(INTDIR)\Util.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVDAILT.pch"



!ENDIF 


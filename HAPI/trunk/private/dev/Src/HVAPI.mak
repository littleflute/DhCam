# Microsoft Developer Studio Generated NMAKE File, Based on HVAPI.dsp
!IF "$(CFG)" == ""
CFG=HVAPI - Win32 Debug
!MESSAGE No configuration specified. Defaulting to HVAPI - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "HVAPI - Win32 Release" && "$(CFG)" != "HVAPI - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HVAPI.mak" CFG="HVAPI - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HVAPI - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "HVAPI - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "HVAPI - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\HVAPI.dll" "$(OUTDIR)\HVAPI.bsc"


CLEAN :
	-@erase "$(INTDIR)\HVAPI.obj"
	-@erase "$(INTDIR)\HVAPI.pch"
	-@erase "$(INTDIR)\HVAPI.res"
	-@erase "$(INTDIR)\HVAPI.sbr"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\HVAPI.bsc"
	-@erase "$(OUTDIR)\HVAPI.dll"
	-@erase "$(OUTDIR)\HVAPI.exp"
	-@erase "$(OUTDIR)\HVAPI.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HVAPI_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\HVAPI.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\HVAPI.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\HVAPI.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\HVAPI.sbr" \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\HVAPI.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\HVAPI.pdb" /machine:I386 /def:".\HVAPI.def" /out:"$(OUTDIR)\HVAPI.dll" /implib:"$(OUTDIR)\HVAPI.lib" 
DEF_FILE= \
	".\HVAPI.def"
LINK32_OBJS= \
	"$(INTDIR)\HVAPI.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\HVAPI.res"

"$(OUTDIR)\HVAPI.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "HVAPI - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\..\..\..\test\HVAPI.dll" "$(OUTDIR)\HVAPI.bsc"


CLEAN :
	-@erase "$(INTDIR)\HVAPI.obj"
	-@erase "$(INTDIR)\HVAPI.pch"
	-@erase "$(INTDIR)\HVAPI.res"
	-@erase "$(INTDIR)\HVAPI.sbr"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\HVAPI.bsc"
	-@erase "$(OUTDIR)\HVAPI.exp"
	-@erase "$(OUTDIR)\HVAPI.lib"
	-@erase "$(OUTDIR)\HVAPI.pdb"
	-@erase "..\..\..\..\..\test\HVAPI.dll"
	-@erase "..\..\..\..\..\test\HVAPI.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HVAPI_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\HVAPI.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\HVAPI.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\HVAPI.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\HVAPI.sbr" \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\HVAPI.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\HVAPI.pdb" /debug /machine:I386 /def:".\HVAPI.def" /out:"D:\work\hvapi\test\HVAPI.dll" /implib:"$(OUTDIR)\HVAPI.lib" /pdbtype:sept 
DEF_FILE= \
	".\HVAPI.def"
LINK32_OBJS= \
	"$(INTDIR)\HVAPI.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\HVAPI.res"

"..\..\..\..\..\test\HVAPI.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("HVAPI.dep")
!INCLUDE "HVAPI.dep"
!ELSE 
!MESSAGE Warning: cannot find "HVAPI.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "HVAPI - Win32 Release" || "$(CFG)" == "HVAPI - Win32 Debug"
SOURCE=.\HVAPI.cpp

"$(INTDIR)\HVAPI.obj"	"$(INTDIR)\HVAPI.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVAPI.pch"


SOURCE=.\HVAPI.rc

"$(INTDIR)\HVAPI.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\main.cpp

"$(INTDIR)\main.obj"	"$(INTDIR)\main.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVAPI.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "HVAPI - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HVAPI_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\HVAPI.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\HVAPI.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "HVAPI - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HVAPI_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\HVAPI.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\HVAPI.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 


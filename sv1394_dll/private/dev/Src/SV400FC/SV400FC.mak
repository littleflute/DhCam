# Microsoft Developer Studio Generated NMAKE File, Based on SV400FC.dsp
!IF "$(CFG)" == ""
CFG=SV400FC - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SV400FC - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SV400FC - Win32 Release" && "$(CFG)" != "SV400FC - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SV400FC.mak" CFG="SV400FC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SV400FC - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SV400FC - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "SV400FC - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\SV400FC.dll"


CLEAN :
	-@erase "$(INTDIR)\camera.obj"
	-@erase "$(INTDIR)\CamObj.obj"
	-@erase "$(INTDIR)\feature.obj"
	-@erase "$(INTDIR)\format7.obj"
	-@erase "$(INTDIR)\isochapi.obj"
	-@erase "$(INTDIR)\local.obj"
	-@erase "$(INTDIR)\rtl.obj"
	-@erase "$(INTDIR)\SV400FC.res"
	-@erase "$(INTDIR)\trigger.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\SV400FC.dll"
	-@erase "$(OUTDIR)\SV400FC.exp"
	-@erase "$(OUTDIR)\SV400FC.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SV400FC_EXPORTS" /Fp"$(INTDIR)\SV400FC.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\SV400FC.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SV400FC.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\SV400FC.pdb" /machine:I386 /def:".\sv400fc.DEF" /out:"$(OUTDIR)\SV400FC.dll" /implib:"$(OUTDIR)\SV400FC.lib" 
DEF_FILE= \
	".\sv400fc.DEF"
LINK32_OBJS= \
	"$(INTDIR)\camera.obj" \
	"$(INTDIR)\CamObj.obj" \
	"$(INTDIR)\feature.obj" \
	"$(INTDIR)\format7.obj" \
	"$(INTDIR)\isochapi.obj" \
	"$(INTDIR)\local.obj" \
	"$(INTDIR)\rtl.obj" \
	"$(INTDIR)\trigger.obj" \
	"$(INTDIR)\SV400FC.res"

"$(OUTDIR)\SV400FC.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SV400FC - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "E:\sw_hadailt\trunk\private\Test\bin\SV400FC.dll" "$(OUTDIR)\SV400FC.bsc"


CLEAN :
	-@erase "$(INTDIR)\camera.obj"
	-@erase "$(INTDIR)\camera.sbr"
	-@erase "$(INTDIR)\CamObj.obj"
	-@erase "$(INTDIR)\CamObj.sbr"
	-@erase "$(INTDIR)\feature.obj"
	-@erase "$(INTDIR)\feature.sbr"
	-@erase "$(INTDIR)\format7.obj"
	-@erase "$(INTDIR)\format7.sbr"
	-@erase "$(INTDIR)\isochapi.obj"
	-@erase "$(INTDIR)\isochapi.sbr"
	-@erase "$(INTDIR)\local.obj"
	-@erase "$(INTDIR)\local.sbr"
	-@erase "$(INTDIR)\rtl.obj"
	-@erase "$(INTDIR)\rtl.sbr"
	-@erase "$(INTDIR)\SV400FC.res"
	-@erase "$(INTDIR)\trigger.obj"
	-@erase "$(INTDIR)\trigger.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SV400FC.bsc"
	-@erase "$(OUTDIR)\SV400FC.exp"
	-@erase "$(OUTDIR)\SV400FC.lib"
	-@erase "$(OUTDIR)\SV400FC.pdb"
	-@erase "E:\sw_hadailt\trunk\private\Test\bin\SV400FC.dll"
	-@erase "E:\sw_hadailt\trunk\private\Test\bin\SV400FC.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SV400FC_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SV400FC.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\SV400FC.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SV400FC.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\camera.sbr" \
	"$(INTDIR)\CamObj.sbr" \
	"$(INTDIR)\feature.sbr" \
	"$(INTDIR)\format7.sbr" \
	"$(INTDIR)\isochapi.sbr" \
	"$(INTDIR)\local.sbr" \
	"$(INTDIR)\rtl.sbr" \
	"$(INTDIR)\trigger.sbr"

"$(OUTDIR)\SV400FC.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\SV400FC.pdb" /debug /machine:I386 /def:".\sv400fc.DEF" /out:"E:\sw_hadailt\trunk\private\Test\bin\SV400FC.dll" /implib:"$(OUTDIR)\SV400FC.lib" /pdbtype:sept 
DEF_FILE= \
	".\sv400fc.DEF"
LINK32_OBJS= \
	"$(INTDIR)\camera.obj" \
	"$(INTDIR)\CamObj.obj" \
	"$(INTDIR)\feature.obj" \
	"$(INTDIR)\format7.obj" \
	"$(INTDIR)\isochapi.obj" \
	"$(INTDIR)\local.obj" \
	"$(INTDIR)\rtl.obj" \
	"$(INTDIR)\trigger.obj" \
	"$(INTDIR)\SV400FC.res"

"E:\sw_hadailt\trunk\private\Test\bin\SV400FC.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("SV400FC.dep")
!INCLUDE "SV400FC.dep"
!ELSE 
!MESSAGE Warning: cannot find "SV400FC.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SV400FC - Win32 Release" || "$(CFG)" == "SV400FC - Win32 Debug"
SOURCE=..\common\camera.cpp

!IF  "$(CFG)" == "SV400FC - Win32 Release"


"$(INTDIR)\camera.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SV400FC - Win32 Debug"


"$(INTDIR)\camera.obj"	"$(INTDIR)\camera.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\common\CamObj.CPP

!IF  "$(CFG)" == "SV400FC - Win32 Release"


"$(INTDIR)\CamObj.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SV400FC - Win32 Debug"


"$(INTDIR)\CamObj.obj"	"$(INTDIR)\CamObj.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\common\feature.cpp

!IF  "$(CFG)" == "SV400FC - Win32 Release"


"$(INTDIR)\feature.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SV400FC - Win32 Debug"


"$(INTDIR)\feature.obj"	"$(INTDIR)\feature.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\common\format7.cpp

!IF  "$(CFG)" == "SV400FC - Win32 Release"


"$(INTDIR)\format7.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SV400FC - Win32 Debug"


"$(INTDIR)\format7.obj"	"$(INTDIR)\format7.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\common\isochapi.cpp

!IF  "$(CFG)" == "SV400FC - Win32 Release"


"$(INTDIR)\isochapi.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SV400FC - Win32 Debug"


"$(INTDIR)\isochapi.obj"	"$(INTDIR)\isochapi.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\local.cpp

!IF  "$(CFG)" == "SV400FC - Win32 Release"


"$(INTDIR)\local.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SV400FC - Win32 Debug"


"$(INTDIR)\local.obj"	"$(INTDIR)\local.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=..\common\rtl.cpp

!IF  "$(CFG)" == "SV400FC - Win32 Release"


"$(INTDIR)\rtl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SV400FC - Win32 Debug"


"$(INTDIR)\rtl.obj"	"$(INTDIR)\rtl.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\SV400FC.rc

"$(INTDIR)\SV400FC.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=..\common\trigger.cpp

!IF  "$(CFG)" == "SV400FC - Win32 Release"


"$(INTDIR)\trigger.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SV400FC - Win32 Debug"


"$(INTDIR)\trigger.obj"	"$(INTDIR)\trigger.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 


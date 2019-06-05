# Microsoft Developer Studio Generated NMAKE File, Based on HVPerf_en.dsp
!IF "$(CFG)" == ""
CFG=HVPerf_en - Win32 Debug
!MESSAGE No configuration specified. Defaulting to HVPerf_en - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "HVPerf_en - Win32 Release" && "$(CFG)" != "HVPerf_en - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HVPerf_en.mak" CFG="HVPerf_en - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HVPerf_en - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "HVPerf_en - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\Bin\Fre\HVPerfFW_en.exe"


CLEAN :
	-@erase "$(INTDIR)\aboutdlg.obj"
	-@erase "$(INTDIR)\AutoTest.obj"
	-@erase "$(INTDIR)\BusView.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\ChkRecursion.obj"
	-@erase "$(INTDIR)\Device.obj"
	-@erase "$(INTDIR)\DeviceManager.obj"
	-@erase "$(INTDIR)\DIBSECTN.OBJ"
	-@erase "$(INTDIR)\ErrorBox.obj"
	-@erase "$(INTDIR)\HV1300FC.OBJ"
	-@erase "$(INTDIR)\HV1300FM.obj"
	-@erase "$(INTDIR)\HV1300UC.obj"
	-@erase "$(INTDIR)\HV1300UM.obj"
	-@erase "$(INTDIR)\HV1301UC.OBJ"
	-@erase "$(INTDIR)\HV1302UC.OBJ"
	-@erase "$(INTDIR)\HV1302UM.obj"
	-@erase "$(INTDIR)\HV1303UC.OBJ"
	-@erase "$(INTDIR)\HV1303UM.obj"
	-@erase "$(INTDIR)\HV2000FC.obj"
	-@erase "$(INTDIR)\HV2000UC.obj"
	-@erase "$(INTDIR)\HV2001UC.OBJ"
	-@erase "$(INTDIR)\HV2002UC.obj"
	-@erase "$(INTDIR)\HV2003UC.obj"
	-@erase "$(INTDIR)\HV3000UC.obj"
	-@erase "$(INTDIR)\HV3100FC.obj"
	-@erase "$(INTDIR)\HV3102UC.obj"
	-@erase "$(INTDIR)\HV3103UC.obj"
	-@erase "$(INTDIR)\HVBase.obj"
	-@erase "$(INTDIR)\HVPerf.obj"
	-@erase "$(INTDIR)\HVPerf_en.pch"
	-@erase "$(INTDIR)\HVPerf_en.res"
	-@erase "$(INTDIR)\Image.obj"
	-@erase "$(INTDIR)\ImageView.obj"
	-@erase "$(INTDIR)\mainfrm.obj"
	-@erase "$(INTDIR)\Plugin.obj"
	-@erase "$(INTDIR)\PluginManager.obj"
	-@erase "$(INTDIR)\PropertyBag.obj"
	-@erase "$(INTDIR)\PropertyView.obj"
	-@erase "$(INTDIR)\RectTracker.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\SV1300FC.OBJ"
	-@erase "$(INTDIR)\SV1300FM.OBJ"
	-@erase "$(INTDIR)\SV1310FC.OBJ"
	-@erase "$(INTDIR)\SV1310FM.OBJ"
	-@erase "$(INTDIR)\SV1311FC.OBJ"
	-@erase "$(INTDIR)\SV1311FM.OBJ"
	-@erase "$(INTDIR)\SV1400FC.OBJ"
	-@erase "$(INTDIR)\SV1400FM.OBJ"
	-@erase "$(INTDIR)\SV1410FC.OBJ"
	-@erase "$(INTDIR)\SV1410FM.OBJ"
	-@erase "$(INTDIR)\SV1420FC.OBJ"
	-@erase "$(INTDIR)\SV1420FM.OBJ"
	-@erase "$(INTDIR)\SV2000FC.OBJ"
	-@erase "$(INTDIR)\SV2000FM.OBJ"
	-@erase "$(INTDIR)\SV400FC.OBJ"
	-@erase "$(INTDIR)\SV400FM.OBJ"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "..\..\Bin\Fre\HVPerfFW_en.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /Zi /O2 /I "..\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "STRICT" /Fp"$(INTDIR)\HVPerf_en.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\HVPerf_en.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\HVPerf_en.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\HVPerfFW_en.pdb" /machine:I386 /out:"..\..\Bin\Fre\HVPerfFW_en.exe" 
LINK32_OBJS= \
	"$(INTDIR)\HV1300FC.OBJ" \
	"$(INTDIR)\HV1300FM.obj" \
	"$(INTDIR)\HV1300UC.obj" \
	"$(INTDIR)\HV1300UM.obj" \
	"$(INTDIR)\HV1301UC.OBJ" \
	"$(INTDIR)\HV1302UC.OBJ" \
	"$(INTDIR)\HV1302UM.obj" \
	"$(INTDIR)\HV1303UC.OBJ" \
	"$(INTDIR)\HV1303UM.obj" \
	"$(INTDIR)\HV2000FC.obj" \
	"$(INTDIR)\HV2000UC.obj" \
	"$(INTDIR)\HV2001UC.OBJ" \
	"$(INTDIR)\HV2002UC.obj" \
	"$(INTDIR)\HV2003UC.obj" \
	"$(INTDIR)\HV3000UC.obj" \
	"$(INTDIR)\HV3100FC.obj" \
	"$(INTDIR)\HV3102UC.obj" \
	"$(INTDIR)\HV3103UC.obj" \
	"$(INTDIR)\SV1300FC.OBJ" \
	"$(INTDIR)\SV1300FM.OBJ" \
	"$(INTDIR)\SV1310FC.OBJ" \
	"$(INTDIR)\SV1310FM.OBJ" \
	"$(INTDIR)\SV1311FC.OBJ" \
	"$(INTDIR)\SV1311FM.OBJ" \
	"$(INTDIR)\SV1400FC.OBJ" \
	"$(INTDIR)\SV1400FM.OBJ" \
	"$(INTDIR)\SV1410FC.OBJ" \
	"$(INTDIR)\SV1410FM.OBJ" \
	"$(INTDIR)\SV1420FC.OBJ" \
	"$(INTDIR)\SV1420FM.OBJ" \
	"$(INTDIR)\SV2000FC.OBJ" \
	"$(INTDIR)\SV2000FM.OBJ" \
	"$(INTDIR)\SV400FC.OBJ" \
	"$(INTDIR)\SV400FM.OBJ" \
	"$(INTDIR)\aboutdlg.obj" \
	"$(INTDIR)\AutoTest.obj" \
	"$(INTDIR)\BusView.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\ChkRecursion.obj" \
	"$(INTDIR)\Device.obj" \
	"$(INTDIR)\DeviceManager.obj" \
	"$(INTDIR)\DIBSECTN.OBJ" \
	"$(INTDIR)\ErrorBox.obj" \
	"$(INTDIR)\HVBase.obj" \
	"$(INTDIR)\HVPerf.obj" \
	"$(INTDIR)\Image.obj" \
	"$(INTDIR)\ImageView.obj" \
	"$(INTDIR)\mainfrm.obj" \
	"$(INTDIR)\Plugin.obj" \
	"$(INTDIR)\PluginManager.obj" \
	"$(INTDIR)\PropertyBag.obj" \
	"$(INTDIR)\PropertyView.obj" \
	"$(INTDIR)\RectTracker.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\utility.obj" \
	"$(INTDIR)\HVPerf_en.res" \
	"..\Lib\HVDAILT.LIB" \
	"..\Lib\HVUtil.lib" \
	"..\Lib\Raw2Rgb.lib" \
	"..\Lib\htmlhelp.lib"

"..\..\Bin\Fre\HVPerfFW_en.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\Bin_en\HVPerfFW.exe" "$(OUTDIR)\HVPerf_en.bsc"


CLEAN :
	-@erase "$(INTDIR)\aboutdlg.obj"
	-@erase "$(INTDIR)\aboutdlg.sbr"
	-@erase "$(INTDIR)\AutoTest.obj"
	-@erase "$(INTDIR)\AutoTest.sbr"
	-@erase "$(INTDIR)\BusView.obj"
	-@erase "$(INTDIR)\BusView.sbr"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\ChildFrm.sbr"
	-@erase "$(INTDIR)\ChkRecursion.obj"
	-@erase "$(INTDIR)\ChkRecursion.sbr"
	-@erase "$(INTDIR)\Device.obj"
	-@erase "$(INTDIR)\Device.sbr"
	-@erase "$(INTDIR)\DeviceManager.obj"
	-@erase "$(INTDIR)\DeviceManager.sbr"
	-@erase "$(INTDIR)\DIBSECTN.OBJ"
	-@erase "$(INTDIR)\DIBSECTN.SBR"
	-@erase "$(INTDIR)\ErrorBox.obj"
	-@erase "$(INTDIR)\ErrorBox.sbr"
	-@erase "$(INTDIR)\HV1300FC.OBJ"
	-@erase "$(INTDIR)\HV1300FC.SBR"
	-@erase "$(INTDIR)\HV1300FM.obj"
	-@erase "$(INTDIR)\HV1300FM.sbr"
	-@erase "$(INTDIR)\HV1300UC.obj"
	-@erase "$(INTDIR)\HV1300UC.sbr"
	-@erase "$(INTDIR)\HV1300UM.obj"
	-@erase "$(INTDIR)\HV1300UM.sbr"
	-@erase "$(INTDIR)\HV1301UC.OBJ"
	-@erase "$(INTDIR)\HV1301UC.SBR"
	-@erase "$(INTDIR)\HV1302UC.OBJ"
	-@erase "$(INTDIR)\HV1302UC.SBR"
	-@erase "$(INTDIR)\HV1302UM.obj"
	-@erase "$(INTDIR)\HV1302UM.sbr"
	-@erase "$(INTDIR)\HV1303UC.OBJ"
	-@erase "$(INTDIR)\HV1303UC.SBR"
	-@erase "$(INTDIR)\HV1303UM.obj"
	-@erase "$(INTDIR)\HV1303UM.sbr"
	-@erase "$(INTDIR)\HV2000FC.obj"
	-@erase "$(INTDIR)\HV2000FC.sbr"
	-@erase "$(INTDIR)\HV2000UC.obj"
	-@erase "$(INTDIR)\HV2000UC.sbr"
	-@erase "$(INTDIR)\HV2001UC.OBJ"
	-@erase "$(INTDIR)\HV2001UC.SBR"
	-@erase "$(INTDIR)\HV2002UC.obj"
	-@erase "$(INTDIR)\HV2002UC.sbr"
	-@erase "$(INTDIR)\HV2003UC.obj"
	-@erase "$(INTDIR)\HV2003UC.sbr"
	-@erase "$(INTDIR)\HV3000UC.obj"
	-@erase "$(INTDIR)\HV3000UC.sbr"
	-@erase "$(INTDIR)\HV3100FC.obj"
	-@erase "$(INTDIR)\HV3100FC.sbr"
	-@erase "$(INTDIR)\HV3102UC.obj"
	-@erase "$(INTDIR)\HV3102UC.sbr"
	-@erase "$(INTDIR)\HV3103UC.obj"
	-@erase "$(INTDIR)\HV3103UC.sbr"
	-@erase "$(INTDIR)\HVBase.obj"
	-@erase "$(INTDIR)\HVBase.sbr"
	-@erase "$(INTDIR)\HVPerf.obj"
	-@erase "$(INTDIR)\HVPerf.sbr"
	-@erase "$(INTDIR)\HVPerf_en.pch"
	-@erase "$(INTDIR)\HVPerf_en.res"
	-@erase "$(INTDIR)\Image.obj"
	-@erase "$(INTDIR)\Image.sbr"
	-@erase "$(INTDIR)\ImageView.obj"
	-@erase "$(INTDIR)\ImageView.sbr"
	-@erase "$(INTDIR)\mainfrm.obj"
	-@erase "$(INTDIR)\mainfrm.sbr"
	-@erase "$(INTDIR)\Plugin.obj"
	-@erase "$(INTDIR)\Plugin.sbr"
	-@erase "$(INTDIR)\PluginManager.obj"
	-@erase "$(INTDIR)\PluginManager.sbr"
	-@erase "$(INTDIR)\PropertyBag.obj"
	-@erase "$(INTDIR)\PropertyBag.sbr"
	-@erase "$(INTDIR)\PropertyView.obj"
	-@erase "$(INTDIR)\PropertyView.sbr"
	-@erase "$(INTDIR)\RectTracker.obj"
	-@erase "$(INTDIR)\RectTracker.sbr"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\stdafx.sbr"
	-@erase "$(INTDIR)\SV1300FC.OBJ"
	-@erase "$(INTDIR)\SV1300FC.SBR"
	-@erase "$(INTDIR)\SV1300FM.OBJ"
	-@erase "$(INTDIR)\SV1300FM.SBR"
	-@erase "$(INTDIR)\SV1310FC.OBJ"
	-@erase "$(INTDIR)\SV1310FC.SBR"
	-@erase "$(INTDIR)\SV1310FM.OBJ"
	-@erase "$(INTDIR)\SV1310FM.SBR"
	-@erase "$(INTDIR)\SV1311FC.OBJ"
	-@erase "$(INTDIR)\SV1311FC.SBR"
	-@erase "$(INTDIR)\SV1311FM.OBJ"
	-@erase "$(INTDIR)\SV1311FM.SBR"
	-@erase "$(INTDIR)\SV1400FC.OBJ"
	-@erase "$(INTDIR)\SV1400FC.SBR"
	-@erase "$(INTDIR)\SV1400FM.OBJ"
	-@erase "$(INTDIR)\SV1400FM.SBR"
	-@erase "$(INTDIR)\SV1410FC.OBJ"
	-@erase "$(INTDIR)\SV1410FC.SBR"
	-@erase "$(INTDIR)\SV1410FM.OBJ"
	-@erase "$(INTDIR)\SV1410FM.SBR"
	-@erase "$(INTDIR)\SV1420FC.OBJ"
	-@erase "$(INTDIR)\SV1420FC.SBR"
	-@erase "$(INTDIR)\SV1420FM.OBJ"
	-@erase "$(INTDIR)\SV1420FM.SBR"
	-@erase "$(INTDIR)\SV2000FC.OBJ"
	-@erase "$(INTDIR)\SV2000FC.SBR"
	-@erase "$(INTDIR)\SV2000FM.OBJ"
	-@erase "$(INTDIR)\SV2000FM.SBR"
	-@erase "$(INTDIR)\SV400FC.OBJ"
	-@erase "$(INTDIR)\SV400FC.SBR"
	-@erase "$(INTDIR)\SV400FM.OBJ"
	-@erase "$(INTDIR)\SV400FM.SBR"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\utility.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\HVPerf_en.bsc"
	-@erase "$(OUTDIR)\HVPerfFW.pdb"
	-@erase "..\Bin_en\HVPerfFW.exe"
	-@erase "..\Bin_en\HVPerfFW.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "STRICT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\HVPerf_en.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\HVPerf_en.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\HVPerf_en.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\HV1300FC.SBR" \
	"$(INTDIR)\HV1300FM.sbr" \
	"$(INTDIR)\HV1300UC.sbr" \
	"$(INTDIR)\HV1300UM.sbr" \
	"$(INTDIR)\HV1301UC.SBR" \
	"$(INTDIR)\HV1302UC.SBR" \
	"$(INTDIR)\HV1302UM.sbr" \
	"$(INTDIR)\HV1303UC.SBR" \
	"$(INTDIR)\HV1303UM.sbr" \
	"$(INTDIR)\HV2000FC.sbr" \
	"$(INTDIR)\HV2000UC.sbr" \
	"$(INTDIR)\HV2001UC.SBR" \
	"$(INTDIR)\HV2002UC.sbr" \
	"$(INTDIR)\HV2003UC.sbr" \
	"$(INTDIR)\HV3000UC.sbr" \
	"$(INTDIR)\HV3100FC.sbr" \
	"$(INTDIR)\HV3102UC.sbr" \
	"$(INTDIR)\HV3103UC.sbr" \
	"$(INTDIR)\SV1300FC.SBR" \
	"$(INTDIR)\SV1300FM.SBR" \
	"$(INTDIR)\SV1310FC.SBR" \
	"$(INTDIR)\SV1310FM.SBR" \
	"$(INTDIR)\SV1311FC.SBR" \
	"$(INTDIR)\SV1311FM.SBR" \
	"$(INTDIR)\SV1400FC.SBR" \
	"$(INTDIR)\SV1400FM.SBR" \
	"$(INTDIR)\SV1410FC.SBR" \
	"$(INTDIR)\SV1410FM.SBR" \
	"$(INTDIR)\SV1420FC.SBR" \
	"$(INTDIR)\SV1420FM.SBR" \
	"$(INTDIR)\SV2000FC.SBR" \
	"$(INTDIR)\SV2000FM.SBR" \
	"$(INTDIR)\SV400FC.SBR" \
	"$(INTDIR)\SV400FM.SBR" \
	"$(INTDIR)\aboutdlg.sbr" \
	"$(INTDIR)\AutoTest.sbr" \
	"$(INTDIR)\BusView.sbr" \
	"$(INTDIR)\ChildFrm.sbr" \
	"$(INTDIR)\ChkRecursion.sbr" \
	"$(INTDIR)\Device.sbr" \
	"$(INTDIR)\DeviceManager.sbr" \
	"$(INTDIR)\DIBSECTN.SBR" \
	"$(INTDIR)\ErrorBox.sbr" \
	"$(INTDIR)\HVBase.sbr" \
	"$(INTDIR)\HVPerf.sbr" \
	"$(INTDIR)\Image.sbr" \
	"$(INTDIR)\ImageView.sbr" \
	"$(INTDIR)\mainfrm.sbr" \
	"$(INTDIR)\Plugin.sbr" \
	"$(INTDIR)\PluginManager.sbr" \
	"$(INTDIR)\PropertyBag.sbr" \
	"$(INTDIR)\PropertyView.sbr" \
	"$(INTDIR)\RectTracker.sbr" \
	"$(INTDIR)\stdafx.sbr" \
	"$(INTDIR)\utility.sbr"

"$(OUTDIR)\HVPerf_en.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\HVPerfFW.pdb" /debug /machine:I386 /out:"..\Bin_en\HVPerfFW.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\HV1300FC.OBJ" \
	"$(INTDIR)\HV1300FM.obj" \
	"$(INTDIR)\HV1300UC.obj" \
	"$(INTDIR)\HV1300UM.obj" \
	"$(INTDIR)\HV1301UC.OBJ" \
	"$(INTDIR)\HV1302UC.OBJ" \
	"$(INTDIR)\HV1302UM.obj" \
	"$(INTDIR)\HV1303UC.OBJ" \
	"$(INTDIR)\HV1303UM.obj" \
	"$(INTDIR)\HV2000FC.obj" \
	"$(INTDIR)\HV2000UC.obj" \
	"$(INTDIR)\HV2001UC.OBJ" \
	"$(INTDIR)\HV2002UC.obj" \
	"$(INTDIR)\HV2003UC.obj" \
	"$(INTDIR)\HV3000UC.obj" \
	"$(INTDIR)\HV3100FC.obj" \
	"$(INTDIR)\HV3102UC.obj" \
	"$(INTDIR)\HV3103UC.obj" \
	"$(INTDIR)\SV1300FC.OBJ" \
	"$(INTDIR)\SV1300FM.OBJ" \
	"$(INTDIR)\SV1310FC.OBJ" \
	"$(INTDIR)\SV1310FM.OBJ" \
	"$(INTDIR)\SV1311FC.OBJ" \
	"$(INTDIR)\SV1311FM.OBJ" \
	"$(INTDIR)\SV1400FC.OBJ" \
	"$(INTDIR)\SV1400FM.OBJ" \
	"$(INTDIR)\SV1410FC.OBJ" \
	"$(INTDIR)\SV1410FM.OBJ" \
	"$(INTDIR)\SV1420FC.OBJ" \
	"$(INTDIR)\SV1420FM.OBJ" \
	"$(INTDIR)\SV2000FC.OBJ" \
	"$(INTDIR)\SV2000FM.OBJ" \
	"$(INTDIR)\SV400FC.OBJ" \
	"$(INTDIR)\SV400FM.OBJ" \
	"$(INTDIR)\aboutdlg.obj" \
	"$(INTDIR)\AutoTest.obj" \
	"$(INTDIR)\BusView.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\ChkRecursion.obj" \
	"$(INTDIR)\Device.obj" \
	"$(INTDIR)\DeviceManager.obj" \
	"$(INTDIR)\DIBSECTN.OBJ" \
	"$(INTDIR)\ErrorBox.obj" \
	"$(INTDIR)\HVBase.obj" \
	"$(INTDIR)\HVPerf.obj" \
	"$(INTDIR)\Image.obj" \
	"$(INTDIR)\ImageView.obj" \
	"$(INTDIR)\mainfrm.obj" \
	"$(INTDIR)\Plugin.obj" \
	"$(INTDIR)\PluginManager.obj" \
	"$(INTDIR)\PropertyBag.obj" \
	"$(INTDIR)\PropertyView.obj" \
	"$(INTDIR)\RectTracker.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\utility.obj" \
	"$(INTDIR)\HVPerf_en.res" \
	"..\Lib\HVDAILT.LIB" \
	"..\Lib\HVUtil.lib" \
	"..\Lib\Raw2Rgb.lib" \
	"..\Lib\htmlhelp.lib"

"..\Bin_en\HVPerfFW.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("HVPerf_en.dep")
!INCLUDE "HVPerf_en.dep"
!ELSE 
!MESSAGE Warning: cannot find "HVPerf_en.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "HVPerf_en - Win32 Release" || "$(CFG)" == "HVPerf_en - Win32 Debug"
SOURCE=.\Camera\HV1300FC.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\HV1300FC.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\HV1300FC.OBJ"	"$(INTDIR)\HV1300FC.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\HV1300FM.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\HV1300FM.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\HV1300FM.obj"	"$(INTDIR)\HV1300FM.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\HV1300UC.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\HV1300UC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\HV1300UC.obj"	"$(INTDIR)\HV1300UC.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\HV1300UM.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\HV1300UM.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\HV1300UM.obj"	"$(INTDIR)\HV1300UM.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\HV1301UC.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\HV1301UC.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\HV1301UC.OBJ"	"$(INTDIR)\HV1301UC.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\HV1302UC.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\HV1302UC.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\HV1302UC.OBJ"	"$(INTDIR)\HV1302UC.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\HV1302UM.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\HV1302UM.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\HV1302UM.obj"	"$(INTDIR)\HV1302UM.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\HV1303UC.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\HV1303UC.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\HV1303UC.OBJ"	"$(INTDIR)\HV1303UC.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\HV1303UM.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\HV1303UM.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\HV1303UM.obj"	"$(INTDIR)\HV1303UM.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\HV2000FC.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\HV2000FC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\HV2000FC.obj"	"$(INTDIR)\HV2000FC.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\HV2000UC.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\HV2000UC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\HV2000UC.obj"	"$(INTDIR)\HV2000UC.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\HV2001UC.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\HV2001UC.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\HV2001UC.OBJ"	"$(INTDIR)\HV2001UC.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\HV2002UC.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\HV2002UC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\HV2002UC.obj"	"$(INTDIR)\HV2002UC.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\HV2003UC.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\HV2003UC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\HV2003UC.obj"	"$(INTDIR)\HV2003UC.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\HV3000UC.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\HV3000UC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\HV3000UC.obj"	"$(INTDIR)\HV3000UC.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\HV3100FC.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\HV3100FC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\HV3100FC.obj"	"$(INTDIR)\HV3100FC.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\HV3102UC.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\HV3102UC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\HV3102UC.obj"	"$(INTDIR)\HV3102UC.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\HV3103UC.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\HV3103UC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\HV3103UC.obj"	"$(INTDIR)\HV3103UC.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\SV1300FC.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\SV1300FC.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\SV1300FC.OBJ"	"$(INTDIR)\SV1300FC.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\SV1300FM.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\SV1300FM.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\SV1300FM.OBJ"	"$(INTDIR)\SV1300FM.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\SV1310FC.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\SV1310FC.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\SV1310FC.OBJ"	"$(INTDIR)\SV1310FC.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\SV1310FM.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\SV1310FM.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\SV1310FM.OBJ"	"$(INTDIR)\SV1310FM.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\SV1311FC.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\SV1311FC.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\SV1311FC.OBJ"	"$(INTDIR)\SV1311FC.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\SV1311FM.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\SV1311FM.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\SV1311FM.OBJ"	"$(INTDIR)\SV1311FM.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\SV1400FC.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\SV1400FC.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\SV1400FC.OBJ"	"$(INTDIR)\SV1400FC.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\SV1400FM.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\SV1400FM.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\SV1400FM.OBJ"	"$(INTDIR)\SV1400FM.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\SV1410FC.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\SV1410FC.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\SV1410FC.OBJ"	"$(INTDIR)\SV1410FC.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\SV1410FM.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\SV1410FM.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\SV1410FM.OBJ"	"$(INTDIR)\SV1410FM.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\SV1420FC.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\SV1420FC.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\SV1420FC.OBJ"	"$(INTDIR)\SV1420FC.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\SV1420FM.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\SV1420FM.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\SV1420FM.OBJ"	"$(INTDIR)\SV1420FM.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\SV2000FC.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\SV2000FC.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\SV2000FC.OBJ"	"$(INTDIR)\SV2000FC.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\SV2000FM.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\SV2000FM.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\SV2000FM.OBJ"	"$(INTDIR)\SV2000FM.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\SV400FC.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\SV400FC.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\SV400FC.OBJ"	"$(INTDIR)\SV400FC.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Camera\SV400FM.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\SV400FM.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\SV400FM.OBJ"	"$(INTDIR)\SV400FM.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\aboutdlg.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\aboutdlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\aboutdlg.obj"	"$(INTDIR)\aboutdlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ENDIF 

SOURCE=.\AutoTest.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\AutoTest.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\AutoTest.obj"	"$(INTDIR)\AutoTest.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ENDIF 

SOURCE=.\BusView.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\BusView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\BusView.obj"	"$(INTDIR)\BusView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ENDIF 

SOURCE=.\ChildFrm.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\ChildFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\ChildFrm.obj"	"$(INTDIR)\ChildFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ENDIF 

SOURCE=.\ChkRecursion.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /Zi /O2 /I "..\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "STRICT" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ChkRecursion.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "STRICT" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ChkRecursion.obj"	"$(INTDIR)\ChkRecursion.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Device.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\Device.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\Device.obj"	"$(INTDIR)\Device.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ENDIF 

SOURCE=.\DeviceManager.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\DeviceManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\DeviceManager.obj"	"$(INTDIR)\DeviceManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ENDIF 

SOURCE=.\DIBSECTN.CPP

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\DIBSECTN.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\DIBSECTN.OBJ"	"$(INTDIR)\DIBSECTN.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ENDIF 

SOURCE=.\ErrorBox.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\ErrorBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\ErrorBox.obj"	"$(INTDIR)\ErrorBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ENDIF 

SOURCE=.\HVBase.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\HVBase.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\HVBase.obj"	"$(INTDIR)\HVBase.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ENDIF 

SOURCE=.\HVPerf.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\HVPerf.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\HVPerf.obj"	"$(INTDIR)\HVPerf.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ENDIF 

SOURCE=.\HVPerf_en.rc

"$(INTDIR)\HVPerf_en.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\Image.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\Image.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\Image.obj"	"$(INTDIR)\Image.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ENDIF 

SOURCE=.\ImageView.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\ImageView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\ImageView.obj"	"$(INTDIR)\ImageView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ENDIF 

SOURCE=.\mainfrm.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\mainfrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\mainfrm.obj"	"$(INTDIR)\mainfrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ENDIF 

SOURCE=.\Plugin.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\Plugin.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\Plugin.obj"	"$(INTDIR)\Plugin.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ENDIF 

SOURCE=.\PluginManager.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\PluginManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\PluginManager.obj"	"$(INTDIR)\PluginManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ENDIF 

SOURCE=.\PropertyBag.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\PropertyBag.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\PropertyBag.obj"	"$(INTDIR)\PropertyBag.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ENDIF 

SOURCE=.\PropertyView.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\PropertyView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\PropertyView.obj"	"$(INTDIR)\PropertyView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ENDIF 

SOURCE=.\RectTracker.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\RectTracker.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\RectTracker.obj"	"$(INTDIR)\RectTracker.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ENDIF 

SOURCE=.\stdafx.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /Zi /O2 /I "..\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "STRICT" /Fp"$(INTDIR)\HVPerf_en.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\stdafx.obj"	"$(INTDIR)\HVPerf_en.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "STRICT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\HVPerf_en.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\stdafx.obj"	"$(INTDIR)\stdafx.sbr"	"$(INTDIR)\HVPerf_en.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\utility.cpp

!IF  "$(CFG)" == "HVPerf_en - Win32 Release"


"$(INTDIR)\utility.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ELSEIF  "$(CFG)" == "HVPerf_en - Win32 Debug"


"$(INTDIR)\utility.obj"	"$(INTDIR)\utility.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\HVPerf_en.pch"


!ENDIF 


!ENDIF 


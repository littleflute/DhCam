@echo off
::-----------------------------------------------
::[mk-bin-080905.bat]说明:
::本文件为使用"F系列用户光盘080905"中发布的驱动版本测试SV400FC摄像机而制作的批处理文件,可从SVN上自动Export相关文件
::HVPerf_cn.exe 2.5.4.8;HVDAILT.dll 3.0.3.1;HVAPI.dll 1.1.2.1;SV400FC.dll 1.2.5.1;SV400FC.sys 2.0.0.1  
:: 2009.2.5
::-----------------------------------------------

@echo 用户文件生成开始

rem 先清空目录!
for /r %%a in ( bin ) do  rd /s /q %%a
Title 用户文件生成，正在进行...


::批处理命令开始-----------------------------------
set SVN_HOME=E:\svn
set path=%path%;%SVN_HOME%\bin
set LANG=zh_CN.UTF8
set APR_ICONV_PATH=%SVN_HOME%\iconv 


set HVPERF="http://192.168.10.30/svn/sw_demo/hvperf/tag/public/v2.5.4.8"
set IDISPLAY="http://192.168.10.30/svn/sw_plugins/IDisplay/tag/public/v1.6.2.1"
set LUT="http://192.168.10.30/svn/st_hvapi2/tag/public/LUT"
set HVDAILT="http://192.168.10.30/svn/sw_module_interface/hvdailt/tag/public/v3.0.3.1"
set HVAPI="http://192.168.10.30/svn/sw_module_interface/hvapi/tag/public/v1.1.2.1"
set SVFX="http://192.168.10.30/svn/sw_module_interface/sv1394serial/tag/public/v1.2.5.1"
set SYS="http://192.168.10.30/svn/sw_driver/1394sys/tag/public/v2.0.0.1"
set Raw2RGB="http://192.168.10.30/svn/st_hvapi2/tag/public/Raw2rgb/v1.7.3.0"
set HVUTIL="http://192.168.10.30/svn/sw_module_interface/hvdailt/trunk/private/ref/ALD2DH/080530test"

svn export %HVPERF% 	HVPERF
svn export %IDISPLAY% 	IDISPLAY
svn export %LUT% 	LUT0
svn export %HVDAILT% 	HVDAILT
svn export %HVAPI% 	HVAPI
svn export %SVFX%  	SVFX 
svn export %SYS%  	SYS  
svn export %Raw2RGB%	Raw2RGB
svn export %HVUTIL%	HVUTIL


rem 建立目录!
MD bin
MD bin\PlugIns
MD bin\Lut

copy HVPERF\HVPerf_cn.exe 		bin\HVPerf_cn.exe
copy IDISPLAY\bin_cn\IDISPLAY.plx	bin\PlugIns\IDISPLAY.plx
copy LUT0\*.*				bin\LUT\*.*
copy HVDAILT\HVDAILT.dll 		bin\HVDAILT.dll
copy HVAPI\HVAPI.dll 			bin\HVAPI.dll
copy SVFX\SV400FC.dll			bin\SV400FC.dll
copy SYS\SV400FC.sys			bin\SV400FC.sys
copy SYS\SV400FC.inf			bin\SV400FC.inf

copy Raw2RGB\Raw2RGb.dll		bin\Raw2RGB.dll
copy HVUTIL\HVUtil.dll			bin\HVUtil.dll


 
rem rmdir temp /s /q
 
rem 最后清空目录!
  for /r %%a in ( HVPERF HVDAILT HVAPI SVFX SYS Raw2RGB HVUTIL IDISPLAY LUT0) do  rd /s /q %%a


   
@echo 用户文件生成，完成
::批处理命令结束-----------------------------------
rem pause

@echo on

@echo off
::-----------------------------------------------
::注释内容
::-----------------------------------------------

@echo 用户文件生成开始

Title 用户文件生成，正在进行...


::批处理命令开始-----------------------------------
set SVN_HOME=F:\xd\exe\svn-win32-1.4.6
set path=%path%;%SVN_HOME%\bin
set LANG=zh_CN.UTF8
set APR_ICONV_PATH=%SVN_HOME%\iconv
set SVNROOT="http://192.168.0.18/svn/st_hvapi2/branches/HvTest/HvApiTest"
set LOCALROOT="sdk_sample" 

::建立目录
rem mkdir %LOCALROOT% 

::生成bin的内容
svn export %SVNROOT%/private/VC/Inc ..\HvApiTest_BK


 
rem rmdir temp /s /q
 

   
@echo 用户文件生成，完成
::批处理命令结束-----------------------------------
pause

@echo on

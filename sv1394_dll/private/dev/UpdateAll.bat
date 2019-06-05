@echo off
 
  
set mypath=
cd > CurPath.txt 
for /F %%a in (CurPath.txt) do (
  set mypath=%%a
) 

for /F %%a in (MakeAll.txt) do (
  rem echo %mypath%\%%a.rc  
  echo %mypath%\Src\%%a\%%a.rc >RC1.txt 
  echo %mypath%\Src\%%a\%%a.r  >RC2.txt


  call ATools Version1.txt Version2.txt RC1.txt RC2.txt 
  call ATools V11.txt V22.txt RC1.txt RC2.txt 

)
del CurPath.txt
del RC1.txt
del RC2.txt
 
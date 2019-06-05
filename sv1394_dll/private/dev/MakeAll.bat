@echo off
echo to make all sv1394 serial Dlls......
for /F %%a in (MakeAll.txt) do (
  echo %%a
  cd Src\%%a
  call make.bat
  cd..
  cd..
)
echo ----------------Finished!------------------
 
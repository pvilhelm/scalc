@echo off
set pth=%CD:~2,99%
set pth=%pth:\=/%
set drv=%CD:~0,1%
set "fullpath=/cygdrive/%drv%%pth%"
bash.exe --login -i -c "cd ""%fullpath%""; pwd; ./build.sh"
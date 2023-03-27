SET PATH=%PATH%;%cd%\MinGW\bin
gcc BC6X4X.c src/kernel_loader.c -o BC6X4X.exe -lOpenCL -Iinclude && BC6X4X.exe ./test/ 2 1
cmd
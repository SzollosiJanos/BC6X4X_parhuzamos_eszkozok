SET PATH=%PATH%;%cd%\MinGW\bin
gcc BC6X4X.c -o BC6X4X.exe -lOpenCL && BC6X4X.exe ./test/ 2 1
cmd
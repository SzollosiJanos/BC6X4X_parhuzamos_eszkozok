SET PATH=%PATH%;%cd%\MinGW\bin
gcc BC6X4X_normal.c -o BC6X4X_normal.exe -lOpenCL && BC6X4X_normal.exe ./test/ 2 2
cmd
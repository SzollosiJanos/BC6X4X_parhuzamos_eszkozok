all:
	gcc src/BC6X4X.c src/kernel_loader.c src/BC6X4X_helper.c -o BC6X4X.exe -lOpenCL -Iinclude
	gcc src/BC6X4X_normal.c src/kernel_loader.c src/BC6X4X_helper.c -o BC6X4X_normal.exe -lOpenCL -Iinclude
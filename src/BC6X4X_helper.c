#include "BC6X4X_helper.h"

int init_kernel(cl_int *err,cl_uint *n_platforms,cl_platform_id *platform_id,cl_device_id *device_id,cl_context * context,cl_program* program){
	*err = clGetPlatformIDs(1, platform_id, n_platforms);
    if (*err != CL_SUCCESS) {
        printf("[ERROR] Error calling clGetPlatformIDs. Error code: %d\n", *err);
        return -1;
    }
    cl_uint n_devices;
    *err = clGetDeviceIDs(*platform_id, CL_DEVICE_TYPE_GPU, 1, device_id, & n_devices);
    if (*err != CL_SUCCESS) {
        printf("[ERROR] Error calling clGetDeviceIDs. Error code: %d\n", *err);
        return -1;
    }
	
    *context = clCreateContext(NULL, n_devices, device_id, NULL, NULL, NULL);
    // Create the kernel program and build it
	int error;
    const char * kernel_code = load_kernel_source("kernels/BC6X4X_kernel.cl", &error);
    if (error != 0) {
        printf("Source code loading error!\n");
        return -1;
    }
    *program = clCreateProgramWithSource(*context, 1, & kernel_code, NULL, NULL);
    *err = clBuildProgram(*program, 0, NULL, NULL, NULL, NULL);
    if (*err != CL_SUCCESS) {
        printf("Build error! Code: %d\n", *err);
        return -1;
    }
	return 0;
}



void encrypt(char * buffer, int size, int key) {
    for (int i = 0; i < size; i++) {
        if (key == 1) {
            buffer[i] += 13;
        } else if (key == 2) {
            buffer[i] += i;
        }
    }
}

void decrypt(char * buffer, int size, int key) {
    for (int i = 0; i < size; i++) {
        if (key == 1) {
            buffer[i] -= 13;
        } else if (key == 2) {
            buffer[i] -= i;
        }
    }
}

int test_and_write_filename(char* path,int mode,FILE ** output,FILE ** entry_file){
	char inputname[255];
	char outputname[100];
	int inputsize;
    
    strcpy(inputname, "");
    strcat(inputname, path);
    printf("Filename:\t%s\n", inputname);
	*entry_file = fopen(inputname, "rb");
	if (entry_file == NULL) {
        fprintf(stderr, "Error : Failed to open file - %s\n", strerror(errno));
        return -1;
    }
    if (mode == 2 && strstr(inputname, FILEFORMAT)) {
        inputname[strlen(inputname) - strlen(FILEFORMAT)] = '\0';
        strcpy(outputname, inputname);
    } else if (mode == 1 && !strstr(inputname, FILEFORMAT)) {
        strcpy(outputname, inputname);
        strcat(outputname, FILEFORMAT);
    } else {
        return -1;
    }
	*output = fopen(outputname, "wb");
	fseek(*entry_file, 0L, SEEK_END);
    inputsize = ftell(*entry_file);
    if (mode == 1) {
        fwrite(SZOVEG, 1, strlen(SZOVEG), *output);
        fseek(*entry_file, 0L, SEEK_SET);
    } else {
        fseek(*entry_file, strlen(SZOVEG), SEEK_SET);
        inputsize -= strlen(SZOVEG);
    }

    printf("Filesize:\t%d\n", inputsize);
	return inputsize;
}
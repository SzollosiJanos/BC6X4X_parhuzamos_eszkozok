#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <BC6X4X_helper.h>



int main(int argc, char ** argv) {
    if (argc != 4) {
        printf("ERROR. Usage: BC6X4X.exe <folder path> <encrypt mode: [1,2]> <encrypt/decrypt [1,2]>\n");
    }
    cl_int err;
    cl_uint n_platforms;
    cl_platform_id platform_id;
    cl_device_id device_id;
	cl_kernel kernel;
	cl_context context;
	cl_program program;
    DIR * FD;
    struct dirent * in_file;
    FILE * entry_file, * output;
    char filename[255], outputname[100];
    int key = atoi(argv[2]), mode = atoi(argv[3]), inputsize, i;
	
	
	if (NULL == (FD = opendir(argv[1]))) {
        fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
        return 0;
    }
	if(init_kernel(&err,&n_platforms,&platform_id,&device_id,&context,&program)==-1){
		printf("ERROR!!");
		return 0;
	}
    

    clock_t start, end, temp;
    start = clock();
    while ((in_file = readdir(FD))) {
		if (in_file -> d_type == DT_DIR)
			continue;
        temp = clock();
		strcpy(filename, "");
        strcat(filename, argv[1]);
        strcat(filename, in_file -> d_name);
		inputsize=test_and_write_filename(filename,mode,&output,&entry_file);
        if(inputsize==-1){
			continue;
		}
		if(mode==2){
			kernel = clCreateKernel(program, "decrypt", NULL);
		}else{
			kernel = clCreateKernel(program, "encrypt", NULL);
		}
        unsigned char * inputfield = (char * ) malloc(inputsize * sizeof(char));
        cl_mem device_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, inputsize * sizeof(char), NULL, NULL);
        if (fread(inputfield, 1, inputsize, entry_file) != NULL) {
            clSetKernelArg(kernel, 0, sizeof(cl_mem), (void * ) & device_buffer);
            clSetKernelArg(kernel, 1, sizeof(int), (void * ) & key);
            cl_command_queue command_queue = clCreateCommandQueue(context, device_id, NULL, NULL);
            clEnqueueWriteBuffer(command_queue, device_buffer, CL_FALSE, 0, inputsize * sizeof(char), inputfield, 0, NULL, NULL);
            size_t local_work_size = 256;
            size_t n_work_groups = (inputsize + local_work_size + 1) / local_work_size;
            size_t global_work_size = n_work_groups * local_work_size;
            clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, & global_work_size, & local_work_size, 0, NULL, NULL);
            clEnqueueReadBuffer(command_queue, device_buffer, CL_TRUE, 0, inputsize * sizeof(char), inputfield, 0, NULL, NULL);
            fwrite(inputfield, 1, inputsize, output);
        }
		
        fclose(entry_file);
        fclose(output);
        remove(filename);
        free(inputfield);
        end = clock();
        printf("Runtime:\t%lf\n\n", ((double)(end - temp)) / CLOCKS_PER_SEC);
    }
    end = clock();
    printf("\n\nOverall runtime:\t%lf\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);
    clReleaseDevice(device_id);
}

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


#include "BC6X4X_kernel.cl"
#define SZOVEG "Encrypted by: Szöllősi János(BC6X4X)\n"
#define FILEFORMAT ".bc6x4x"

int main(int argc, char **argv) {
	if(argc!=4){
		printf("ERROR. Arg 1 must be a folder name, arg 2 must be a number for mode select, arg 3 must be a number for encrypt mode.\n");
	}
	cl_int err;
	cl_uint n_platforms;
	cl_platform_id platform_id;
	cl_device_id device_id;
	DIR* FD;
    struct dirent* in_file;
    FILE    *entry_file,*output;
	char filename[255],outputname[100];
	int key=atoi(argv[2]),mode=atoi(argv[3]),inputsize,szovegsize,i;
	cl_kernel kernel;
	
	
	err = clGetPlatformIDs(1, &platform_id, &n_platforms);
	if (NULL == (FD = opendir (argv[1]))) 
    {
        fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
        return 0;
    }
	if (err != CL_SUCCESS) {
		printf("[ERROR] Error calling clGetPlatformIDs. Error code: %d\n", err);
		return 0;
	}
	cl_uint n_devices;err = clGetDeviceIDs(platform_id,CL_DEVICE_TYPE_GPU,1,&device_id,&n_devices);
	if (err != CL_SUCCESS) {
		printf("[ERROR] Error calling clGetDeviceIDs. Error code: %d\n", err);
		return 0;
	}
    cl_context context = clCreateContext(NULL, n_devices, &device_id, NULL, NULL, NULL);
	cl_program program = clCreateProgramWithSource(context, 1, &kernel_code, NULL, NULL);
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Build error! Code: %d\n", err);
        return 0;
    }
	
	struct timeval stop, start;
	gettimeofday(&start, NULL);
    while ((in_file = readdir(FD))) 
    {
        if (!strcmp (in_file->d_name, "."))
            continue;
        if (!strcmp (in_file->d_name, ".."))    
            continue;
		if (in_file->d_type==DT_DIR)    
            continue;
		strcpy(filename,"");
		strcat(filename,argv[1]);
		strcat(filename,in_file->d_name);
		printf("%s\n\n",filename);
		entry_file = fopen(filename, "rb");
		if(mode==2&&strstr(filename,FILEFORMAT)){
			filename[strlen(filename)-strlen(FILEFORMAT)]='\0';
			strcpy(outputname, filename);
			kernel = clCreateKernel(program, "decrypt", NULL);
		}else if(mode==1&&!strstr(filename,FILEFORMAT)){
			strcpy(outputname, filename);
			strcat(outputname,FILEFORMAT);
			kernel = clCreateKernel(program, "encrypt", NULL);
		}else{
			continue;
		}
		output=fopen(outputname,"wb");
		
        
		fseek(entry_file, 0L, SEEK_END);
		inputsize = ftell(entry_file);
		fseek(entry_file, 0L ,SEEK_SET);
		unsigned char *inputfield=(char*)malloc(inputsize*sizeof(char));
		cl_mem device_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, inputsize * sizeof(char), NULL, NULL);
        if (entry_file == NULL)
        {
            fprintf(stderr, "Error : Failed to open file - %s\n", strerror(errno));
            return 1;
        }
		
        if (fread(inputfield,1, inputsize, entry_file) != NULL)
        {
			clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&device_buffer);
			szovegsize=strlen(SZOVEG);
			clSetKernelArg(kernel, 1, sizeof(int), (void*)&szovegsize);
			clSetKernelArg(kernel, 2, sizeof(int), (void*)&key);
			cl_command_queue command_queue = clCreateCommandQueue(context, device_id, NULL, NULL);
	
			clEnqueueWriteBuffer(command_queue,device_buffer,CL_FALSE,0,inputsize * sizeof(char),inputfield,0,NULL,NULL);

			size_t local_work_size = 256;
			size_t n_work_groups = (inputsize + local_work_size + 1) / local_work_size;
			size_t global_work_size = n_work_groups * local_work_size;

			clEnqueueNDRangeKernel(command_queue,kernel,1,NULL,&global_work_size,&local_work_size,0,NULL,NULL);
			clEnqueueReadBuffer(command_queue,device_buffer,CL_TRUE,0,inputsize * sizeof(char),inputfield,0,NULL,NULL);
			if(mode==1){
				fwrite(SZOVEG,1,strlen(SZOVEG),output);
				fwrite(inputfield,1,inputsize,output);
			}else{
				fwrite(inputfield,1,inputsize-szovegsize,output);
			}
			
        }
		strcpy(filename,"");
		strcat(filename,argv[1]);
		strcat(filename,in_file->d_name);
		fclose(entry_file);
		fclose(output);
		remove(filename);
		free(inputfield);
    }
	gettimeofday(&stop, NULL);
	printf("took %d.%d sec\n", ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec)/1000000,((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec)%1000); 
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);
    clReleaseDevice(device_id);
}
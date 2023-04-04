#include <stdio.h>
#include <CL/cl.h>
#include <kernel_loader.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#define FILEFORMAT ".bc6x4x"
#define SZOVEG "Encrypted by: Szöllõsi János (BC6X4X)\n"


/*
 *
 * Initialize the kernel itself for OpenCL
 *
 */
int init_kernel(cl_int *err,cl_uint *n_platforms,cl_platform_id *platform_id,cl_device_id *device_id,cl_context * context,cl_program* program);

/*
 *
 * Encrypt the given char buffer on CPU
 *
 */
void encrypt(char * buffer, int size, int key);

/*
 *
 * Decrypt the given char buffer on CPU
 *
 */
void decrypt(char * buffer, int size, int key);

/*
 *
 * Test the given filename and initialize the output pointer
 *
 */
int test_and_write_filename(char* path,int mode,FILE ** output,FILE ** entry_file);

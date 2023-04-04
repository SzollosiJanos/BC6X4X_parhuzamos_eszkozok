#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <BC6X4X_helper.h>



int main(int argc, char ** argv) {
    if (argc != 4) {
        printf("ERROR. Usage: BC6X4X_normal.exe <folder path> <encrypt mode: [1,2]> <encrypt/decrypt [1,2]>\n");
    }
    DIR * FD;
    struct dirent * in_file;
    FILE * entry_file, * output;
    char filename[255], outputname[100];
    int key = atoi(argv[2]), mode = atoi(argv[3]), inputsize, i;
    clock_t start, end, temp;
    start = clock();
    if (NULL == (FD = opendir(argv[1]))) {
        fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
        return 0;
    }
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
        unsigned char * inputfield = (char * ) malloc(inputsize * sizeof(char));
        if (fread(inputfield, 1, inputsize, entry_file) != NULL) {
            if (mode == 1) {
                encrypt(inputfield, inputsize, key);
            } else {
                decrypt(inputfield, inputsize, key);
            }
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
}

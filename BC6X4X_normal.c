#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

#define FILEFORMAT ".bc6x4x"
#define SZOVEG "Encrypted by: Szöllősi János (BC6X4X)\n"

void encrypt(char * buffer, int size, int key);
void decrypt(char * buffer, int size, int key);

int main(int argc, char ** argv) {
    if (argc != 4) {
        printf("ERROR. Arg 1 must be a folder name, arg 2 must be a number for mode select, arg 3 must be a number for encrypt mode.\n");
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
        temp = clock();
        if (!strcmp(in_file -> d_name, "."))
            continue;
        if (!strcmp(in_file -> d_name, ".."))
            continue;
        if (in_file -> d_type == DT_DIR)
            continue;
        strcpy(filename, "");
        strcat(filename, argv[1]);
        strcat(filename, in_file -> d_name);
        printf("Filename:\t%s\n", filename);
        entry_file = fopen(filename, "rb");
        if (mode == 2 && strstr(filename, FILEFORMAT)) {
            filename[strlen(filename) - strlen(FILEFORMAT)] = '\0';
            strcpy(outputname, filename);
        } else if (mode == 1 && !strstr(filename, FILEFORMAT)) {
            strcpy(outputname, filename);
            strcat(outputname, FILEFORMAT);
        } else {
            continue;
        }
        output = fopen(outputname, "wb");
        fseek(entry_file, 0 L, SEEK_END);
        inputsize = ftell(entry_file);
        if (mode == 1) {
            fwrite(SZOVEG, 1, strlen(SZOVEG), output);
            fseek(entry_file, 0 L, SEEK_SET);
        } else {
            fseek(entry_file, strlen(SZOVEG), SEEK_SET);
            inputsize -= strlen(SZOVEG);
        }
        printf("Filesize:\t%d\n", inputsize);
        unsigned char * inputfield = (char * ) malloc(inputsize * sizeof(char));
        if (entry_file == NULL) {
            fprintf(stderr, "Error : Failed to open file - %s\n", strerror(errno));
            return 1;
        }
        if (fread(inputfield, 1, inputsize, entry_file) != NULL) {
            if (mode == 1) {
                encrypt(inputfield, inputsize, key);
            } else {
                decrypt(inputfield, inputsize, key);
            }
            fwrite(inputfield, 1, inputsize, output);

        }
        strcpy(filename, "");
        strcat(filename, argv[1]);
        strcat(filename, in_file -> d_name);
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

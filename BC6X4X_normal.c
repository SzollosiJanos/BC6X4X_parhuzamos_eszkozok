#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

#define SZOVEG "Encrypted by: Szöllősi János(BC6X4X)\n"
#define FILEFORMAT ".bc6x4x"

void encrypt(char* buffer,int size, int key);
void decrypt(char* buffer,int size, int key,int from);


int main(int argc, char **argv) {
	if(argc!=4){
		printf("ERROR. Arg 1 must be a folder name, arg 2 must be a number for mode select, arg 3 must be a number for encrypt mode.\n");
	}
	DIR* FD;
    struct dirent* in_file;
    FILE    *entry_file,*output;
	char filename[255],outputname[100];
	int key=atoi(argv[2]),mode=atoi(argv[3]),inputsize,szovegsize,i;
	struct timeval stop, start;
	gettimeofday(&start, NULL);
	if (NULL == (FD = opendir (argv[1]))) 
    {
        fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
        return 0;
    }
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
		}else if(mode==1&&!strstr(filename,FILEFORMAT)){
			strcpy(outputname, filename);
			strcat(outputname,FILEFORMAT);
		}else{
			continue;
		}
		output=fopen(outputname,"wb");
		fseek(entry_file, 0L, SEEK_END);
		inputsize = ftell(entry_file);
		fseek(entry_file, 0L ,SEEK_SET);
		unsigned char *inputfield=(char*)malloc(inputsize*sizeof(char));
        if (entry_file == NULL)
        {
            fprintf(stderr, "Error : Failed to open file - %s\n", strerror(errno));
            return 1;
        }
        if (fread(inputfield,1, inputsize, entry_file) != NULL)
        {
			szovegsize=strlen(SZOVEG);
			if(mode==1){
				encrypt(inputfield,inputsize,key);
				fwrite(SZOVEG,1,strlen(SZOVEG),output);
				fwrite(inputfield,1,inputsize,output);
			}else{
				decrypt(inputfield,inputsize,key,strlen(SZOVEG));
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
}


void encrypt(char* buffer,int size, int key){
	for(int i=0;i<size;i++){
		if(key==1){
			buffer[i]+=13;
		}else if(key==2){
			buffer[i]+=i;
		}
	}
}

void decrypt(char* buffer,int size, int key,int from){
	for(int i=0;i<size;i++){
		if(key==1){
			buffer[i]-=13;
		}else if(key==2){
			if(i>=from){
				buffer[i-from]=buffer[i]-(i-from);
			}
		}
	}
}
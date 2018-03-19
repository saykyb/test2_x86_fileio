/*
 ============================================================================
 Name        : test2_x86_fileio.c
 Author      : kyb
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#if 1
/*
 ============================================================================
 Name        : test1_x86_fileio.c
 Author      : kyb
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#define MAX_STR_BUF 512
#define FIELD_LEN   128

#define  FIELD_INDEX    8

FILE *fp;
FILE *fp2;
FILE *fpOut;
char *strTmp1 = "./tmp1.txt";
char *strOutput = "./output.txt";


char str1[MAX_STR_BUF];

char strTmp[MAX_STR_BUF];
char buf[MAX_STR_BUF];

int main(void) {

	int lineCount;

	// 1.
	system("find . -type f -name BIO_* -printf '%p\n'>tmp1.txt");

	printf("output tmp1.txt\n");
	//=========================================================================
	// 2.
	fp=fopen(strTmp1,"r");
	if(!fp){
		printf("Error io 1~!\n");
		return 1;
	}
	fpOut=fopen(strOutput,"wt");
	if(!fpOut){
		printf("Error io fpOut~!\n");
		return 1;
	}
	while(!feof(fp)){
		int cnt=0;
		int i;
		char strField[MAX_STR_BUF];
		int nTmp,nFieldFatiLevel;
		memset(str1,0,MAX_STR_BUF);
		memset(strTmp,0,MAX_STR_BUF);

		memset(buf,0,MAX_STR_BUF);

		//read
		fscanf(fp,"%s\n",str1);

		fp2=fopen(str1,"r");
		if(!fp2){
			printf("Error - Can not open file [%s] ~!\n",str1);
			return 1;
		}
		while(!feof(fp2)){
			memset(strField,0,MAX_STR_BUF);
			fscanf(fp2,"%s\n",strField);

			// working
			int nCount=0;
			for(i=0;i<strlen(strField);i++){
				if(strField[i]=='\n' || strField[i]=='\0'){
					break;
				} else if(strField[i]==','){
					nCount++;
					if(nCount >=FIELD_INDEX ){
						break;
					}
				}
			}
			if(nCount>=FIELD_INDEX){
				if(&strField[i++]){
					nFieldFatiLevel = strField[i] -'0';
					if(nFieldFatiLevel>0 && nFieldFatiLevel<= 4){
						snprintf(buf,MAX_STR_BUF,"%s ==>%s\n",str1,strField);
						fputs(buf,fpOut);
					}
				} else {
					printf("check file ~~!\n");

				}
			}
		}
		fclose(fp2);
	}
	printf("output file : output.out\n");
	printf("Done~!\n");
	fclose(fp);
	fclose(fpOut);
	//puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}

#else
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#define MAX_PATH_LEN 1024
#define FIELD_LEN  128
//#define FIELD_NUM 13   // TIME,HR,Resp,GSR,SKT,RRi,SDNN,FatiVal,FatiLevel,gPpgReliability,gCurUserIndex,baseSDNN,nEyeDropMs

FILE *fp;
FILE *fp2;
char *strOut = "./output.txt";

// field value
char strFieldTime[FIELD_LEN];
int nTmp;
int nFieldFatiLevel;

char str1[MAX_PATH_LEN];
char buf[MAX_PATH_LEN];
char path[MAX_PATH_LEN];

void listdir(const char *name, int indent)
{
    DIR *dir;
    struct dirent *entry;
    //char strCurDir[MAX_PATH_LEN];
    //char strCurFile[MAX_PATH_LEN];

    if (!(dir = opendir(name)))
        return;

	fp2=fopen(strOut,"wt");
	if(!fp2){
		printf("Error io 2 ~!\n");
		return ;
	}

    while ((entry = readdir(dir)) != NULL) {
    	int lineCount;

        if (entry->d_type == DT_DIR) {

            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            printf("%*s[%s]\n", indent, "", entry->d_name);
            listdir(path, indent + 2);
            //printf("PATH:%s <==\n",path);
        } else {

        	//printf("%*s- %s\n", indent, "", entry->d_name);
            //[[
        	/*
            if(!(getcwd(strCurDir,MAX_PATH_LEN))){
            	printf("ERROR ~~\n");
            	return;
            }
            */
            //snprintf(strCurFile,MAX_PATH_LEN,"%s/%s",strCurDir,entry->d_name);
        	if(!strncmp("BIO_",entry->d_name,4)){

				snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
				printf("%s\n",path);
				// TODO:
				fp=fopen(path,"r");
				if(!fp){
					printf("Error io 1~!\n");
					return ;
				}


				lineCount=0;
				while(!feof(fp)){

					memset(buf,0,MAX_PATH_LEN);
					memset(strFieldTime,0,FIELD_LEN);
					//read
					fscanf(fp,"%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",strFieldTime,&nTmp,&nTmp,&nTmp,&nTmp,&nTmp,&nTmp,&nTmp,&nFieldFatiLevel,&nTmp,&nTmp,&nTmp,&nTmp,&nTmp);

					// working
					if(nFieldFatiLevel>0){
						snprintf(buf,MAX_PATH_LEN,"%s, time:%s\n",path,strFieldTime);
						fputs(buf,fp2);
					}
				}
				printf("output output.txt(total line:%d)\n", lineCount);
				fclose(fp);
        	}
            //]]

        }

    }

	fclose(fp2);
    closedir(dir);
}

int main(void) {
    listdir(".", 0);
    return 0;
}
#endif

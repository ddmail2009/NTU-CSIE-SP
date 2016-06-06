#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"merge_sort.h"

int main(int argv, char* argc[]){
	int beg, end;
	char tmpfileName[TMPFILENAMESIZE]="";

	if(argv!=4){
		printf("The number of argumets is wrong!\n");
		exit(EXIT_FAILURE);
	}
	beg= atoi(argc[1]);
	end= atoi(argc[2]);
	strncat(tmpfileName, argc[3], TMPFILENAMESIZE-1);
	if( MP3_SORT(beg, end, tmpfileName) == -1){
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}


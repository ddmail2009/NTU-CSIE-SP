#include <stdlib.h>
#include <stdio.h>
#include "merge_sort.h"

int main(int argc,char*argv[]){
    if(MP3_MERGE(argc-1, argv+1)==-1){
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

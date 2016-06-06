#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include"merge_sort.h"
#include"heap.h"
static int errormsg(char* msg){
	fprintf(stderr, "MP3_MERGE: %s\n", msg);
	return -1;
}
struct HeapElement{
	int key;
	FILE*fp;
};

static int heap_compare(const void* a, const void* b){
	const struct HeapElement* c = a;
	const struct HeapElement* d = b;
	return c->key - d->key;
	/*
	return c->key > d->key? 1 : (c->key == d->key? 0: -1);
	*/
}

static void* heap_copy(void* a, const void* b, size_t num){/*a little faster than memcpy*/
	struct HeapElement* c = a;
	const struct HeapElement* d = b;
	*c = *d;
	return a;
}

/*specialized function is much faster than fscanf(fp, "%d", r)*/
static int fgetint(FILE* fp, int* r){
	int a, n;
	char s[10];
	if(fread(s, sizeof(char), 10, fp) != 10)
		return EOF;
	n = 0;
	for(a = 0; a < 9; a++)
		n = n * 10 + s[a] - '0';
	*r = n;
	return 1;
}
/*much faster than fprintf(fp, "%09d\n", n)*/
static int fputint(FILE* fp, int n){
	int a;
	char s[10];
	s[9] = '\n';
	for(a = 8; a >= 0; a--){
		s[a] = n % 10 + '0';
		n /= 10;
	}
	if(fwrite(s, sizeof(char), 10, fp) != 10)
		return -1;
	return 10;
}

static int compare(const void* a, const void *b){
	return ( *(int*)a - *(int*)b );
}

/*public merge function*/
int MP3_MERGE(int argc, char* argv[]){
	FILE** fp;
	void* heap;
	int a;
	if(argc < 2)
		return errormsg("too few arguments");

	if(argc==2 && strncmp(argv[0], argv[1], strlen(argv[0]) )==0)
		return 0;
/*open files*/
	fp = (FILE**)malloc(sizeof(FILE*) * argc);
	if(fp == NULL)
		return errormsg("malloc error");
	for(a = 0; a < argc; a++){
		fp[a] = fopen(argv[a], (a == 0? "w": "r"));
		if(fp[a] == NULL)
			return errormsg("fopen error");
	}
/*create heap*/
	heap = heap_alloc(argc - 1, sizeof(struct HeapElement), heap_compare, heap_copy);
	if(heap == NULL)
		return errormsg("malloc error");
	for(a = 1; a < argc; a++){
		struct HeapElement e;
		e.fp = fp[a];
		if(fgetint(e.fp, &(e.key)) == 1)
			heap_push(heap, &e);
		else
			fclose(fp[a]);
	}
/*merge*/
	while(!heap_isempty(heap)){
		struct HeapElement e;
		heap_getmin(heap, &e);
		if( fputint(fp[0], e.key) < 0)
			return errormsg("output error");
		if(fgetint(e.fp, &e.key) == 1)
			heap_changemin(heap, &e);
		else{
			heap_pop(heap);
			fclose(e.fp);
		}
	}
/*clean*/
	heap_free(heap);
	fflush(fp[0]);
	free(fp);
	return 0;
}
int MP3_SORT(int beg, int end, char tmpfileName[]){
	int i;
	int *dataNum;
	FILE *fpnum, *fptfile;
	if(end-beg > MAX_LENGTH_CHUNK)
		return errormsg("Exceeding max length of chunk!");
	
//extract data from num.txt, and sort them by qsort()
	if( (fpnum= fopen("num.txt", "r")) ==NULL)
		return errormsg("fopen(num.txt) errors!\n");
	
	if(fseek(fpnum, beg *RECORDSIZE , SEEK_SET) < 0 )
		return errormsg("fseek() to the beg position errors!\n");

	if( (dataNum= malloc( sizeof(int)*(end-beg) )) == NULL)
		return errormsg("Allocating dataNum errors!\n");

	for(i=0; i< end-beg ; ++i)
		fgetint(fpnum, &dataNum[i]);
	
	qsort(dataNum, end-beg, sizeof(int), compare);
	
//open a tmpfile to save result
	if( ( fptfile= fopen(tmpfileName, "w") )==NULL){
		free(dataNum);
		return errormsg("Openning tmp file errors!\n");
	}
//printf("size:%d\n", size);
	for(i=0; i<end-beg; i++){
		if(fputint(fptfile,dataNum[i]) < 0){
			free(dataNum);
			return errormsg("Writing tmpfile errors!\n");
		}
	}
	fflush(fptfile);
	free(dataNum);
	return 0;
}

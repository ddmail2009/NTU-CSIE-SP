#ifndef MP3_MERGE_SORT_DEV
#define MP3_MERGE_SORT_DEV
#define MAX_LENGTH_CHUNK 100000
#define TMPFILENAMESIZE 40
#define RECORDSIZE 10
int MP3_MERGE(int argc, char* argv[]);
/*
 * parameter: 
 *   argc = number of elements in argv[]
 *   argv[0] = output file name
 *   argv[1] = tmp file name 1
 *   argv[2] = tmp file name 2
 *   and so on
 * return:
 *   0 if success
 *   -1 if error
 */
int MP3_SORT(int beg, int end, char tmpfileName[]);
/*
 * parameter: 
 *   beg = the begin of range
 *   end = the end of range
 *   tmpfileName = the name of output file
 * return:
 *   0 if success
 *   -1 if error
 */
#endif

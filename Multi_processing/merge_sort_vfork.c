#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#include "./lib/merge_sort.h"

#define NAME_LENGTH TMPFILENAMESIZE
#define TOP(x) ((x)>TXT_LENGTH?TXT_LENGTH:(x))
#define IN(x,y,z) ((x<y)&&(y<=z))
#define MIN(x,y) (x<y?x:y)
#define CONFIGURE(a,b) {CHUNK_LENGTH=MIN(a,MAX_LENGTH_CHUNK);\
                        MERGE_DEGREE=b;}

int MAX_PROCESS , CHUNK_LENGTH , MERGE_DEGREE;
int TXT_LENGTH , CHILD_MAX;
char sort_file[10]="lib/sort" , merge_file[10]="lib/merge";

void auto_config()
{
    MAX_PROCESS = CHILD_MAX;

    if( IN(0,TXT_LENGTH,1000) )
        CONFIGURE( TXT_LENGTH , 2 )
    else if( IN(1000,TXT_LENGTH,10000) )
        CONFIGURE( TXT_LENGTH/2 , 4 )
    else if( IN(10000,TXT_LENGTH,100000) )
        CONFIGURE( TXT_LENGTH/8 , 8 )
    else if( IN(100000,TXT_LENGTH,1000000) )
        CONFIGURE( TXT_LENGTH/16 , 8 )
    else if( IN(1000000,TXT_LENGTH,3200000) )
        CONFIGURE( TXT_LENGTH/32 , 14 )
    else if( IN(3200000,TXT_LENGTH,6400000) )
        CONFIGURE( TXT_LENGTH/64 , 20 )
    else if( IN(6400000,TXT_LENGTH,10000000) )
        CONFIGURE( TXT_LENGTH/96 , 30 )
    else
        CONFIGURE( MAX_LENGTH_CHUNK , 35 );
}

int FORK_SORT()
{
    int child_pid[ MAX_PROCESS ] , begin = 0 , end = 0;
    int i , j , index , status;
    char fullname[ NAME_LENGTH ] = "merge_", *str , beg[13] , endd[13];
    str = fullname + 6;

    for( i=0 ; end!=TXT_LENGTH ; begin+=CHUNK_LENGTH )
    {
        end = TOP( begin+CHUNK_LENGTH );
        sprintf( str, "%d_%d" , begin , end );

        sprintf( beg , "%d" , begin );
        sprintf( endd , "%d" , end );

        index = i%MAX_PROCESS;
        while( (child_pid[ index ] = vfork()) < 0 )
            wait( &status );

        if( child_pid[ index ] == 0 )
        {
            execl( sort_file , sort_file , beg , endd , fullname , (char*)0 );
            _exit(1);
        }
        else if( (++i) >= MAX_PROCESS )
            wait( &status );
    }
    for( j=0 ; j<MAX_PROCESS && j<i ; j++ )
        waitpid( child_pid[j] , &status , 0 );
    return 0;
}

int FORK_MERGE()
{
    int child_pid[ MAX_PROCESS ] , tar_end , tar_beg , end , beg;
    int file_count , chunk_size = CHUNK_LENGTH;
    int i , x , step , index , status;

    char *files[MERGE_DEGREE+3], *str[MERGE_DEGREE+2] , *tmp;
    files[0] = merge_file;
    for( x=1 ; x<=MERGE_DEGREE+1 ; x++ )
    {
        files[ x ] = (char*)malloc(sizeof(char)*NAME_LENGTH);
        sprintf( files[x] , "merge_" );
        str[x-1] = files[x] + 6 ;
    }
    files[MERGE_DEGREE+2] = NULL;
    str[MERGE_DEGREE+1] = files[MERGE_DEGREE+2];

    for( step=0 ; ; step++ , chunk_size*=MERGE_DEGREE )
    {
        for( i=0 , tar_beg=0 ; ; tar_beg+=chunk_size*MERGE_DEGREE )
        {
            tar_end = TOP( tar_beg + chunk_size*MERGE_DEGREE );

            sprintf( str[0] , "%d_%d" , tar_beg , tar_end );
            for( file_count=1,beg=tar_beg,end=tar_beg;
                 file_count<=MERGE_DEGREE&&end!=tar_end ;
                 file_count++ , beg+=chunk_size )
            {
                end = TOP( beg + chunk_size );
                sprintf( str[file_count] , "%d_%d" , beg , end );
            }

            if( file_count != MERGE_DEGREE+1 )
            {
                tmp = files[file_count+1];
                files[file_count+1] = files[MERGE_DEGREE+2];
                files[MERGE_DEGREE+2] = tmp;
            }

            index = i%MAX_PROCESS;
            while( (child_pid[ index ] = vfork()) < 0 )
                wait( &status );

            if( child_pid[ index ] == 0 )
            {
                execv( "lib/merge" , files );
                _exit(1);
            }
            else if( (++i) >= MAX_PROCESS )
                wait( &status );

            if( file_count != MERGE_DEGREE+1 )
            {
                tmp = files[file_count+1];
                files[file_count+1] = files[MERGE_DEGREE+2];
                files[MERGE_DEGREE+2] = tmp;
            }

            if( tar_end == TXT_LENGTH )
                break;
        }

        for( x=0 ; x<MAX_PROCESS && x<i ; x++ )
            waitpid( child_pid[x] , &status , 0 );

        if( tar_beg == 0 )
            break;
    }
    return 0;
}

int main ( int argc , char *argv[] )
{
    if( argc == 4 )
    {
        CHILD_MAX = sysconf( _SC_CHILD_MAX )-7;
        MAX_PROCESS = atoi( argv[1] );
        CHUNK_LENGTH = atoi( argv[2] );
        MERGE_DEGREE = atoi( argv[3] );

        struct stat statbuf;
        stat( "num.txt" , &statbuf );
        TXT_LENGTH = statbuf.st_size/10;

        if( MERGE_DEGREE == CHUNK_LENGTH && CHUNK_LENGTH == MAX_PROCESS && MAX_PROCESS == 0 )
            auto_config();

        MAX_PROCESS = MIN( MAX_PROCESS , CHILD_MAX );

        if( MAX_PROCESS<0 || MERGE_DEGREE<2 || CHUNK_LENGTH<1 )
        {
            puts("parameter Error");
            exit(1);
        }

        FORK_SORT();
        FORK_MERGE();
    }
    else
        puts("Argc not correct");
    return 0;
}

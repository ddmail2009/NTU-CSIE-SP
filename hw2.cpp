#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include "base.h"

int size_limit;
File_Shortest files;
#define Path_MAX (1025)

int select_size( struct stat *statbuf )
{
    if( statbuf->st_size <= size_limit )return true;
    else return false;
}

int select_time( struct stat *statbuf )
{
    if( statbuf->st_ctime == statbuf->st_mtime )return true;
    else return false;
}

int select_all_pass( struct stat *statbuf )
{
    return true;
}

void File_traversal( char *fullpath , int (*select)(struct stat*) )
{
    struct stat statbuf , parbuf;
    struct dirent *dir;
    DIR *dp;

    char parent[ Path_MAX ];
    strcpy( parent , fullpath);
    
    stat( parent , &parbuf );
    files.Add_point( parbuf.st_ino );

    dp = opendir( fullpath );
    while( (dir = readdir(dp)) != NULL )
    {
        if( strcmp(dir->d_name,".") && strcmp(dir->d_name,"..") )
        {
            sprintf( fullpath,"%s/%s",parent,dir->d_name );

            if( stat( fullpath , &statbuf ) == -1 )
                continue;

            if( S_ISDIR(statbuf.st_mode) && files.Search( statbuf.st_ino ) == -1 )  
                File_traversal( fullpath , select );
            else files.Add_point( statbuf.st_ino );
            
            if( select( &parbuf ) && select( &statbuf ) )
                files.Add_Line( statbuf.st_ino , parbuf.st_ino );
        }
    }
    closedir( dp );
}

int main ( int argc , char *argv[] )
{
    int (*select)(struct stat*);
    char *fullpath = new char[ Path_MAX ];

    if( argc == 2 )
    {
        select = select_all_pass;
        strcpy( fullpath , argv[1] );
    }
    else if( argc == 3 && !strcmp( argv[1] , "-t" ) )
    {
        select = select_time;
        strcpy( fullpath , argv[2] );
    }
    else if( argc == 4 && !strcmp( argv[1] , "-s" ) )
    {
        select = select_size;
        size_limit = atoi( argv[2] );
        strcpy( fullpath , argv[3] );
    }
    else return 1;

    if( chdir( fullpath ) == -1 )
        return -1;
    getcwd( fullpath , Path_MAX );
    File_traversal( fullpath , select );

    files.Update();
    printf("%d\n",files.Calculate());
    return 0;
}

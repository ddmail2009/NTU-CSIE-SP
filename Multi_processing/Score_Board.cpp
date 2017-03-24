#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Library.cpp"

int main ( int argc , char *argv[] )
{
    if( argc == 2 )
    {
        umask(0);
        unlink( argv[1] );
        
        if( mkfifo( argv[1] , 0666 ) < 0 )
            err_msg( "Fifo Create Error" );

        int fd_in;
        if( (fd_in = open( argv[1] , O_RDONLY )) == -1 )
            err_msg( "Fifo Open Error" );

        FILE *fd_out = fopen( "Result.txt" , "w" );

        char buffer;
        while( 1 )
        {
            if( read( fd_in , &buffer , 1 ) == 1 )
            {
                fwrite( &buffer , sizeof(char) , 1 , fd_out );
                fflush( fd_out );
            }
        }
    }
    else
        err_msg("number of arguments wrong");
}


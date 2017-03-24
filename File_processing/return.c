#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "library.h"
#include "base.h"

#define exit_say(x) {puts(x);exit(1);}

int main ( int argc , char *argv[] )
{
    if( argc == 2 )
    {
        Inform book;
        struct flock f1;
        char str[40];

        int book_index = atoi(argv[1]);
        int Book_file = open( "books",O_RDWR );

        f1.l_type = F_WRLCK;
        f1.l_whence = SEEK_SET;
        f1.l_start = Infosize*book_index;
        f1.l_len = Infosize;

        if( Book_file == -1 )
            exit_say("FILE OPEN ERROR")
        else
        {
            fcntl( Book_file , F_SETLKW , &f1 );
            if( lseek( Book_file , Infosize*book_index , SEEK_SET ) == -1 || read( Book_file , &book , Infosize ) != Infosize )
            {
                sprintf(str,"invalid book index %d\n",book_index);
                update_log(str,strlen(str));
            }
            else
            {
                if( book.userID_for_resv_request[0] == NO_USER )
                {
                    sprintf(str,"%d is not held by anyone\n",book_index);
                    update_log(str,strlen(str));
                }
                else
                {
                    sprintf(str,"%d returned %d\n",book.userID_for_resv_request[0],book_index);
                    update_log(str,strlen(str));

                    book.userID_for_resv_request[0] = NO_USER;
                    lseek( Book_file , Infosize*book_index , SEEK_SET );
                    write( Book_file , &book , Infosize);
                }
            }
            close( Book_file );
        }

    }
    return 0;
}

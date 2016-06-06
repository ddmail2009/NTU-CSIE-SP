#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "library.h"
#include "base.h"

#define exit_say(x) {puts(x);exit(1);}
#define update_book lseek( Book_file , Infosize*book_index , SEEK_SET );write( Book_file , &book , Infosize);

int main ( int argc , char *argv[] )
{
    if( argc == 3 )
    {
        int i;

        Inform book;
        struct flock f1;
        char str[40];

        int book_index = atoi(argv[1]);
        int user_index = atoi(argv[2]);

        int Book_file = open( "books",O_RDWR );

        f1.l_type = F_WRLCK;
        f1.l_whence = SEEK_SET;
        f1.l_start = Infosize*book_index;
        f1.l_len = Infosize;

        if( user_index < 0 )
        {
            sprintf(str,"invalid user ID %d\n",user_index);
            update_log(str,strlen(str));
        }
        else if( Book_file == -1 )
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
                int change = 0;

                if( book.userID_for_resv_request[0] == NO_USER )
                {
                    if( book.userID_for_resv_request[1] == user_index )
                    {
                        change = 1;

                        sprintf(str,"%d borrowed %d\n",user_index,book_index);
                        update_log(str,strlen(str));

                        book.number_of_resv_req --;
                        for( i=0 ; i<MAX_RESERVE-1 ; i++ )
                            book.userID_for_resv_request[i] = book.userID_for_resv_request[i+1];
                        update_book
                    }
                    else if( book.number_of_resv_req == 0 )
                    {
                        change = 1;

                        sprintf(str,"%d borrowed %d\n",user_index,book_index);
                        update_log(str,strlen(str));

                        book.userID_for_resv_request[0] = user_index;
                        update_book
                    }
                }

                if( change == 0 )
                {
                    int user_pos = -1;
                    for( i=0 ; i<=book.number_of_resv_req ; i++)
                        if( book.userID_for_resv_request[i] == user_index )
                            user_pos = i;

                    if( user_pos != -1 || book.number_of_resv_req == 15 )
                    {
                        sprintf(str,"%d couldn't borrow %d\n",user_index,book_index);
                        update_log(str,strlen(str));
                    }
                    else
                    {
                        sprintf(str,"%d reserved %d\n",user_index,book_index);
                        update_log(str,strlen(str));

                        book.userID_for_resv_request[ ++book.number_of_resv_req ] = user_index;
                        update_book
                    }
                }
            }
            close( Book_file );
        }
    }
    return 0;
}

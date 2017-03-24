#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define Inform struct BookInfo
#define Infosize sizeof(struct BookInfo)

void update_log( char *str , int len )
{
    int file = open( "book.log" , O_WRONLY | O_APPEND | O_CREAT , S_IRUSR | S_IWUSR );

    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;

    fcntl( file , F_SETLKW , &fl );
    write( file , str , sizeof(char)*len );
    close( file );
}

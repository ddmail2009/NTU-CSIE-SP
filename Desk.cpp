#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <fcntl.h>
#include <errno.h>

#include "Library.cpp"

int FIFO_OUT;

void Fork_pipe( int *fd1 , int *fd2 , char *exec , char * argv[] , int *pid )
{
    if( pipe( fd1 ) < 0 || pipe( fd2 ) )
        err_msg( "signal error" );

    if( (*pid = fork()) < 0 )
        err_msg( "fork error");
    else if( *pid == 0 )
    {
        if( close( fd1[1] ) < 0 || close( fd2[0] ) < 0 )
            err_msg( "file_discriptor close error");

        if( dup2( fd1[0] , STDIN_FILENO ) != STDIN_FILENO )
            err_msg( "dup2 error to stdin" );
        if( dup2( fd2[1] , STDOUT_FILENO ) != STDOUT_FILENO )
            err_msg( "dup2 error to stdout" );

        if( execv( exec , argv ) < 0 )
            err_msg( "execl error" );
        _exit(0);
    }
    else if( *pid > 0 )
    {
        if( close( fd1[0] ) < 0 || close( fd2[1] ) < 0 )
            err_msg( "file_discriptor close error");
    }

}

void FIFO_puts( char *str , int len )
{
    if( write( FIFO_OUT , str , len ) != len )
        err_msg( "FIFO WRITE UNCOMPLETE" );
}

class Desk
{
    public:
        Desk( int player , char *name , int Score )
        {
            Player_num = player;
            Table_Score = Score;
            turn = 0;
            order = 1;

            Name = MALLOC( char , strlen( name ) );
            strcpy( Name , name );

            fd_p = MALLOC( int* , Player_num );
            pid = MALLOC( int , Player_num );
            for( int i=0 ; i<Player_num ; i++ )
                fd_p[i] = MALLOC( int , 2 );

            alive = MALLOC( int , Player_num );
            for( int i=0 ; i<Player_num ; i++ )
                alive[i] = 1;
        }

        void Player_Set( int **a )
        {
            for( int i=0 ; i<Player_num ; i++ )
                for( int j=0 ; j<2 ; j++ )
                    fd_p[i][j] = a[i][j];
        }

        int Get_Card( int p )
        {
            char str[MAXLINE] = {0};
            if( Read_Player( p , str ) == 0 )
                Close_Player( p );
            else return Char_2_Int( str );
            return -1;
        }

        int Add_Card( int num )
        {
            if( num == 4 )
                order = -order;
            else if( MIDDLE( 0,Card_Effect(Table_Score,num),99 ) )
                Table_Score = Card_Effect(Table_Score,num);
            else return -1;

            return Table_Score;
        }

        void Next_player()
        {
            if( (turn+=order) < 0 )
                turn += Player_num;
            turn %= Player_num;
        }

        void Close_Player( int p )
        {
            int status;
            alive[p] = 0;
            //wait( &status );
            waitpid( pid[p] , &status , 0 );
            if( close( fd_p[p][0] ) < 0 || close( fd_p[p][1] ) < 0 )
                err_msg( "file_discriptor close error");
        }

        int Read_Player( int p , char *s )
        {
            int bit = read( fd_p[p][0] , s , MAXLINE );
            //printf("READ = %s",s);
            if( bit < 0 )
                err_msg( "Read Error" );
            else if( bit == 0 )
            {
                Close_Player( p );
                return -1;
            }
            return bit;
        }

        int Send_Player( int p , const char *s , int len )
        {
            //printf("Send Player[%d]: %s",p,s);

            if( write( fd_p[p][1] , s , len ) == -1 )
            {
                if( errno != EPIPE )
                    err_msg( "Write Error" );
                else
                {
                    Close_Player(p);
                    return -1;
                }
            }
            return len;
        }

        int Remain( int *win )
        {
            int remain = 0;
            for( int i=0 ; i<Player_num ; i++ )
            {
                if( alive[i] == 1 )
                {
                    remain++;
                    *win = i;
                }
            }
            return remain;
        }

        int Table_Score , Player_num , turn , order ;
        int *alive , **fd_p , *pid;
        char *Name;
};

int main( int argc , char *argv[] )
{
    if( argc == 7 )
    {
        if( signal( SIGPIPE , SIG_IGN ) < 0 )
            err_msg( "Signal Catch Fail" );

        srand( time(NULL) );
        Desk desk( 4 , argv[1] , 90 );

        if( (FIFO_OUT = open( argv[6] , O_WRONLY )) == -1 )
            err_msg( "fifo open error , please try run SCORE_BOARD FIRST" );

        for( int i=0 ; i<desk.Player_num ; i++ )
        {
            int fd1[2] , fd2[2];

            char **argument = MALLOC( char* , 6 );
            argument[0] = argv[2+i];
            for( int j=1 ; j<5 ; j++ )
            {
                argument[j] = MALLOC( char , 5 );
                Int_2_Char( rand()%13+1 , argument[j] , 0 );
            }
            argument[5] = NULL;

            Fork_pipe( fd1 , fd2 , argv[2+i] , argument , &desk.pid[i] );

            desk.fd_p[i][1] = fd1[1];
            desk.fd_p[i][0] = fd2[0];

            for( int j=1 ; j<5 ; j++ )
                free( argument[j] );
            free( argument );
        }

        int winner=0;
        while( desk.Remain(&winner) != 1 )
        {
            int player = desk.turn , num;
            if( desk.alive[player] == 1 )
            {
                char str[MAXLINE] = {0};
                int bit = sprintf( str , "%d\n" , desk.Table_Score );

                if( desk.Send_Player( player , str , bit ) == -1 )
                    continue;
                if( (num = desk.Get_Card( player )) == -1 )
                    continue;
                
                if( desk.Add_Card( num ) != -1 )
                {
                    char str1[MAXLINE] = {0};
                    Int_2_Char( rand()%13+1 , str1 , 0 );
                    int bit = sprintf( str1 , "%s\n" , str1);
                    if( desk.Send_Player( player , str1 , bit ) == -1 )
                        continue;
                    
                    char str2[MAXLINE] = {0};
                    if( desk.Read_Player( player , str2 ) == -1 )
                        continue;
                    if( strcmp( str2 , "ACK\n" ) )
                        err_msg( "Child check response Error" );
                }
                else
                {
                    if( desk.Send_Player( player , "FAIL\n" , 5 ) == -1 )
                        continue;
                    if( desk.Remain(&winner) == 1 )
                        break;
                    desk.Close_Player( player );
                }
                /*
                printf("STILL ALIVE: ");
                for( int i=0 ; i<desk.Player_num ; i++ )
                    if( desk.alive[i] == 1 )
                        printf("%d ",i);
                puts("\n");
                */
            }
            desk.Next_player();
        }
        desk.Send_Player( winner , "WIN\n" , 4 );
        desk.Close_Player( winner );

        char str[MAXLINE] = {0};
        int bit = sprintf( str , "%s %d\n" , desk.Name , winner+1 );
        FIFO_puts( str , bit );

        printf("%s %d\n",desk.Name , winner+1 );
    }
    else
        puts("number of parameter wrong");
    return 0;
}

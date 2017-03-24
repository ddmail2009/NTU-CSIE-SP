#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>

#include "Library.cpp"

int Get_num( int low , int high , int bit )
{
    char tmp[MAXLINE] = {0};
    read( STDIN_FILENO , tmp , MAXLINE );

    if( !strncmp( tmp , "FAIL" , 4 ) || !strncmp( tmp , "WIN" , 3 ) )
        exit(0);
    else
    {
        int value = (bit?atoi(tmp):Char_2_Int(tmp));

        if( !MIDDLE(low,value,high) )
            err_msg( "invalid Value passed from Desk" );
        else
            return value;
    }
    return -1;
}

int Give_Card( int num )
{
    if( MIDDLE( 1 , ABS(num) , 13 ) )
    {
        char tmp[MAXLINE] = {0};
        Int_2_Char( num , tmp , 1 );
        int bit = sprintf( tmp , "%s\n" , tmp );
        return write( STDOUT_FILENO , tmp , bit );
    }
    else
        err_msg( "GIVE CARD ERROR(WITH UNKNOWN NUM)" );
    return 0;
}

int Judge( int table , int *Set )
{
    for( int i=0 ; i<4 ; i++ )
    {
        if( Set[i] == 10 || Set[i] == 12 )
        {
            int p = Card_Effect( table , +Set[i] );
            int n = Card_Effect( table , -Set[i] );

            if( MIDDLE( 0 , p , 99 ) )return i;
            else if( MIDDLE( 0 , n , 99 ) )return i+4;
        }
        else
        {
            int num = Card_Effect( table , Set[i] );
            if( MIDDLE( 0 , num , 99 ) )return i;
        }
    }
    return 0;
}

void sig_pipe( int signo )
{
    exit(0);
}

int main ( int argc , char *argv[] )
{
    if( argc == 5 )
    {
        signal( SIGPIPE , sig_pipe );

        int Set[4];
        for( int i=0 ; i<4 ; i++ )
            Set[i] = Char_2_Int( argv[1+i] );

        while(1)
        {
            int table_num = Get_num( 0 , 99 , 1 );
            int give = Judge( table_num , Set );
            Give_Card( (give/4==0?1:-1)*Set[give%4] );
            Set[give%4] = Get_num( 1 , 13 , 0 );
            write( STDOUT_FILENO , "ACK\n" , 4 );
        }
    }
    else
        err_msg( "Argument number Wrong");
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIDDLE(a,b,c) (a<=b&&b<=c)
#define IS_SPECIAL(a) ( ( a==4||MIDDLE(10,a,13) )?'1':'0' )
#define ABS(a) (a<0?-a:a)

#define SWAP(x,y,type) { type tmp=x; x=y; y=tmp; }
#define MALLOC(type,n) (type*)malloc(sizeof(type)*n)

#define MAXLINE (10)

const char *FAILED="FAIL\n" , *WIN="WIN\n";

struct Card
{
    char number;
    char color;
};

void err_msg( const char *tmp )
{
    fprintf( stderr , "%s\n" , tmp );
    exit(155);
}

int Card_num( char *s )
{
    if( atoi(s) !=0 )
        return atoi(s);
    else if( s[0] == 'A' )
        return 1;
    else if( s[0] == 'J' )
        return 11;
    else if( s[0] == 'Q' )
        return 12;
    else if( s[0] == 'K' )
        return 13;
    else
        err_msg( "Unknown Card number");
    return 0;
}

int Card_Effect( int num , int card )
{
    if( card == 4 )
        return num;
    else if( card == 10 || card == -10 )
        return num+card;
    else if( card == 11 )
        return num;
    else if( card == 12 )
        return num+20;
    else if( card == -12 )
        return num-20;
    else if( card == 13 )
        return 99;
    else if( MIDDLE(1,card,9) )
        return num+card;
    else
        err_msg( "Card Effect Unknown" );
    return -1;
}

int Int_2_Char( int n , char *s , int bit )
{
    if( bit == 0 )
    {
        if( ABS(n)==1 )
            return sprintf( s , "A" );
        else if( ABS(n)==11 )
            return sprintf( s , "J" );
        else if( ABS(n)==12 )
            return sprintf( s , "Q" );
        else if( ABS(n)==13 )
            return sprintf( s , "K" );
        else
            return sprintf( s , "%d" , ABS(n) );
    }

    if( n<0 )
        s[0] = '-';
    else
        s[0] = '+';
    s[1] = ' ';
    return Int_2_Char( n , s+2 , 0 )+2;

}

int Char_2_Int( char *s )
{
    if( s[0] == '-' || s[0] == '+')
    {
        int p = 1;

        if( s[0] == '-'  )    p = -1;

        int num = Card_num( s+2 )*p;
        if( num<0 && num!=-12 && num!=-10 )
            err_msg( "Negative Number Error" );
        return num;
    }
    else
        return Card_num(s);
    return 0;
}

#include <stdio.h>

#define MAX_File (1024)
class File
{
    public:
        File( int a , int number )
        {
            ino_number = a;
            index = number;
        }
        int ino_number , index;
};

class File_Shortest
{
    public:
        File_Shortest()
        {
            index = sum = 0;
            for( int i=0 ; i<MAX_File ; i++ )
                for( int j=0 ; j<MAX_File ; j++ )
                {
                    if( i != j )path[i][j] = 0x0FFFFFFF;
                    else path[i][j] = 0;
                }
        }

        int Add_point( int a )
        {
            if( Search( a ) != -1 )
                return -1;
            else Files[index++] = new File( a , index );
            return index-1;
        }

        int Add_Line( int a , int b )
        {
            int x = Search( a ) , y = Search( b );

            if( x<0 || y<0 || x==y )return false;
            else path[x][y] = path[y][x] = 1;
            return true;
        }

        int Search( int a )
        {
            for( int i=0 ; i<index ; i++ )
                if( Files[i]->ino_number == a )
                    return Files[i]->index;
            return -1;
        }

        int Update()
        {
            for( int k=0 ; k<index ; k++ )
                for( int i=0 ; i<index ; i++ )
                    for( int j=0 ; j<index ; j++ )
                        if( path[i][j] > path[i][k]+path[k][j] )
                            path[i][j] = path[i][k]+path[k][j];
        }

        int Calculate()
        {
            sum = 0;
            for( int i=0 ; i<index ; i++ )
                for( int j=i ; j<index ; j++ )
                {
                    if( path[i][j] == 0x0FFFFFFF )sum -= 1;
                    else sum += path[i][j];
                }
            return sum;
        }

        int path[ MAX_File ][ MAX_File ] , index , sum;
        File *Files[ MAX_File ];
};

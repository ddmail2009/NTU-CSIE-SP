#include<stdio.h>
#include<time.h>
#include<stdlib.h>

int main(int argc, char* argv[])
{
    int amount = 100;
    if( argc == 2 )
        amount = atoi( argv[1] );
    
    int i=0;

    srand( time(NULL) );
    FILE *pFile = fopen("num.txt","w");

    for(i=0; i<amount; ++i)
        fprintf(pFile, "%09d\n", rand()%100000000);
    fclose(pFile);
}

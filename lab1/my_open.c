#include "stdio.h"
int main (argc, argv)
int argc;
char* argv[];
{
    char f[100];
    scanf("%s", f);
        if (fopen(f,"r") == NULL) {
            printf("%s: неудача при попытке открыть файл %s",argv[0], f);
        }
        else {
            printf("%s: файл %s открыт",argv[0], f);
        }
 
    return 0;
}

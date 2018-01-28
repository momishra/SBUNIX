#include<stdio.h>
#include<../include/systemcall.h>

#include<stdlib.h>
int main(int argc,char *argv[],char **env){

printf("%s",argv[0]);
char *s = "test.txt";
readf(s);
exit(0);
return 0;
}
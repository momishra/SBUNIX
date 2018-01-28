#include<stdio.h>
#include<../include/systemcall.h>
#include<unistd.h>
#include<stdlib.h>
int main(int argc,char *argv[],char **env){
   printf("!!! Welcome to SBUNIX !!!\n");
  char str[100];
  char *ch;
  //list();
  //printf("Enter string :\n");
  ch = gets(str);
  printf("\n%s\n",ch);
  
  //readf("etc/test.txt");
  //int x;
  //x = get_pid();
  //printf("%d\n",x);
  //ps();
  //my_execve("bin/execve_test",NULL,NULL);
  ps();
  
  int a = fork();
  //printf("%d\n",a);
  //ps();
  
  if (a==0){
    //printf("id returned ");
   my_execve("bin/ls",NULL,NULL);
    //ps();
    printf("in child");
    exit(0);
    }
  else{
    //printf("in parent\n");
    user_yield();
    //my_execve("bin/execve_test",NULL,NULL);
  
  }
  //printf("%d\n",a);
  //ps();
  printf("aditya\n");
  while(1);
	return 0;
}

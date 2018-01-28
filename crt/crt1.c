#include <stdlib.h>
#include "../include/systemcall.h"
void _start(int argc, char **argv, char **env) {
  // call main() and exit() here
  /*char *envp[]={
  "HOME=/home/adisrivastav",
  "PATH=/home/adisrivastav/fall2017/bin/sbush",
  "TZ=UTC0",
  "USER=adisrivastav",
  "LOGNAME=",
  0
  };*/
	//main(0,NULL,envp);
 //int argc = 1;
	//char* argv[10];
	//char* envp[10];
	//int res;
   main(argc, argv,env);
	//exit(0);
}

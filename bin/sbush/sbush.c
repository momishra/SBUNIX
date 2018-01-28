#include<stdio.h>
#include<../include/systemcall.h>
#include<../include/str.h>
#include<../include/unistd.h>
#include<stdlib.h>
char str[50];
char shell_name[50] = "sbush> ";

void echoFunction(char *str){
  printf("%s\n",str);
}

void list_file(){
  //my_execve("bin/ls",NULL,NULL);
  
  int x= fork();
  if(x==0){
  my_execve("bin/ls",NULL,NULL);
  exit(0);
  //list();
  }
  else{
  user_yield();
  }
  
}

void list_process(){

  int x = fork();
  if(x==0){
  //ps();
  my_execve("bin/ps",NULL,NULL);
  exit(0);
  }
  else{
  user_yield();
  }
}

void print_file(char *argument){

  readf(argument);
  
}
void changeDir(char *pathToBeChanged)
{
  /*int x = fork();
  if(x==0){
  //ps();
  my_execve("bin/cat",NULL,NULL);
  exit();
  }
  else{
  user_yield();
  }*/

  cd(pathToBeChanged);
}

void slp(int t){
sleep(t);
}

void presentWorkingDir(){
  int x = fork();
  if(x==0){
  //pwd();
  my_execve("bin/pwd",NULL,NULL);
  exit(0);
  }
  else
  {
  user_yield();
  }
}
void executeCommand(char *cmd){
	char *command = str_token(cmd, " ");
    char lscommand[] =  "ls";
    char cdcommand[] =  "cd";
    char pwdcommand[] = "pwd";
    char echocommand[] = "echo";
    //char exitcommand[] = "exit";
    char catcommand[] = "cat";
	//char killcommand[] = "kill";
	char pscommand[] = "ps";
 char sleepcommand[] =  "sleep";



	if(strcomp(command,lscommand)==0){
		list_file();
	}
 else if(strcomp(command,sleepcommand)==0){
		char *argument = str_token(NULL, " ");
     int i = stroi(argument);
		slp(i);
	}
	else if(strcomp(command,catcommand)==0){
		char *argument = str_token(NULL, " ");
		print_file(argument);
	}
	else if(strcomp(command,echocommand)==0){
		char *echoString = str_token(NULL, "\n");
		echoFunction(echoString);
	}
	else if(strcomp(command,cdcommand)==0){
		char *pathToBeChanged = str_token(NULL, " ");
		if(pathToBeChanged != NULL){
			changeDir(pathToBeChanged);
		}
	}/*
	else if(strcomp(command,exitcommand)==0){
		exit(0);
	}*/
 
	else if(strcomp(command, pwdcommand)==0){
		presentWorkingDir();
	}
 /*
	else if(strcomp(command,,killcommand)==0){
		char *a1 = str_token(NULL," ");
		char *a2 = str_token(NULL, " ");
		uint64_t pid = stroi(a2);
		kill_process(pid);
	}*/
	else if(strcomp(command,pscommand)==0){
		list_process();
	}
}

int main(int argc,char *argv[],char **env){
	printf("!!! Welcome to SBUNIX !!!\n");
	while(1){
		printf("%s ",shell_name);
		char *ch = gets(str);
		if(ch == NULL){
			puts("Error reading command from user\n");
			continue;
		}
		char *cmd = str_token(str,"\n");
		executeCommand(cmd);
   //printf("%s\n",cmd);
	}
	return 0;
}
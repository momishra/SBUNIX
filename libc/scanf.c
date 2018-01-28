//put entire keyboard related thing here
#include <sys/defs.h>
#include <sys/stdarg.h>
#include<sys/kprintf.h>
#include<sys/types.h>
#include <stdio.h>
#define stdin 0
#define stdout 1
#define stderr 2

/*
int read_stdin(void *buf,int count){
  takeinput =1;
  __asm__ volatile("sti"); //now take input from user
  inputlen =0;
  while(takeinput ==1);
  if(count < inputlen) inputlen = count;
  memcpy((void*)buf,(void*)inputbuf,inputlen);
  while(inputlen >=0){
  inputbuf[inputlen] = '\0';
  inputlen --;
  }
  int len = inputlen;
  inputlen =0;
  return len;
}


//reading from file
int sys_read(uint64_t fd, char* buf, uint64_t len){
  if(fd == stdin){
    return read_stdin((void*)buf,len);
  }
}


int scanf(const char *fmt,...){
  va_list arg;
  int count= 0;
  char c;
  int *num = NULL;
  char *string;
  va_start(arg,fmt);
  while(*fmt != 0){
    while(*fmt == ' '){
      fmt++;
    }
    if(*fmt == '%'){
      ++fmt;
    
    if(*fmt == '\0') break;
    
    switch(*fmt){
      case 'd':{
        num = (int*)va_arg(arg,char*);
        char buffer[10] = {'\0'};
        int i;
        sys_read(0,buffer,sizeof(int));
        while(buffer[i] != '\0' && buffer[i] != '\n'){
          i++;
        }
        i--;
        int val=0,io=1;
        while(i>=0){
          buffer[i] = buffer[i] -48;
          val =val+ buffer[i]*io;
          i--;
          io = io*10;
        }
        *num = val;
        count++;
        break;
      }
      case's':
        string = (char*)va_arg(arg,char*);
        sys_read(0,string,1024);
        count++;
        break;
      case 'c':
        c = va_arg(arg,char);
        sys_read(0,c,sizeof(c));
        count++;
        break;
      case 'x':
        //read from stdin
        count++;
        break;
    }
    }
    fmt++;
    }    
    return count;
}
*/
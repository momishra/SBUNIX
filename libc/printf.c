#include <sys/stdarg.h>
#include <sys/defs.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include<unistd.h>
#include <sys/str.h>

//char *ptr = (char*)(0xFFFFFFFF800B8000);

int printf(char *fmt, ...)
{
va_list arg;
int a,x;

char str[1024];
for(int i=0;i<1024;i++) str[i] = 0;
int temp;
char c;
char *s;
void *p;
int pos=0;
va_start(arg,fmt);
char *printmsg;
printmsg = fmt;
while(*printmsg != 0){
  
  if(*printmsg != '%'){
    str[pos++] = *printmsg++;
    continue;
  }
  
	switch(*++printmsg)
	{
		case 'd':
		{
			int arr[7] = {0,0,0,0,0,0,0};
      int count= 0;
			a = va_arg(arg,int);
			if(a==0){
       str[pos++] = '0';
			}
			while(a!=0)
			{
				temp = a%10;
				arr[count++]=temp;
				a = a/10;
			}
				
			for(int ki=count-1;ki>=0;ki--){
        str[pos++] = arr[ki]+'0';
			}
			printmsg = printmsg+1;
			break;
		}
		case 'c':{
			c = va_arg(arg,int);
       str[pos++] = c;
			printmsg = printmsg+1;
			break;
      }
		case 's':
   {
			s = va_arg(arg,char*);
			while(*s)
			{
        str[pos++] = *s;
				s=  s+1;
			}
			printmsg = printmsg+1;
			break;
      }
		case 'p':
		{
			p = va_arg(arg,void*);
			unsigned long *tem = (unsigned long *)p;
			unsigned long x = (unsigned long)tem;
			int res[100];
           		int temp,i=0;
            		while(x!=0){
				temp = x%16;
               			 if(temp<10){
                   		 res[i]=temp+48;
                   		 i++;
                	}
			if(temp>10){
                    	res[i]=temp+55;
                   	 i++;
                	}
               		 x = x/16;
           		 }
      str[pos++] = '0';
      str[pos++] = 'x';
            		for(int j=i-1;j>=0;j--){
                       str[pos++] = res[j];
			            }
			printmsg = printmsg+1;
			break;
		}
		case 'x':
		{
			x = va_arg(arg,long int);
			int res[100];
        		int temp,i=0;
			if(x == 0){
        str[pos++] = '0';
			}
        		while(x!=0){
				            temp = x%16;
               			 if(temp<10){
                   			 res[i]=temp+48;
                   			 i++;
                		}
               			 if(temp>10){
                   			 res[i]=temp+55;
                   			 i++;
                		}

               			 x = x/16;
        		}
      str[pos++] = '0';
      str[pos++] = 'x';

        		for(int j=i-1;j>=0;j--){
        		  str[pos++] = res[j];
			}	
			printmsg = printmsg+1;
			break;
		}
   default:
       printmsg = printmsg +1;
       continue;
	}
}
str[pos] = '\0';
va_end(arg);
int len =0;
//write(1,str,strlength(str));

while(len < strlength(str)){
  __syscall1(1, (uint64_t)str[len]);
  
  len++;
}
return 0;
}

int puts(const char *s){
  while(*s != '\0'){
    printf("%c", *s++);
  }
  return 1;
}

char *gets(char *str){
  //syscall to memcpy whatever is there in inputbuf and write it to the str
  //printf("Reading input from the stdin\n");
  str = (char*)__syscall1(7,(uint64_t)str);
  return str;
}
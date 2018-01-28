#include <sys/kprintf.h>
#include <sys/stdarg.h>
//char *pt = (char*)0xFFFFFFFF800B8F6A;
char *pt = (char*)0xB8F6A;
//#include"../include/str.h"
/*
void to_hex(long int x, unsigned short **ptr)
{
	int res[100];
	int temp,i=0;
	while(x!=0)
	{
		temp = x%16;
		if(temp<10)
		{
			res[i]=temp+48;
			i++;
		}
		if(temp>10)
		{
			res[i]=temp+55;
			i++;
		}
			
		x = x/16;
	}
	for(int j=0;j<i;j++)
	*ptr = (char)res[j];
	*ptr+=1;
}
*/
void kprintf_sp(const char *fmt, ...)
{
//static unsigned short *ptr = (unsigned short*)0xb8000; //beginning of the VGA buffer memory address
va_list arg;
int a,count=0,temp,x;
char c;
char *s;
void *p;
va_start(arg,fmt);
while(*fmt!='\0')
{

while(*fmt)
{
	char exp = *fmt;
	if(exp == '%')
	{
		fmt +=1;
		exp = *fmt++;
		switch(exp)
		{
			case 'd':
			{
				int arr[7];
				a = va_arg(arg,int);
				while(a!=0)
				{
					temp = a%10;
					arr[count++]=temp;
					a = a/10;
				}
				for(int i=count-1;i>=0;i--){
				*pt = arr[i]+'0';
				pt = pt +2;}
				break;
			}
			case 'c':
				c = va_arg(arg,int);
				*pt = c;
				pt = pt+2;
				break;
			case 's':
				s = va_arg(arg,char*);
				while(*s)
				{
					*pt = *s;
					pt = pt+2;
					s=  s+1;
				}
				break;
			case 'p':
				{
				p = va_arg(arg,void*);
				//while(**p)
				unsigned long *tem = (unsigned long *)p;
				unsigned long x = (unsigned long)tem;
				 int res[100];
                                int temp,i=0;
                                while(x!=0)
                                {
                                        temp = x%16;
                                        if(temp<10)
                                        {
                                                res[i]=temp+48;
                                                i++;
                                        }
                                        if(temp>10)
                                        {
                                                res[i]=temp+55;
                                                i++;
                                        }

                                x = x/16;
                                }
                                for(int j=i-1;j>=0;j--){
                                *pt = res[j];
				pt=pt+2;}
				break;
				}
			case 'x':
			{
				x = va_arg(arg,long int);
				//*ptr++ = x+'0';
			//	to_hex(x,&ptr);
				int res[100];
        			int temp,i=0;
        			while(x!=0)
        			{
                			temp = x%16;
                			if(temp<10)
                			{
                        			res[i]=temp+48;
                        			i++;
                			}
                			if(temp>10)
                			{
                        			res[i]=temp+55;
                        			i++;
                			}

                		x = x/16;
        			}
        			for(int j=i-1;j>=0;j--){
        			*pt = res[j];
				pt=pt+2;}
				break;
			}
		}
	}
	*pt = *fmt;
	pt = pt+2;
	fmt = fmt+1;	
}
pt =(char*)0xB8F6A;
}
va_end(arg);
}


#include <sys/kprintf.h>
#include <sys/stdarg.h>
#include <sys/defs.h>
char *ptr = (char*)(0xFFFFFFFF800B8000);
//char *ptr = (char*)(0xB8000);
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

/*
char ar[129]={0};


int strlength(const char *str)
{
const char *x;
for(x=str;*x;++x);
return (x-str);
}

char *itoa(long long val, int base, char *str, int len) {
	int neg = 0;
	int i;
	if(str == '\0') {
		return '\0';
	}
	if(base < 2 || base > 36 || len < 1) {
		return str;
	}
	if(val == 0) {
		str[0] = '0';
		str[1] = '\0';
		return str;
	}
	
	if(val < 0 && base == 10) {
		neg = 1;
		val *= -1;
	}
	for(i = 0; i < len && val != 0; i++) {
		char c = (char)(val % base);
		if(c < 10) {
			c += '0';
		} else {
			c += 'a' - 10;
		}
		str[i] = c;
		val /= base;
	}
	if(neg) {
		str[i++] = '-';
	}
	str[(i < len)? i : len] = '\0';

	return str;
}
*/
void kprintf(char *fmt, ...)
{
va_list arg;
int a,x;
int count= 0, temp;
char c;
char *s;
void *p;
va_start(arg,fmt);
char *printmsg;
printmsg = fmt;

while(*printmsg != 0){
	if(*printmsg != 37 && *printmsg != '\n'){ // 37 is for %
		*ptr = *printmsg;
		ptr = ptr + 2; 
		printmsg++;
		continue;
	}	

	if(*printmsg == '\n'){
		uint64_t curr = (uint64_t)(ptr);
		uint64_t start = (uint64_t)0xFFFFFFFF800B8000;
		uint64_t offset = 160 - (curr - start)%160;
		ptr = ptr + offset;
		printmsg++;
		continue;
	}

	switch(*++printmsg)
	{
		case 'd':
		{
			int arr[7];
			a = va_arg(arg,int);
			if(a==0){
				*ptr = '0';
                		ptr = ptr + 2;

			}
			while(a!=0)
			{
				temp = a%10;
				arr[count++]=temp;
				a = a/10;
			}
				
			for(int i=count-1;i>=0;i--){
				*ptr = arr[i]+'0';
				ptr = ptr +2;
			}
			printmsg = printmsg+1;
			break;
		}
		case 'c':
			c = va_arg(arg,int);
			*ptr = c;
        	        ptr = ptr + 2;
			printmsg = printmsg+1;
			break;
		case 's':
			s = va_arg(arg,char*);
			while(*s)
			{
				*ptr = *s;
                		ptr = ptr + 2;
				s=  s+1;
			}
			printmsg = printmsg+1;
			break;
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
			*ptr = '0'; ptr= ptr+2;
                        *ptr = 'x'; ptr = ptr+2;
            		for(int j=i-1;j>=0;j--){
               		 *ptr = res[j];
                         ptr = ptr + 2;
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
				*ptr = '0';
                                ptr = ptr + 2;
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
			*ptr = '0'; ptr= ptr+2;
			*ptr = 'x'; ptr = ptr+2;

        		for(int j=i-1;j>=0;j--){
        			*ptr = res[j];
                                ptr = ptr + 2;
			}	
			printmsg = printmsg+1;
			break;
		}
	}
}


/*while(*fmt!='\0')
{
while(*fmt)
{
	char exp = *fmt;
	if(exp == "\\")
	{
		fmt = fmt +1;
		exp = *fmt++;
		switch(exp)
		{
			case 'n':
			{
				*tmp = *ptr - 0x80A0;
				if(*tmp < 0)
				*ptr + *tmp;
			}
		}
	}
*/
/*	if(exp == '\\'){
		fmt +=1;
		exp = *fmt++;
		switch(exp){
			case 'n':
			{
				unsigned long int curr = (unsigned long int)*ptr;
				unsigned long int start = (unsigned long int)0xFFFFFFFF800B8000;
				unsigned long int offset = 160 - (curr - start)%160;
				*ptr = *ptr + offset;
				break;
			}
			case 'r':
			{
				
			}
			
		}
		
	}
		
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
				//itoa((long long) a, 10, ar, sizeof(ar));
				//int len = strlength(ar);
				//puts(ar,len);
				
				if(a==0){
					 *ptr = '0';
					  ptr = ptr+2;
				}
				while(a!=0)
				{
					temp = a%10;
					arr[count++]=temp;
					a = a/10;
				}
				
				for(int i=count-1;i>=0;i--){
				*ptr = arr[i]+'0';
				ptr = ptr+2;
				//arr[i] = arr[i]+'0';
			//	puts((arr[i]),1);
				}
				break;
			}
			case 'c':
				c = va_arg(arg,int);
				*ptr = c;
				ptr = ptr+2;
				break;
			case 's':
				s = va_arg(arg,char*);
				while(*s)
				{
					*ptr = *s;
					ptr = ptr+2;
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
                                *ptr = res[j];
				ptr=ptr+2;}
				break;
				}
			case 'x':
			{
				x = va_arg(arg,long int);
				*ptr++ = x+'0';
			//	to_hex(x,&ptr);
				int res[100];
        			int temp,i=0;
				if(x == 0){
					*ptr = '0';
					ptr=ptr+2;
				}
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
        			*ptr = res[j];
				ptr=ptr+2;}
				break;
			}
		}
	}
	*ptr = *fmt;
	ptr = ptr+2;
	fmt = fmt+1;	
}
ptr += 2;
}*/
va_end(arg);
}

void panic(char *message)
{
    kprintf(message);
    __asm__("hlt");
}

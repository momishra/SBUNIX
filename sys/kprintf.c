#include <sys/kprintf.h>
#include <sys/stdarg.h>
#include <sys/defs.h>
char *ptr = (char*)(0xFFFFFFFF800B8000);
uint16_t *videoMemory = (uint16_t*)VGA_VIRT_ADDR;
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

int putc(char c){
  do_scroll();
  if(c == '\n')
    kprintf("\n");
  else{
    *ptr = c;
    ptr = ptr+2;
  }
  return 0;
}

void put_ch(char c, int col, int row){
  do_scroll();
  int pos = row*79 + col;
  videoMemory[pos] = c; //yeh jugaad ka intezaam dekhna h :D
}

void backspace(){
  uint64_t cur = (uint64_t)ptr;
  uint64_t start = (uint64_t)0xFFFFFFFF800B8000;
  if ((cur - start)>160)
  {
    ptr = ptr-2;
    *ptr= '\0';
    ptr = ptr+2;
    ptr= ptr-2;
}
}

void put_time(char c, int col, int row){
   int pos = row*79 + col;
  videoMemory[pos] = c; 
}

void timeprint(int time){
  int row = 24;
  int col = 79;
  //uint64_t last = (uint64_t)&videoMemory[23*160];
  //kprintf("last:%p",last);
  //for fun :p
  put_ch('T',col-14,row);
  put_ch('I',col-13,row);
  put_ch('M',col-12,row);
  put_ch('E',col-11,row);
  put_ch('R',col-10,row);
  put_ch(' ',col-9,row);
  
  int hr = time/3600;
  time = time%3600;
  int min = time/60;
  time = time%60;
  int sec = time;
  
  //print secs to console
  char c1 = (char)(sec%10) + '0';
  char c2 = (char)(sec/10) + '0';
  
  put_time(c1,col,row);
  put_time(c2,col-1,row);

  //print mins to console
  c1 = (min%10) + '0';
  c2 = (char)(min/10) + '0';
  
  put_ch(':',col-2,row);
  put_ch(c1,col-3,row);
  put_ch(c2,col-4,row);
  
  //print hours to console
  c1 = (char)(hr%10) + '0';
  c2 = (char)(hr/10) + '0';
  
  put_ch(':',col-5,row);
  put_ch(c1,col-6,row);
  put_ch(c2,col-7,row);
  
  /*
  //for fun :p
  put_time('T',col-14,row);
  put_time('I',col-13,row);
  put_time('M',col-12,row);
  put_time('E',col-11,row);
  put_time('R',col-10,row);
  put_time(' ',col-9,row);
  
  int hr = time/3600;
  time = time%3600;
  int min = time/60;
  time = time%60;
  int sec = time;
  
  //print secs to console
  char c1 = (char)(sec%10) + '0';
  char c2 = (char)(sec/10) + '0';
  
  put_time(c1,col,row);
  put_time(c2,col-1,row);

  //print mins to console
  c1 = (min%10) + '0';
  c2 = (char)(min/10) + '0';
  
  put_time(':',col-2,row);
  put_time(c1,col-3,row);
  put_time(c2,col-4,row);
  
  //print hours to console
  c1 = (char)(hr%10) + '0';
  c2 = (char)(hr/10) + '0';
  
  put_time(':',col-5,row);
  put_time(c1,col-6,row);
  put_time(c2,col-7,row);
  */
}

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
     do_scroll();
		*ptr = *printmsg;
		ptr = ptr + 2; 
		printmsg++;
		continue;
	}	

	if(*printmsg == '\n'){
    do_scroll();
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
      do_scroll();
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
      do_scroll();
			c = va_arg(arg,int);
			*ptr = c;
       ptr = ptr + 2;
			printmsg = printmsg+1;
			break;
		case 's':
     do_scroll();
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
      do_scroll();
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
      do_scroll();
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

void load_screen(){
  uint64_t i=0;
  for(;i< (160*25);i++){
    videoMemory[i]=' ';
  }
}
/*
void do_scroll(){
  uint64_t line=0,nextline=0;
  uint64_t curr = (uint64_t)(ptr);
  
  uint64_t end_ind = 6*160;
  uint64_t last = (uint64_t)&videoMemory[end_ind];
  //kprintf("last:%p",last);
  //uint64_t start = (uint64_t)0xFFFFFFFF800B8000;
	//uint64_t diff = curr - start;
   uint64_t diff = last - curr;
   
 if(diff == 2*160){ //as 24th line is for timer
 kprintf("here\n");
   for(int i=1; i<6;i++){
    for(int j=0;j<80;j++){
      line = ((i-1)*80+ j)*2;
      nextline = ((i*80)+j)*2; //next line
      videoMemory[line++] = videoMemory[nextline++];
      videoMemory[line] = videoMemory[nextline];
    }
  }
  ptr = ptr -160; //move 1 line back
  ptr = ptr -160; //creating one line for upcoming chars
 }
}*/


void do_scroll(){
  uint64_t line=0,nextline=0;
  uint64_t curr = (uint64_t)(ptr);
  uint64_t start = (uint64_t)0xFFFFFFFF800B8000;
	uint64_t diff = curr - start;
  
 if(diff == 25*160){ //as 24th line is for timer
 
   for(int i=1; i<24;i++){
    for(int j=0;j<80;j++){
      line = ((i-1)*80+ j)*2;
      nextline = ((i*80)+j)*2; //next line
      videoMemory[line++] = videoMemory[nextline++];
      videoMemory[line] = videoMemory[nextline];
    }
  }
  ptr = ptr -160; //move 1 line back
  ptr = ptr -160; //creating one line for upcoming chars
 }
}

inline void outb(uint16_t port, uint8_t data){
  __asm__ volatile("outb %0,%1;"::"a"(data),"d"(port));

}
/*
void backspace(){
  kprintf("changing backspace");
  uint64_t curr = (uint64_t)ptr;
  uint64_t start = (uint64_t)0xFFFFFFFF800B8000;
  if((curr - start) > 160){
    ptr = ptr -2;
    *ptr = '\0';
    ptr = ptr+2;
    ptr = ptr-2;
    curr = (uint64_t)ptr;
    unsigned short pos = (curr - start)/2;
    outb(0x3D4,0x0F);
    outb(0x3D5,(unsigned char)(pos&0xFF));
    outb(0x3D4,0x0E);
    outb(0x3D5,(unsigned char)((pos>>8)&0xFF));
  }
}
*/
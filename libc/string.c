#include<stdio.h>
#include<../include/systemcall.h>

#define MAX_BUFFER 1024



uint64_t stroi(const char *s)
{
	uint64_t i;
	i=0;
	while(*s >= '0' && *s <= '9')
	{
		i = i *10 + (*s - '0');
		s++;
	}
	return i;
}


int pow(int a, int pow){
	int result = 1;
	int i=0;
	for(i=0;i<pow;i++){
		result *= a;
	}
	
	return result;
}


int putchar(int c)
{
  char k = c;
  my_write(1,&k,sizeof(k));
  return c;
}

/*
int puts(const char *s)
{
  for( ; *s; ++s) if (putchar(*s) != *s) return EOF;
  return (putchar('\0') == '\0') ? 0 : EOF;
}*/

extern char *str_cat(char *dest, char *src)
{
	char *fin = dest;
	while(*dest!='\0'){
	dest++;
	}

	while(*src!='\0')
	{
		*dest = *src;
		dest++;
		src++;
	}
	return fin;
}

extern char *str_token(char *str, char*delim)
{
	static char *temp;
	static int cur;
	int i, beg;
	if (str!='\0')
	{
	temp= str;
	cur = 0;
	}
	beg = cur;
 	while(temp[cur] != '\0')
	{	
		i=0;
		while(delim[i]!= '\0')
		{
			if(temp[cur]==delim[i])
			{
				temp[cur]='\0';
				cur = cur + 1;
				if (temp[beg]!= '\0')
				{
				 return &temp[beg];
				}
				else
				{
				 beg = cur;
				 cur--;
				 break;
				}
			}
			i++;
		}
		cur= cur+1;
	}
	temp[cur]='\0';
	if(temp[beg]!= '\0')
	return &temp[beg];
	else
	return '\0';
}


int strcomp(char *str1,char *str2){
    while(*str1 == *str2){
      if(*str1 == '\0' || *str2 == '\0'){
	break;
      }
      str1++;
      str2++;
     }
    
    if(*str1 == '\0' && *str2 == '\0'){

      //puts("String matched");
      return 0;
    }else{
      //puts("Strings do not match");
      return -1;      
      }
}

int strlength(const char *str)
{
const char *x;
for(x=str;*x;++x);
return (x-str);
}


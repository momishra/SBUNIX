#include<stdio.h>
#include<sys/defs.h>
#include<sys/str.h>

uint64_t atoi(const char *s)
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


uint64_t oct_dec(uint64_t x)
{
	uint64_t i=0, dec = 0;
	while(x != 0){
		dec = dec + (x%10) * pow(8,i++);
		x = x/10;
	}
	return dec;
}


void *memset(void *string, int val, size_t length)
{
	char *str = string;
	while(length>0)
	{
		*str = val;
		str++;
		length--;
	}
	return string;

}

void *memcpy(void *dest,void *src, size_t count){
  const char *sp = (const char*)src;
  char *dp = (char*)dest;
  
  if(sp < dp && sp + count > (char*)dest){
    sp +=count;
    dp += count;
    while(count-- > 0){
      *--dp = *--sp;
    }
  }else{
    while(count != 0){
    *dp++ = *sp++;
    count--;
  }
  }
  return dest;
}

#ifndef _STR_H
#define _STR_H

int pow(int n, int pow);
char *str_token(char *str, char *delim);
int strcomp(char *str1, char *str2);
int strlength(const char *str);
char *str_cat(char *dts, char *src);
char *strcopy(char *dest, const char *src);
char *strncopy(char *dest, const char *src, int len);
uint64_t stroi(const char *s);
int strncomp(char *str1, char *str2,uint64_t len);
#endif

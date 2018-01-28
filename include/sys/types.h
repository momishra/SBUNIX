#ifndef TYPES_H
#define TYPES_H

#define FLOOR(m,n)({uint64_t __m = (uint64_t)(m);(__typeof__(m))(__m - __m%(n));})

#define CEIL(m,n)({uint64_t __n =(uint64_t) (n);(__typeof__(m))(FLOOR((uint64_t)(m)+__n - 1,__n));})
void *memset(void *string, int val, size_t length);
void *memcpy(void *dest, const void *src, size_t count);

uint64_t atoi(const char *s);
uint64_t oct_dec(uint64_t x);

#endif

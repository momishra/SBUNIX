#ifndef __KPRINTF_H
#define __KPRINTF_H
//static unsigned short *ptr = (unsigned short*)0xb8000;
#define VGA_VIRT_ADDR 0xFFFFFFFF800B8000;
static volatile char inputbuf[1024];
static volatile int inputlen =0;
//static volatile int newline=0;
extern int newline;
static volatile int takeinput =0;

char *itoa(long long val, int base, char *str, int len);
int strlength(const char *str);
//void puts(const char *text, int count);
void panic(char *message);
//void kprintf(const char *fmt, ...);
void kprintf(char *fmt, ...);
void kprintf_sp(const char *fmt, ...);
void kprintf_kb(const char *fmt, ...);
void put_ch(char c, int col, int row);
void timeprint(int time);
int putc(char c);
void do_scroll();
void load_screen();
void backspace();
#endif

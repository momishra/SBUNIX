#include<stdio.h>
#include <systemcall.h>
#include<sys/syscall.h>
#include<unistd.h>
#include<stdlib.h>
#define __NR_read 0
#define __NR_write 1
#define __NR_open 2
#define __NR_close 3
#define __NR_map 9
#define __NR_pipe 22
#define __NR_dup2 33
#define __NR_pid 39
#define __NR_fork 57
#define __NR_exec 59
#define __NR_exit 60
#define __NR_wait 61
#define __NR_dent 78
#define __NR_cwd 79
#define __NR_chdir 80
#define __NR_mkdir 83

#define MAX_BUFFER 1024
#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR 2
#define O_CREAT 0x0100
#define O_TRUNC 0x0200

void list(){
  __syscall0(9);
}
int write(uint64_t fd, uint64_t addr, int len){
return __syscall3(15,fd,(uint64_t)addr, len);
}
void readf(char *filename){
  __syscall1(11,(uint64_t)filename);
}

extern int get_pid(){
  return __syscall0(2);
  
}

void cd(char* dir_name){

__syscall1(12, (uint64_t)dir_name);
}

void pwd(){
__syscall0(13);
}

void sleep(int t){
__syscall1(14 ,t);
}

void user_yield(){
/*
  uint64_t a;
  __asm__ volatile(
  "int $0x81":"=a"(a):"0"(n)
    //"int $0x81;"
  );
  return a;*/
  __syscall0(8);
}
extern void my_execve(char *filename, char *argv[], char *envp[]){
  __syscall3(3,(uint64_t)filename,(uint64_t)argv,(uint64_t)envp);
  exit(0);
};
 int fork(){
  return __syscall0(5);
}

extern void ps(){
  __syscall0(6);
}




void exit(int status)
{
 __syscall0(10);
}

extern int my_execl(char *argv[])
{
	int ret;
	__asm__ volatile
	(
	"syscall"
	:"=a"(ret)
	:"a"(__NR_exec),"D"(argv[0]),"S"(argv), "d"(0)
	:"rcx","r11","cc"
	);
return ret;

}

extern int my_dup2(int oldfd, int newfd)
{
	int ret;
	__asm__ volatile
	(
		"syscall"
		:"=r"(ret)
		:"0"(__NR_dup2), "D"(oldfd), "S"(newfd)
	);
return ret;
}
extern int my_pipe(int fd[2])
{
	int ret;
	__asm__ volatile
	(
		"syscall"
		:"=a"(ret)
		:"0"(__NR_pipe), "D"(fd)
	);
 return ret;
}

extern int my_fork()
{
	int ret;
	__asm__ volatile
	(
		"syscall"
		:"=a"(ret)
		:"0"(__NR_fork)
	);
 return ret;
}

extern ssize_t my_write(int fd, const void *buf, size_t size)
{
	 ssize_t ret;
    __asm__ volatile
    (
        "syscall"
        : "=a" (ret)
        : "0"(__NR_write), "D"(fd), "S"(buf), "d"(size)
        : "cc", "rcx", "r11", "memory"
    );
    return ret;
}



extern int my_wait4(int pid,int* status, int opt ){
  int ret;
  __asm__ volatile
    (
     "syscall"
     :"=a"(ret)
     : "0"(__NR_wait),"D"(pid),"S"(&status),"d"(opt)
     );
    return ret;
} 

extern int my_open(char *filename, long int per)
{
	 int ret;
        __asm__ volatile
        (
                "syscall"
                : "=a"(ret)
                : "0"(__NR_open), "D"(filename), "S"(per)

);
return ret;
}

extern ssize_t my_read(int fd, char buf[], ssize_t s)
{
	ssize_t ret;
    __asm__ volatile
        (
		"syscall"
        :"=a"(ret)
        :"0"(__NR_read), "D"(fd), "S"(buf), "d"(s)
		);
	return ret;
}

extern char *my_cwd(char buf[], ssize_t s)
{
	 char *str;
        __asm__ volatile
        (
                "syscall"
                :"=a"(str)
                :"0"(__NR_cwd), "D"(buf),"S"(s)
        );
return str;
}

extern int change_dir(char *path)
{
	int ret;
	__asm__ volatile
	(
		"syscall"
		:"=a"(ret)
		:"0"(__NR_chdir), "D"(path)
	);
 return ret;
}

extern int get_dents(int fd, char buf[], ssize_t s)
{
	int ret;
    __asm__ volatile
        (
           "syscall"
           :"=a"(ret)
           :"0"(__NR_dent),"D"(fd),"S"(buf),"d"(s)
    );
	return ret;
}

extern struct d_entry *my_readdir(char *cwd)
{
	struct d_entry* d ;
    d = (struct d_entry*) {0} ;
	int fd = my_open(cwd,00004);
	char c[1024];
	int direntries = get_dents(fd,c,sizeof(c));
	static int i;
	d = (struct d_entry*)(c+i);

	i = i + d->d_reclen;
	if(i<=direntries)
		return d;
	else
		return '\0';
}

extern int my_close(int fd)
{
	int ret;
	__asm__ volatile
	(
		"syscall"
		:"=a"(ret)
		:"0"(__NR_close),"D"(fd)
	);
return ret;
}



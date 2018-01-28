#ifndef _SYSTEMCALL_H
#define _SYSTEMCALL_H
#include "sys/defs.h"

typedef struct d_entry {
        long d_ino;
        unsigned long d_off;
        unsigned short d_reclen;
        char d_name[];
}*d;

void readf(char *filename);
void list();
//void exit();
void my_execve(char *filename, char *argv[], char *envp[]);
int my_execl(char *arg[]);
//int fork();
void user_yield();
ssize_t my_write(int fd, const void *buf, size_t size);
void cd(char *);
void pwd();
//void sleep();
int get_pid();
void ps();
int my_open(char *filename, long int per);

ssize_t my_read(int fd, char buf[], ssize_t s);

char *my_cwd(char buf[], ssize_t s);

int get_dents(int fd,char buf[], ssize_t s);

int change_dir(char *path);

struct d_entry *my_readdir(char *cwd);

int my_wait4(int pid,int *status, int opt);

int my_close(int fd);



int my_pipe(int fd[2]);

int my_dup2(int oldfd, int newfd);

#endif

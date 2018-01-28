#ifndef _TARFS_H
#define _TARFS_H

#include<sys/defs.h>

extern char _binary_tarfs_start;
extern char _binary_tarfs_end;

struct posix_header_ustar {
  char name[100];
  char mode[8];
  char uid[8];
  char gid[8];
  char size[12];
  char mtime[12];
  char checksum[8];
  char typeflag[1];
  char linkname[100];
  char magic[6];
  char version[2];
  char uname[32];
  char gname[32];
  char devmajor[8];
  char devminor[8];
  char prefix[155];
  char pad[12];
};

#define REGULAR 0
#define DIRECTORY 5

typedef struct tarfs_obj_{
  char name[100];
  int pindex;
  uint64_t size;
  uint64_t start;
  uint8_t filetype; //enum created
}tarfs_obj;


typedef struct tarfs_file_{
    int index;//This field will be index in the tarfs table
    char name[100];
    int size;
    char filetype;
    uint64_t start;
    int is_fs;
}tarfs_file;


tarfs_obj tfs[512]; 
int explore_tarfs(tarfs_obj *tfs);
void tarfs_init();
int get_pindex_file(char *root, uint8_t f_type);
tarfs_obj* tarfs_file_open(char *filename);
uint64_t tarfs_file_read(tarfs_obj *file, int size, uint64_t buf);
uint64_t tarfs_close_file();
uint64_t tarfs_open_dir(char *dir);
uint64_t tarfs_read_dir(uint64_t dir_off);
uint64_t tarfs_close_dir(uint64_t dir_off);
//open, read, close, opendir, readdir, closedir

#endif

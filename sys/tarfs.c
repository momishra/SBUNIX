#include<sys/tarfs.h>
#include<sys/str.h>
#include<sys/kprintf.h>
#include <sys/types.h>
#include <sys/pmap.h>

tarfs_obj* tarfs_file_open(char *filename){
  tarfs_obj *entry = (tarfs_obj*)kmalloc(sizeof(tarfs_obj));
  tarfs_obj obj;
  int i=0;
  while(strlength(tfs[i].name)!=0){
    memset(&obj,0,sizeof(obj));
    obj = tfs[i];
    if(strcomp(obj.name,filename)==0 && obj.filetype == REGULAR){
      strcopy(entry->name,filename);
      entry->pindex = i;
      entry->start = obj.start;
      return entry;
    }
    i++;
  }
  kprintf("File doesn't exist\n");
  return NULL;
}

uint64_t tarfs_file_read(tarfs_obj *file, int size, uint64_t buf){
  if(file->start !=0){
    struct posix_header_ustar *tarfs = (struct posix_header_ustar *)file->start;
    uint64_t fsize = oct_dec(stroi(tarfs->size));
    if(fsize < size){
      size = fsize;
    }
    int bytes = size;
    char *tmp = (char*)buf;
    char *file_start = (char*)(file->start + 512);
    int i=0;
    while(size-- > 0){
      tmp[i++] = *file_start++;
    }
    tmp[i] = '\0';
    kprintf("%s\n",tmp);
    return bytes;
  }else{
    return 0;
  }
}

//int close_file(tarfs_file* addr){


//}


int get_pindex(char *root, uint8_t f_type){
  char fname[100];
  int nlen = strlength(root);
  strcopy(&fname[0],root);
  if(f_type == REGULAR) nlen = nlen-1;
  if(f_type == DIRECTORY) nlen = nlen -2;
  
  while(fname[nlen] !='/'){
    nlen --;
    if(nlen ==0) return -1; //some value which can never be index of file
  }
  nlen++;
  fname[nlen] = '\0';
  int i=0;
  while(strcomp(&fname[0], &(tfs[i].name[0]))!=0){
    i++;
  }
  return i;
}

void tarfs_init(){
  int count=0,i,j;
  uint64_t offset=0,fsize;
  tarfs_obj obj;
  struct posix_header_ustar *tarfs = (struct posix_header_ustar *)&_binary_tarfs_start+offset;
  while(strlength(tarfs->name) != 0){
    memset(&obj,0,sizeof(obj));
    j=0;
    fsize=0;
    for(i=10;i>=0;--i)
      fsize = fsize + (tarfs->size[i]-48) * pow(8,j++); //file size in bytes
    if(fsize%512!=0){
      fsize=(fsize/512)*512;
      fsize+=512;   
      }
    obj.size = fsize;
    strcopy(&obj.name[0],tarfs->name);
    obj.filetype = stroi(tarfs->typeflag);
    obj.pindex = get_pindex(&(obj.name[0]),obj.filetype);
    offset = (uint64_t)tarfs - (uint64_t)&_binary_tarfs_start;
    obj.start = (uint64_t)&_binary_tarfs_start + offset;
    tfs[count++] = obj;
    tarfs = (struct posix_header_ustar *)((uint64_t)tarfs + fsize + sizeof(struct posix_header_ustar));
    }
  //kprintf("File count in tarfs %d\n",count); 
}

//returns the address of opened directory
uint64_t tarfs_open_dir(char *dir){
  tarfs_obj entry;
  int i =0;
  
  if(strcomp(dir,"/") == 0){ //root directory --list tfs entries having pindex = -1
    return tfs[0].start;
  }  
  
  while(strlength(tfs[i].name) != 0){
    entry = tfs[i];
	  if(entry.name !=0){
		  if(entry.filetype == DIRECTORY){
			  if(strcomp(entry.name,dir)==0){
				  return entry.start;
			  }
		  }
	  }
  i++;
  }
  kprintf("Directory doesn't exist!!\n");
  return 0;
 }

uint64_t tarfs_close_dir(uint64_t dir_off){
  return 0;
}

/*
void list_tarfs_entries(){


}*/

//takes in header offset as argument to start reading directory
uint64_t tarfs_read_dir(uint64_t dir_off){
	tarfs_obj entry;
	char *dir_name;
 int i=0,p_index= -999,len =0;
  while(strlength(tfs[i].name)!=0){
    memset(&entry,0,sizeof(entry));
    entry = tfs[i];
    if(entry.start == dir_off){ //found the directory ..get the name of dir
      dir_name = entry.name;
      p_index = i;
      i++;
      len = strlength(dir_name);
      continue;
    }
  if(entry.pindex == p_index && (strncomp(&(entry.name[0]),dir_name,strlength(dir_name)))==0){
      kprintf("%s    ",entry.name+len);
  }
  i++;
  }
  kprintf("\n");
  return 0;
}

/*
uint64_t wait_pid(pid_t pid, int *status, int options){
  struct task_struct *parent = curr_task;
  struct task_struct *child;
  uint64_t pid = parent->pid;
  
  if(pid > 0){ //have pid wait for termination of given pid waala process
    
  
  
  }
   
  if(pid == 0){
  
  
  }
  
  if()
  
  
  
}*/
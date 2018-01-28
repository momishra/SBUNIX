#include <sys/defs.h>

typedef uint64_t (*read_type_t)(struct fs_node*, uint64_t,uint64_t,uint16_t*);
typedef uint64_t (*write_type_t)(struct fs_node*, uint64_t,uint64_t,uint16_t*);
typedef void (*open_type_t)(struct fs_node*);
typedef void (*close_type_t)(struct fs_node*);
typedef struct dirent * (*readdir_type_t)(struct fs_node*, uint64_t);
typedef struct fs_node * (*finddir_type_t)(struct fs_node*, char *name);

struct dirent{
	char name[128];
	uint64_t ino;
};

typedef struct fs_node{
	char name[128]; //filename
	uint64_t mask; //permission mask
	uint64_t uid; //owning user
	uint64_t inode; //files
	uint64_t length; //size of file
	uint64_t impl; //implementation-defined number
	uint64_t flags;
	read_type_t read;
	write_type_t write;
	open_type_t open;
	close_type_t close;
	readdir_type_t readdir;
	finddir_type_t finddir;
	struct fs_node *ptr; 
}fs_node_t;

#include <fs.h>
#include <mmu.h>
#include "lib.h"

// IDE disk number to look on for our file system
#define DISKNO 1
#define BY2SECT 512					// Bytes per disk sector
#define SECT2BLK (BY2BLK / BY2SECT) // sectors to a block

// Disk block n, when in memory, is mapped into the file system
// server's address space at DISKMAP+(n*BY2BLK).
#define DISKMAP 0x10000000

// Maximum disk size we can handle (1GB)
#define DISKMAX 0x40000000

// ide.c
void ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs);
void ide_write(u_int diskno, u_int secno, void *src, u_int nsecs);

// fs.c
void fs_init(void);
void fs_sync(void);
int map_block(u_int);
int alloc_block(void);
int file_remove(char *path);
void file_flush(struct File *);
void file_close(struct File *f);
int file_dirty(struct File *f, u_int offset);
int file_open(char *path, struct File **pfile);
int file_set_size(struct File *f, u_int newsize);
int file_get_block(struct File *f, u_int blockno, void **pblk);
extern u_int *bitmap;

// test.c
void fs_test(void);

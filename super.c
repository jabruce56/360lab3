/********* super.c code ***************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>

typedef unsigned int   u32;

//define shorter TYPES, save typing efforts
typedef struct ext2_group_desc  GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;    // need this for new version of e2fs

GD    *gp;
SUPER *sp;
INODE *ip;
DIR   *dp;

/******************* in <ext2fs/ext2_fs.h>*******************************
struct ext2_super_block {
  u32  s_inodes_count;       // total number of inodes
  u32  s_blocks_count;       // total number of blocks
  u32  s_r_blocks_count;
  u32  s_free_blocks_count;  // current number of free blocks
  u32  s_free_inodes_count;  // current number of free inodes
  u32  s_first_data_block;   // first data block in this group
  u32  s_log_block_size;     // 0 for 1KB block size
  u32  s_log_frag_size;
  u32  s_blocks_per_group;   // 8192 blocks per group
  u32  s_frags_per_group;
  u32  s_inodes_per_group;
  u32  s_mtime;
  u32  s_wtime;
  u16  s_mnt_count;          // number of times mounted
  u16  s_max_mnt_count;      // mount limit
  u16  s_magic;              // 0xEF53
  // MANY MORE non-essential fields
};
**********************************************************************/

#define BLKSIZE 1024
char buf[BLKSIZE];
int fd;

// int get_block(int fd, int blk, char buf[ ])
// {
//   lseek(fd, (long)blk*BLKSIZE, 0);
//   read(fd, buf, BLKSIZE);
// }

super()
{
  // read SUPER block
  get_block(fd, 1, buf);
  sp = (SUPER *)buf;

  // check for EXT2 magic number:

  //printf("s_magic = %x\n", sp->s_magic);
  if (sp->s_magic != 0xEF53){
    printf("NOT an EXT2 FS\n");
    exit(1);
  }

  //printf("EXT2 FS OK\n");
  printf("\n      -super block-\n");
  printf("inodes count      = %6d\n", sp->s_inodes_count);
  printf("blocks count      = %6d\n", sp->s_blocks_count);
  printf("free inodes count = %6d\n", sp->s_free_inodes_count);
  printf("free blocks count = %6d\n", sp->s_free_blocks_count);

  printf("first data block  = %6d\n", sp->s_first_data_block);

  printf("log block size    = %6d\n", sp->s_log_block_size);

  printf("blocks per group  = %6d\n", sp->s_blocks_per_group);
  printf("inodes per group  = %6d\n", sp->s_inodes_per_group);


  printf("mnt count         = %6d\n", sp->s_mnt_count);
  printf("max mnt count     = %6d\n", sp->s_max_mnt_count);

  printf("magic             = %6x\n", sp->s_magic);

  //printf("s_mtime = %s", ctime(&sp->s_mtime));
  //printf("s_wtime = %s", ctime(&sp->s_wtime));
}

//char *disk = "mydisk";

// main(int argc, char *argv[ ]){
//   if (argc > 1)
//     disk = argv[1];
//   fd = open(disk, O_RDONLY);
//   if (fd < 0){
//     printf("open failed\n");
//     exit(1);
//   }
//   super();
// }
/*
***** SAMPLE OUTPUTs of super.c ****************
s_inodes_count                 =      184
s_blocks_count                 =     1440
s_free_inodes_count            =      174
s_free_blocks_count            =     1411
s_log_block_size               =        0
s_blocks_per_group             =     8192
s_inodes_per_group             =      184
s_mnt_count                    =        1
s_max_mnt_count                =       34
s_magic                        =     ef53
s_mtime = Mon Feb  9 07:08:22 2004
************************************************/

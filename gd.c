#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
typedef unsigned int   u32;
//typedef unsigned short u16;

// define shorter TYPES, save typing efforts
typedef struct ext2_group_desc  GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;
GD    *gp;
SUPER *sp;
INODE *ip;
DIR   *dp;
/*struct ext2_group_desc
{
  u32 bg_block_bitmap; //Bmap block number
  u32 bg_inode_bitmap; //Imap block number
  u32 bg_inode_table;  //Inodes begin block number
  u16 bg_free_blocks_count;
  u16 bg_free_inodes_count;
  u16 bg_used_dirs_count;

  //ignore
  u16 bg_pad;
  u32 bg_reserved[3];
}*/
#define BLKSIZE 1024
char buf[BLKSIZE];
int fd;

// int get_block(int fd, int blk, char buf[ ])
// {
//   lseek(fd, (long)blk*BLKSIZE, 0);
//   read(fd, buf, BLKSIZE);
// }

gd()
{
  get_block(fd, 2, buf);//go to block 2, group descriptor block
  gp=(GD *)buf;
  printf("\n   -group descriptor-\n");
  printf("block bitmap      = %6d\n", gp->bg_block_bitmap);
  printf("inode bitmap      = %6d\n", gp->bg_inode_bitmap);
  printf("inode table       = %6d\n", gp->bg_inode_table);
  printf("free blocks count = %6d\n", gp->bg_free_blocks_count);
  printf("free inodes count = %6d\n", gp->bg_free_inodes_count);
  printf("used dirs count   = %6d\n", gp->bg_used_dirs_count);
}
// char *disk = "mydisk";
//
// main(int argc, char *argv[ ])
//
// {
//   if (argc > 1)
//     disk = argv[1];
//   fd = open(disk, O_RDONLY);
//   if (fd < 0){
//     printf("open failed\n");
//     exit(1);
//   }
//
//   gd();
// }

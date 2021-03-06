/********* inode.c: print information in / INODE (INODE #2) *********/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>

#define BLKSIZE 1024

// define shorter TYPES, save typing efforts
typedef struct ext2_group_desc  GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;    // need this for new version of e2fs

GD    *gp;
SUPER *sp;
INODE *ip;
DIR   *dp;

int fd;
int iblock;

// int get_block(int fd, int blk, char buf[ ])
// {
//    lseek(fd,(long)blk*BLKSIZE, 0);
//    read(fd, buf, BLKSIZE);
// }

inode()
{
  char buf[BLKSIZE];

  // read GD
  get_block(fd, 2, buf);
  gp = (GD *)buf;
  /************ print these to see what they are  ****
  printf("%8d %8d %8d %8d %8d %8d\n",
	 gp->bg_block_bitmap,
	 gp->bg_inode_bitmap,
	 gp->bg_inode_table,
	 gp->bg_free_blocks_count,
	 gp->bg_free_inodes_count,
	 gp->bg_used_dirs_count);
  /***************************************************/
  iblock = gp->bg_inode_table;   // get inode start block#
  printf("\n      -root inode-\n");
  printf("inode block = %6d", iblock);

  // get inode start block
  get_block(fd, iblock, buf);

  ip = (INODE *)buf + 1;         // ip points at 2nd INODE


  printf("  uid  = %4d\n", ip->i_uid);
  printf("mode        = %6x ", ip->i_mode);
  printf(" gid  = %4d\n", ip->i_gid);
  printf("size        = %6d\n", ip->i_size);
  printf("created     = %6s",   ctime(&ip->i_ctime));
  printf("link count  = %6d\n", ip->i_links_count);
  printf("block count = %6d\n", ip->i_blocks);
  printf("i_block[0]  = %6d\n", ip->i_block[0]);

 /*****************************
  u16  i_mode;                      // fiel type|permission bits
  u16  i_uid;                       // ownerID
  u32  i_size;                      // file size in bytes
  u32  i_atime;                     // time fields
  u32  i_ctime;
  u32  i_mtime;
  u32  i_dtime;
  u16  i_gid;                       // groupID
  u16  i_links_count;               // link count
  u32  i_blocks;                    // IGNORE
  u32  i_flags;                     // IGNORE
  u32  i_reserved1;                 // IGNORE
  u32  i_block[15];                 // IMPORTANT, but later
 ***************************/
}

// char *disk = "mydisk";
// main(int argc, char *argv[])
// {
//   if (argc > 1)
//     disk = argv[1];
//
//   fd = open(disk, O_RDONLY);
//   if (fd < 0){
//     printf("open %s failed\n", disk);
//     exit(1);
//   }
//
//   inode();
// }
/******************************
inode_block = 10
mode=    41ed
uid=0
gid=0
size=1024
time=Mon Feb  9 07:32:16 2014
link=3
i_block[0]=33
******************************/

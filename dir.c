#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>

typedef unsigned int   u32;

// define shorter TYPES, save typing efforts
typedef struct ext2_group_desc  GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;    // need this for new version of e2fs

GD    *gp;
SUPER *sp;
INODE *ip;
DIR   *dp;
#define BLKSIZE 1024
char buf[BLKSIZE];
int fd;

int get_block(int fd, int blk, char buf[ ])
{
  lseek(fd, (long)blk*BLKSIZE, 0);
  read(fd, buf, BLKSIZE);
}
int tst_bit(char *buf, int bit)
{
  int i, j;
  i = bit / 8;  j = bit % 8;
  return (buf[i] & (1 << j));
}
/****
struct ext2_dir_entry_2 {
u32 inode; // inode number; count from 1, NOT 0
u16 rec_len; // this entry's length in bytes
u8 name_len; // name length in bytes
u8 file_type; // not used
char name[EXT2_NAME_LEN]; // name: 1 -255 chars, no NULL byte
};****/
u32 search(INODE *inodePtr, char *name)
{

}
printdir(DIR * dp, char *cp)
{
  int i =0;
  printf("%d ", dp->inode);
  for (i=0; i < dp->name_len;i++){
    printf("%c", dp->name[0]);
  }
  putchar('\n');

}
dir(){
  char *cp;
  int ninodes = 0, i = 0, j = 0;
  get_block(fd, 1, buf);
  sp = (SUPER *)buf;
  ninodes = sp->s_inodes_count;
  get_block(fd, 2, buf);
  gp = (GD *)buf;

  printf("#inodes %d\n", ninodes);
  get_block(fd, gp->bg_inode_table, buf);

  ip = (INODE *)buf+1;//inode 2

  get_block(fd, ip->i_block[i], buf);
  dp = (DIR *)buf;
  cp = buf;
  while(dp->inode)
  {
    printdir(dp, cp);
    cp += dp->rec_len;
    dp = (DIR *)cp;
  }
}
char *disk = "mydisk";
main(int argc, char *argv[ ]){
  if (argc > 1)
    disk = argv[1];
  fd = open(disk, O_RDONLY);
  if (fd < 0){
    printf("open failed\n");
    exit(1);
  }
  //super();
  //gd();
  //bmap();
  dir();
}

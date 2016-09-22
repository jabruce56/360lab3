#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <ext2fs/ext2_fs.h>

#include "super.c"
#include "gd.c"
#include "inode.c"
#include "dir.c"

typedef unsigned int   u32;
typedef struct ext2_group_desc  GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;

GD    *gp;
SUPER *sp;
INODE *ip;
DIR   *dp;

#define BLKSIZE 1024
char buf[BLKSIZE];
int fd;

int get_block(int fd, int blk, char buf[ ]){
  lseek(fd, (long)blk*BLKSIZE, 0);
  read(fd, buf, BLKSIZE);
}

u32 search(INODE *inodePtr, char *name){
  int n = 0, i = 0, j = 0;
  char *str, *cp;
  get_block(fd, inodePtr->i_block[0], buf);
  dp = (DIR *)buf;
  cp = buf;
  printf("\nsearching for %s...\n", name);
  for(i=0;i<6;i++){
      printf("%4d %4s\n", dp->inode, dp->name);
      for(j=0;j<strlen(name);j++)
        if(dp->name[j]==name[j]){
          if(j==strlen(name)-1){
            printf("found %s at inumber %u\n", name, (u32)dp->inode);
            return dp->inode;
          }
        }
        else
            break;
    cp += dp->rec_len;
    dp = (DIR *)cp;
  }
  printf("search found nothing with the name %s\n", name);
  return 0;
}

print_diskb()
{
  int i = 0;
  printf("\n -disk blocks-\n");
  for(i=0;(i<=15)&&(ip->i_block[i]);i++)
    printf("block[%2d] = %u\n", i, ip->i_block[i]);
}

print_db()
{
  int i = 0;
  printf("\n            -direct blocks-\n");
  for(i=0;(i<12)&&(ip->i_block[i]);i++){
    printf("%3u ", ip->i_block[i]);
    if(((i+1)%10==0)){
      putchar('\n');
    }
  }
  putchar('\n');
}

print_idb(){
  u32 ubuf[256];
  int i = 0;
  printf("\n           -indirect blocks-\n");
  get_block(fd, ip->i_block[12], ubuf);
  for(i=0;i<256&&(ubuf[i]);i++){
    printf("%3u ", ubuf[i]);
    if(((i+1)%10==0)){
      putchar('\n');
    }
  }
  putchar('\n');
}

print_didb(){
  u32 dbuf[256], ubuf[256];
  int i = 0, j = 0;
  printf("\n      -double indirect pointers-\n");
  get_block(fd, ip->i_block[13], dbuf);
  for(i=0;i<256&&(dbuf[i]);i++){
    printf("%3u ", dbuf[i]);
    if(((i+1)%10==0)){
      putchar('\n');
    }
  }
  putchar('\n');
  printf("\n       -double indirect blocks-\n");
  for(i=0;i<256&&(dbuf[i]);i++){
    get_block(fd, dbuf[i], ubuf);
    for(j=0;j<256&&(ubuf[j]);j++){
      printf("%3u ", ubuf[j]);
      if(((j+1)%10==0)){
        putchar('\n');
      }
    }
  }
}

char *disk;

main(int argc, char *argv[]){
  u32 ino, blk, offset, inostrt;
  int n = 0, i = 0;
  char *str, *token, *cp, ubuf[256];
  str = argv[2];
  for(i=0;i<strlen(str);i++)
    if(str[i]=='/')
      n++;
  char *name[n];
  if (argc > 1)
    disk = argv[1];
  fd = open(disk, O_RDONLY);
  if (fd < 0){
    printf("open failed\n");
    exit(1);
  }

  super();
  gd();
  inode();
  printf("\n============================================\n");
  dir();

  get_block(fd, 2, buf);
  gp = (GD *)buf;
  inostrt = gp->bg_inode_table;
  get_block(fd, gp->bg_inode_table, buf);
  ip = (INODE *)buf+1;

  token = strtok(str, "/");
  i=0;
  while (token != NULL){
    name[i]=token;
    i++;
    token = strtok(NULL, str);
  }

  printf("\n  -begin search-\n");
  printf("n    = %d\nname = ", n);
  for(i=0;i<n;i++){printf("/%s", name[i]);}
  putchar('\n');
  for(i=0;i<n;i++){
    ino=search(ip, name[i]);
    if(!ino)
      break;
    blk = (ino - 1) / 8 + inostrt;
    offset = (ino - 1) % 8;
    get_block(fd, blk, buf);
    ip=(INODE *)buf+offset;
  }
  printf("\n============================================\n");

  if(ino){
    print_diskb();
    print_db();
    if(ip->i_block[12])
      print_idb();
    if(ip->i_block[13])
      print_didb();
  }
  else
    printf("no blocks to display\n");
  printf("\n");
}

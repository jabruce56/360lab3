/***********************************
Jesse Bruce
cpts 360
lab3 showblock

showblock DISK PATHNAME

prints superblock group descriptor inode and dir of given disk and will search for a given file to print block information
***********************************/
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

u32 search(INODE *inodePtr, char *name){//searches for name in each dir entry
  int n = 0, i = 0, j = 0;
  char *str, *cp;
  get_block(fd, inodePtr->i_block[0], buf);
  dp = (DIR *)buf;
  cp = buf;
  printf("\nsearching for %s...\n", name);
  for(i=0;i<12;i++){
      printf("%4d %4s\n", dp->inode, dp->name);
      for(j=0;j<strlen(name);j++)//over length of name
        if(dp->name[j]==name[j]){//compare each char
          if(j==strlen(name)-1){//if at end of name
            printf("found %s at inumber %u\n", name, (u32)dp->inode);
            return dp->inode;//return the inode number
          }
        }
        else//if any char does not match, break
            break;
    cp += dp->rec_len;//advance
    dp = (DIR *)cp;
  }
  printf("search found nothing with the name %s\n", name);
  return 0;
}

print_diskb(){//prints i blocks 0-15 if available
  int i = 0;
  printf("\n -disk blocks-\n");
  for(i=0;(i<=15)&&(ip->i_block[i]);i++)
    printf("block[%2d] = %u\n", i, ip->i_block[i]);
}

print_db(){//prints all direct blocks
  int i = 0;
  printf("\n            -direct blocks-\n");
  for(i=0;(i<12)&&(ip->i_block[i]);i++){//for blocks 0-11(direct blocks)
    printf("%3u ", ip->i_block[i]);
    if(((i+1)%10==0)){//every tenth put newline
      putchar('\n');
    }
  }
  putchar('\n');
}

print_idb(){//prints al 256 indirect blocks
  u32 ubuf[256];
  int i = 0;
  printf("\n           -indirect blocks-\n");
  get_block(fd, ip->i_block[12], ubuf);//seek to indirect block containing 256 block numbers that each point to a block
  for(i=0;i<256&&(ubuf[i]);i++){//go through block numbers
    printf("%3u ", ubuf[i]);
    if(((i+1)%10==0)){//newline every 10
      putchar('\n');
    }
  }
  putchar('\n');
}

print_didb(){//prints double indirect blocks
  u32 dbuf[256], ubuf[256];
  int i = 0, j = 0;
  printf("\n      -double indirect pointers-\n");
  get_block(fd, ip->i_block[13], dbuf);//seek to double indirect block containing 256 pointers that each point to 256 block numbers that each point to a block
  for(i=0;i<256&&(dbuf[i]);i++){
    printf("%3u ", dbuf[i]);
    if(((i+1)%10==0)){
      putchar('\n');
    }
  }
  putchar('\n');
  printf("\n       -double indirect blocks-\n");
  for(i=0;i<256&&(dbuf[i]);i++){//go through the double indirect block of 256 pointers to 256 pointers
    get_block(fd, dbuf[i], ubuf);//move to each block of indirect pointers
    for(j=0;j<256&&(ubuf[j]);j++){//go through each of the 256 block numbers and print blocks
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
  for(i=0;i<strlen(str);i++)//find n
    if(str[i]=='/')
      n++;
  char *name[n];

  //check disk input
  if (argc > 1)
    disk = argv[1];
  fd = open(disk, O_RDONLY);
  if (fd < 0){
    printf("open failed\n");
    exit(1);
  }

  //printf information
  super();
  gd();
  inode();
  printf("    -ready to search-\npress any key to continue...");
  getchar();
  printf("\n============================================\n");
  dir();

  get_block(fd, 2, buf);
  gp = (GD *)buf;
  inostrt = gp->bg_inode_table;
  get_block(fd, gp->bg_inode_table, buf);
  ip = (INODE *)buf+1;

  //parse str using strtok and place into name[n]
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
  for(i=0;i<n;i++){//search for input pathname
    ino=search(ip, name[i]);
    if(!ino)
      break;

    //mailmans algo to convert inode number to disk position
    blk = (ino - 1) / 8 + inostrt;
    offset = (ino - 1) % 8;
    get_block(fd, blk, buf);//seek
    ip=(INODE *)buf+offset;//add offset
  }
  printf("  -ready to print blocks-\npress any key to continue...");
  getchar();
  printf("\n============================================\n");

  if(ino){//print blocks
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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <ext2fs/ext2_fs.h>
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
printdir(DIR * dp, char *cp){
  int i =0;
  printf("%d ", dp->inode);
  for (i=0; i < dp->name_len;i++){
    printf("%c", dp->name[0]);
  }
  putchar('\n');
}
u32 search(INODE *inodePtr, char *name){
  int n = 0, i = 0, j = 0;
  char *str, *cp;
  get_block(fd, inodePtr->i_block[0], buf);
  dp = (DIR *)buf;
  cp = buf;
  printf("searching for %s\n", name);
  for(i=0;i<6;i++){

      printf("%4d %4s\n", dp->inode, dp->name);
      for(j=0;j<strlen(name);j++)
        if(dp->name[j]==name[j]){
          if(j==strlen(name)-1){
            printf("found %s\n", name);
            return dp->inode;
          }
        }
        else
            break;
      //name[i]=dp->name;
    cp += dp->rec_len;
    dp = (DIR *)cp;
  }
  return 0;
}
print_db(INODE *inodePtr)
{
  DIR *d;
  int i = 0;
  for(i=0;i<12;i++)
  {
    get_block(fd, inodePtr->i_block[i], buf);
    d = (DIR *)buf;
    printf("diskblock %d:\n%s\n", i, d->name);
  }
}
char *disk = "mydisk";


// Linear_address LA = N*block + house;
// Block_address BA = (LA / N, LA % N);
// blk = (ino - 1) / INODES_PER_BLOCK + InodesBeginBlock;
// offset = (ino - 1) % INODES_PER_BLOCK

main(int argc, char *argv[]){
  u32 ino, blk, offset, inostrt;
  char ibuf[BLKSIZE];
  u32 *up;
  int n = 0, i = 0;
  char *str, *token, *cp, gbuf[BLKSIZE];
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
  get_block(fd, 2, gbuf);
  gp = (GD *)gbuf;
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
  for(i=0;i<n;i++){
    //if(name[i]==)
    ino=search(ip, name[i]);
    //printf("%s\n%d\n", name[i], ino);
    if(!ino)
      break;
    printf("%d\n", ino);
    blk = (ino - 1) / 8 + inostrt;
    offset = (ino - 1) % 8;
    get_block(fd, blk, buf);
    ip=(INODE *)buf+offset;
  }
  if(ino)
    print_db(ip);

  if (ip->i_block[12]){  // has indirect blocks
       get_block(fd, ip->i_block[12], ibuf);
       up = (u32 *)ibuf;
       while(*up){
          printf("%d \n", *up);
          up++;
       }
    }
    u32 ubuf[256];
    get_block(fd, ip->i_block[12], (char *)ubuf);
    print_db(ubuf);
    while(ubuf[i]){

      printf("%d \n", ubuf[i++]);
    }
  /*
  Assume INODE *ip -> INODE of file

     char ibuf[BLKSIZE];
     u32 *up;


     if (ip->i_block[12]){  // has indirect blocks
        get_block(fd, ip->i_block[12], ibuf);
        up = (shut_up)ibuf;
        while(*up){
           printf("%d ", *up);
           up++;
        }
     }

==================================================
     u32 ubuf[256];
     get_block(fd, ip->i_block[12], ubuf);
     int i = 0;
     while(ubuf[i]){
        printf("%d ", ubuf[i++]);
     }
================================================

     YOU DO THE double indirect blocks
*/
}

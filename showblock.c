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
char *name[50];
int fd;
int get_block(int fd, int blk, char buf[ ])
{
  lseek(fd, (long)blk*BLKSIZE, 0);
  read(fd, buf, BLKSIZE);
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
  int used_dirs = 0, i = 0, j = 0;
  get_block(fd, 2, buf);
  gp = (GD *)buf;
  used_dirs=gp->bg_used_dirs_count;
  char *dirs[used_dirs];
  get_block(fd, gp->bg_inode_table, buf);
  ip = (INODE *)buf+1;//inode 2
  get_block(fd, ip->i_block[i], buf);
  dp = (DIR *)buf;
  cp = buf;
  for(i=0;i<=used_dirs;i++)
  {
    if (i!=2){

      //printdir(dp, cp);
      dirs[i]=dp->name;
    }
    cp += dp->rec_len;
    dp = (DIR *)cp;
  }
  return 0;
}

u32 search(INODE *inodePtr, char *name)
{
  int n = 0, i = 0, t = 0, used_dirs = 0, j = 0;
  char *str, *token, *cp;
  get_block(fd, inodePtr->i_block[i], buf);
  dp = (DIR *)buf;
  cp = buf;

  for(i=0;i<=used_dirs;i++)
  {
    if (i!=2){
      if(&dp->name[0]==name[i])
          return 1;
      // printdir(dp, cp);
      //name[i]=dp->name;
    }
    cp += dp->rec_len;
    dp = (DIR *)cp;
  }
  return 0;
}
char *disk = "mydisk";

main(int argc, char *argv[]){
  int n = 0, i = 0, t = 0, used_dirs = 0, j = 0;
  char *str, *token, *cp;
  char **dirname;
  if (argc > 1)
    disk = argv[1];
  fd = open(disk, O_RDONLY);
  if (fd < 0){
    printf("open failed\n");
    exit(1);
  }
  get_block(fd, 2, buf);
  gp = (GD *)buf;
  used_dirs=gp->bg_used_dirs_count;
  get_block(fd, gp->bg_inode_table, buf);
  ip = (INODE *)buf+1;
  str = argv[2];
  token = strtok(str, "/");
  while (token != NULL){
    name[n]=token;
    n++;
    token = strtok(NULL, str);
  }
  for(i=0;i<n;i++)
    printf("%s\n%d\n", name[i], n);
  t = dir(name, n);
  //printf("%s\n", dirname[4]);
}

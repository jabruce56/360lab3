#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>

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

bmap()
{
  int i = 0, fr = 0, inuse = 0, nblk = 0;
  get_block(fd, 1, buf);
  sp = (SUPER *)buf;
  nblk =sp->s_blocks_count;
  get_block(fd, sp->s_first_data_block, buf);
  gp = (GD *)buf;

  for(i = 1; i < nblk; i++)
  {
    if(tst_bit(buf, gp->bg_block_bitmap+i))
      inuse++;
    else
      fr++;
  }
  printf("bmap bitmap\n");
  printf("%d blocks in use\n", inuse);
  printf("%d blocks free\n", fr);
}

char *disk = "mydisk";

main(int argc, char *argv[ ])
{
  if (argc > 1)
    disk = argv[1];

  fd = open(disk, O_RDONLY);
  if (fd < 0){
    printf("open %s failed\n", disk);
    exit(1);
  }

  bmap();
}

/*Jesse Bruce

360 LAB #3 Pre-Work

                   CS 360 LAB #3 Pre-work

                    FS Examples Programs


DUE: Wed 9-14-2016 in class


TURN IN a hard copy of your gd.c, bmap.c, dir.c code as specified below

    NOTE: You may integerate YOUR gd.c, bmap.c and dir.c ALL in one program

===============================================================================
ASSUME: An EXT2 virtual disk, e.g. ~samples/LAB3/mydisk.
Write C programs to display the specified information on the virtual disk:
------------------------------------------------------------------------
1. super.c  : print superblock information,
2. WRITE YOUR gd.c to print group descriptor information.

3. imap.c   : print Imap as a sequence of 0/1 chars (80 chars per row)
4. WRITE YOUR bmap.c to display Bmap; the blocks bitmap.

5. inode.c  : root inode information
6. WRITE YOUR dir.c to print ALL names under / directory
-------------------------------------------------------------------------*/
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
gd()
{
  get_block(fd, 2, buf);//go to block 2, group descriptor block
  gp=(GD *)buf;

  printf("bg_block_bitmap = %d\n", gp->bg_block_bitmap);
  printf("bg_inode_bitmap = %d\n", gp->bg_inode_bitmap);
  printf("bg_inode_table = %d\n", gp->bg_inode_table);
  printf("bg_free_blocks_count = %d\n", gp->bg_free_blocks_count);
  printf("bg_free_inodes_count = %d\n", gp->bg_free_inodes_count);
  printf("bg_used_dirs_count = %d\n", gp->bg_used_dirs_count);
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
  printf("bmap\n");
  printf("%d blocks in use\n", inuse);
  printf("%d blocks free\n", fr);
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
  int i = 0, n = 0;
  get_block(fd, 1, buf);
  sp = (SUPER *)buf;
  get_block(fd, 2, buf);
  gp = (GD *)buf;
  n=gp->bg_used_dirs_count;
  get_block(fd, gp->bg_inode_table, buf);

  ip = (INODE *)buf+1;//inode 2
  printf("%d\n", n);
  get_block(fd, ip->i_block[i], buf);
  dp = (DIR *)buf;
  cp = buf;
  for(i=0;i<=n;i++){
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
  gd();
  bmap();
  dir();
}

/*6. WRITE YOUR dir.c to print ALL files under the / directory:

              HOW TO STEP THROUGH dir_entries:

Assume the root directory / contains entries
           this  is  aVeryLongName short
Then the 0th data block (i_block[0] in its inode) of this DIR contains:

|2 12 1.|2 12 2..|11 12 4this|12 12 2is|13 24 13aVeryLongName|14 952 5short   |

Each record has a rec_len and a name_len field.

First, read the block into a char buf[1024].
Let DIR *dp and char *cp BOTH point at buf;
Use dp-> to access the fields of the record, e.g. print its name

                    TO MOVE TO THE NEXT entry:
        cp += dp->rec_len;       // advance cp by rec_len BYTEs
        dp = (shut-up)cp;        // pull dp along to the next record

This way, you can step through ALL the record entries of a DIR file.
*/



/*360 LAB #3 Pre-Work

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
-------------------------------------------------------------------------

2. WRITE YOUR OWN gd.c to print information of Group Descriptor 0


3.

4. WRITE YOUR OWN bmap.c to print the blocks bitmap


5.

6. WRITE YOUR dir.c to print ALL files under the / directory:

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

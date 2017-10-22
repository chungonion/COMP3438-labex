#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include "../superblock.h"
#include "../inode.h"

const char *HD = "../HD";

int main() {
	//Please input your code
	//Flush the superblock region and initialize the root directory in the file.
	int fd = open(HD, O_RDWR, 0660);
	if (!fd) {
		printf("Error in open()\n");
		return -1;
	}
	// else{
	// printf("This is ok!\n");
	// }

	struct superblock *superblockdata = (struct superblock *)malloc(sizeof(struct superblock));

	superblockdata->inode_offset = INODE_OFFSET;
	superblockdata->data_offset = DATA_OFFSET;
	superblockdata->max_inode = MAX_INODE;
	superblockdata->max_data_blk = MAX_DATA_BLK;
	superblockdata->next_available_inode = 1;
	superblockdata->next_available_blk = 1;
	superblockdata->blk_size = BLOCK_SIZE;

	if (lseek(fd, SB_OFFSET, SEEK_SET)<0) {
		printf("Error in lseek()!\n");
	}
	// else{
	//  printf("lseek ok!\n");
	// }
	// printf("SB Offset: %d\n",SB_OFFSET);

	if (write(fd, (void *)superblockdata, sizeof(struct superblock))<0) {
		printf("Error in write()!\n");
	}

	if (lseek(fd, SB_OFFSET, SEEK_SET)<0) {
		printf("Error in lseek()!\n");
	}


	struct superblock *test = (struct superblock *)malloc(sizeof(struct superblock));


	if (read(fd, (struct superblock*)test, sizeof(struct superblock))<0) {
		printf("Error in read()!\n");
	}

	
	//Test whether read is success, printf something to test.
	printf("inode offset: %d\n", test->inode_offset);
	printf("data offset: %d\n", test->data_offset);
	printf("max. inode: %d\n", test->max_inode);
	printf("max data block: %d\n", test->max_data_blk);
	printf("next available inode: %d\n", test->next_available_inode);
	printf("next available block: %d\n", test->next_available_blk);
	printf("block size: %d\n", test->blk_size);
	//Above for testing purpose only


	struct inode *root_inode = (struct inode *)malloc(sizeof(struct inode));
	root_inode->i_type = 0;
	root_inode->i_number = 0;
	root_inode->file_num = 2;
	root_inode->i_size = 0;
	root_inode->direct_blk[0] = 0;
	time(&root_inode->i_mtime);

	if (lseek(fd, superblockdata->inode_offset, SEEK_SET)<0) {
		printf("Error in lseek()!\n");
	}

	if (write(fd, (struct inode *)root_inode, sizeof(struct inode))<0) {
		printf("Error in write()!\n");
	}

	DIR_NODE *current_dir_node = (DIR_NODE *)malloc(sizeof(DIR_NODE));
	DIR_NODE *parent_dir_node = (DIR_NODE *)malloc(sizeof(DIR_NODE));
	strcpy(current_dir_node->dir, ".");
	current_dir_node->inode_number = 0;
	strcpy(parent_dir_node->dir, "..");
	parent_dir_node->inode_number = 0;



	if (lseek(fd, superblockdata->data_offset + root_inode->direct_blk[0] * superblockdata->blk_size + root_inode->i_size, SEEK_SET)<0) {
		printf("Error in lseek()!\n");
	}

	if (write(fd, (DIR_NODE *)current_dir_node, sizeof(DIR_NODE))<0) {
		printf("Error in write()!\n");
	} else {
		root_inode->i_size += sizeof(DIR_NODE);
		if (root_inode->i_size / superblockdata->blk_size + 1 >root_inode->i_blocks) {
			root_inode->i_blocks += root_inode->i_size / superblockdata->blk_size + 1 - root_inode->i_blocks;
		}
	}

	if (lseek(fd, superblockdata->data_offset + root_inode->direct_blk[0] * superblockdata->blk_size + root_inode->i_size, SEEK_SET)<0) {
		printf("Error in lseek()!\n");
	}

	if (write(fd, (DIR_NODE *)parent_dir_node, sizeof(DIR_NODE))<0) {
		printf("Error in write()!\n");
	} else {
		root_inode->i_size += sizeof(DIR_NODE);
		if (root_inode->i_size / superblockdata->blk_size + 1>root_inode->i_blocks) {
			root_inode->i_blocks += root_inode->i_size / superblockdata->blk_size + 1 - root_inode->i_blocks;
		}
	}

	if (lseek(fd, superblockdata->inode_offset, SEEK_SET)<0) {
		printf("Error in lseek()!\n");
	}
	if (write(fd, (struct inode *)root_inode, sizeof(struct inode))<0) {
		printf("Error in write()!\n");
	}

	// DIR_NODE *TEMP = (DIR_NODE *)malloc(sizeof(DIR_NODE));
	//
	//
	// if (lseek(fd, superblockdata->data_offset, SEEK_SET)<0) {
	//  printf("Error in lseek()!\n");
	// }else{
	//  if (read(fd, (DIR_NODE *)TEMP, sizeof(DIR_NODE))<0) {
	//  }else{
	//      printf("Testing file name: %s \n",TEMP->dir);
	//      printf("inode no.: %d \n",TEMP->inode_number);
	//
	//  }
	//
	// }

	//Also construct the first inode

	close(fd);
	return 0;
}

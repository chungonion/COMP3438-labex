#include "call.h"
const char *HD = "../HD";

int path_find_t(char *name, int fd, struct superblock *superblock_data, struct inode *inode_data, int flags);
int open_t(char *pathname, int flags);
int read_t(int inode_number, int offest, void *buf, int count);
int write_t(int inode_number, int offest, void *buf, int count);
int inode_create(int fd, struct superblock* superblock_data, int type);
int inode_modify(int fd, struct superblock* superblock_data, int i_number, char* key, int value);
struct inode *inode_get(int fd, struct superblock* superblock_data, int i_number);
struct superblock *superblock_get(int fd);
int superblock_modify(int fd, struct superblock* superblock_data, char* key, int value);
int read_data_block(int fd, struct superblock* superblock_data, struct inode* inode_data, void *buf, int offset, int count);
int write_data_block(int fd, struct superblock* superblock_data, struct inode* inode_data, void *buf, int offset, int count);

int open_t(char *pathname, int flags) {
	//flag 0 = exist file and replace it
	//flag 1 = exist directory and replace it
	//flag 2 = target is an existing file
	int inode_number;
	int fd = open(HD, O_RDWR, 0660);
	if (!fd) {
		printf("Error in open()\n");
		return -1;
	}
	struct superblock *superblock_data = (struct superblock *)malloc(sizeof(struct superblock));
	superblock_data = superblock_get(fd);
	//Root inode manipulation=s
	struct inode *root_inode;
	root_inode = inode_get(fd, superblock_data, 0);
	inode_number = path_find_t(pathname, fd, superblock_data, root_inode, flags);
	// printf("Returned inode_number: %d\n", inode_number);
	close(fd);
	//TODO: find that in direct block 1 first, then on direct block 2

	//Testing purpose only
	// printf("inode offset: %d\n",superblock_data->inode_offset);
	// printf("data offset: %d\n",superblock_data->data_offset);
	// printf("max. inode: %d\n",superblock_data->max_inode);
	// printf("max data block: %d\n",superblock_data->max_data_blk);
	// printf("next available inode: %d\n",superblock_data->next_available_inode);
	// printf("next available block: %d\n",superblock_data->next_available_blk);
	// printf("block size: %d\n",superblock_data->blk_size);


	return inode_number;
}

int read_t(int inode_number, int offset, void *buf, int count) {
	//Flow
	//1) accept the input of arguments
	//2) if count+ offset > file size, or count > file max size, or offset > filesize ), return 0 ,
	// indicate nothing is read
	//3) read the direct pointer 0 first,
	//4) when necessary, read direct pointer 1
	int read_bytes = 0;
	// return 0;
	int fd = open(HD, O_RDWR, 0660);
	if (!fd) {
		printf("Error in open()\n");
		return -1;
	}
	struct superblock *superblock_data = superblock_get(fd);
	struct inode *inode_data = inode_get(fd, superblock_data, inode_number);
	// printf("offset: %d, i_size: %d\n",offset,inode_data->i_size);
	read_bytes = read_data_block(fd, superblock_data, inode_data, buf, offset, count);
	close(fd);
	return read_bytes;
}

int write_t(int inode_number, int offset, void *buf, int count) {
	int write_bytes = 0;

	int fd = open(HD, O_RDWR, 0660);
	if (!fd) {
		printf("Error in open()\n");
		return -1;
	}
	struct superblock *superblock_data = superblock_get(fd);
	struct inode *inode_data = inode_get(fd, superblock_data, inode_number);

	write_bytes = write_data_block(fd, superblock_data, inode_data, buf, offset, count);

	inode_data = inode_get(fd, superblock_data, inode_number);
	// printf("Inode %d's i_size: %d\n", inode_data->i_number, inode_data->i_size);

	// if (lseek(fd, superblock_data->data_offset + inode_data->direct_blk[0] * superblock_data->blk_size, SEEK_SET)<0) {
	//  printf("Error in lseek()!\n");
	//  // buf[0] = '\0';
	//  free(superblock_data);
	//  free(inode_data);
	//  return -1;
	// }
	// if (count > superblock_data)
	close(fd);
	return write_bytes;
}

int path_find_t(char *pathname, int fd, struct superblock *superblock_data, struct inode *input_inode_data, int flags) {
	int dir_node_total_size = 0;
	char *dir;
	char *delim = "/";

	dir = strtok(pathname, delim);
	// printf("strtok str: %s\n", dir);
	char *remain_part = strtok(NULL, "");
	// printf("pathname: %s\n", remain_part);

	if (dir == NULL && remain_part == NULL) {
		return 0;
	}


	// printf("====================\n");

	// printf("input_inode_data inumber: %d\n", input_inode_data->i_number);
	//Reading part
	do {
		if (lseek(fd, superblock_data->data_offset + input_inode_data->direct_blk[0] * superblock_data->blk_size + dir_node_total_size, SEEK_SET)<0) {
			printf("Error in lseek()!\n");
		}
		DIR_NODE *dir_node_data = (DIR_NODE *)malloc (sizeof(DIR_NODE));
		if (read(fd, (DIR_NODE *)dir_node_data, sizeof(DIR_NODE))<0) {
			printf("Error in reading the inode!!!\n");
		}
		if (strcmp(dir_node_data->dir, dir)==0) { //dir string found
			struct inode *next_inode = (struct inode *)malloc(sizeof(struct inode));
			if (lseek(fd, superblock_data->inode_offset + sizeof(struct inode) * dir_node_data->inode_number, SEEK_SET)<0) {
				printf("Error in lseek()!\n");
			}

			if (read(fd, (struct inode *)next_inode, sizeof(struct inode))<0) {
				printf("Error in reading the inode!!!\n");
			}

			if (remain_part!=NULL) {
				// printf("====================\n");
				return path_find_t(remain_part, fd, superblock_data, next_inode, flags);
			} else {
				// printf("====================\n");
				// printf("i number found: %d\n", dir_node_data->inode_number);
				switch (flags) {
					struct inode *new_inode_data;
					int i_number, return_value;
					case 0:
					{
						i_number = inode_create(fd, superblock_data, 0);
						new_inode_data = inode_get(fd, superblock_data, i_number);
						// DIR_NODE *current_dir_node = (DIR_NODE *)malloc(sizeof(DIR_NODE));
						dir_node_data->inode_number = i_number;
						// strcpy(current_dir_node->dir, dir);

						if (lseek(fd, superblock_data->data_offset + input_inode_data->direct_blk[0] * superblock_data->blk_size + dir_node_total_size, SEEK_SET)<0) {
							printf("Error in lseek()!\n");
						}

						if (write(fd, (DIR_NODE *)dir_node_data, sizeof(DIR_NODE))>=0) {

							inode_modify(fd, superblock_data, input_inode_data->i_number, "file_num", input_inode_data->file_num + 1);
							// inode_modify(fd, superblock_data, input_inode_data->i_number, "i_size", input_inode_data->i_size + sizeof(DIR_NODE));

						} else {
							printf("Error in write during findpath()!\n");
						}

						free(dir_node_data);

						return_value = new_inode_data->i_number;
						free(new_inode_data);
						return return_value;
					}

					case 1:
					{
						i_number = inode_create(fd, superblock_data, 0);
						new_inode_data = inode_get(fd, superblock_data, i_number);

						DIR_NODE *current_dir_node = (DIR_NODE *)malloc(sizeof(DIR_NODE));
						DIR_NODE *parent_dir_node = (DIR_NODE *)malloc(sizeof(DIR_NODE));

						strcpy(current_dir_node->dir, ".");
						current_dir_node->inode_number = i_number;

						//create
						if (lseek(fd, superblock_data->data_offset + new_inode_data->direct_blk[0] * superblock_data->blk_size + new_inode_data->i_size, SEEK_SET)<0) {
							printf("Error in lseek()!\n");
						}
						if (write(fd, (DIR_NODE *)current_dir_node, sizeof(DIR_NODE))>=0) {
							// printf("Before: file_num: %d, i_size: %d\n",new_inode_data->file_num,new_inode_data->i_size);

							inode_modify(fd, superblock_data, i_number, "file_num", new_inode_data->file_num + 1);
							inode_modify(fd, superblock_data, i_number, "i_size", new_inode_data->i_size + sizeof(DIR_NODE));
							new_inode_data = inode_get(fd, superblock_data, i_number);
							// printf("after: file_num: %d, i_size: %d\n",new_inode_data->file_num,new_inode_data->i_size);

						} else {
							printf("Error in write()!\n");
						}


						strcpy(parent_dir_node->dir, "..");
						parent_dir_node->inode_number = input_inode_data->i_number;



						if (lseek(fd, superblock_data->data_offset + new_inode_data->direct_blk[0] * superblock_data->blk_size + new_inode_data->i_size, SEEK_SET)<0) {
							printf("Error in lseek()!\n");
						}
						if (write(fd, (DIR_NODE *)parent_dir_node, sizeof(DIR_NODE))<0) {
							printf("Error in write()!\n");
						} else {
							inode_modify(fd, superblock_data, i_number, "file_num", new_inode_data->file_num + 1);
							inode_modify(fd, superblock_data, i_number, "i_size", new_inode_data->i_size + sizeof(DIR_NODE));
						}

						dir_node_data->inode_number = i_number;
						if (lseek(fd, superblock_data->data_offset + input_inode_data->direct_blk[0] * superblock_data->blk_size + dir_node_total_size, SEEK_SET)<0) {
							printf("Error in lseek()!\n");
						}

						if (write(fd, (DIR_NODE *)dir_node_data, sizeof(DIR_NODE))<0) {
							printf("Error in write()!\n");
						} else {
							// printf("Amending inode %d\n", input_inode_data->i_number);
							// inode_modify(fd, superblock_data, input_inode_data->i_number, "file_num", input_inode_data->file_num + 1);
							// inode_modify(fd, superblock_data, input_inode_data->i_number, "i_size", input_inode_data->i_size + sizeof(DIR_NODE));
						}

						free(current_dir_node);
						free(parent_dir_node);
						free(dir_node_data);
						return_value = new_inode_data->i_number;
						free(new_inode_data);
						return return_value; //TODO: return the file created
					}
					case 2:
						return_value = dir_node_data->inode_number;
						free(dir_node_data);
						return return_value;
					default:
						break;

				}
			}
			// if (child_inumber == -1) {
			//  return dir_node_data->inode_number; //TODO: create the file base on the conditions
			// } else {
			//  return child_inumber;
			// }
			//TODO: if the case is the last dir?
		}
		if (dir_node_data->inode_number == 0 && strcmp(dir_node_data->dir, "\0")==0) {
			break;
		}
		// printf("DIR: %s i-node: %d\n", dir_node_data->dir, dir_node_data->inode_number);
		dir_node_total_size += sizeof(DIR_NODE);
		free(dir_node_data);

	} while (dir_node_total_size<=input_inode_data->i_size); //TODO: looping condition is?
	// printf("====================\n");

	//TODO: create file if not there
	switch (flags) {
		struct inode *new_inode_data;
		int i_number;

		case 0:
			//create file

			if (remain_part==NULL) {
				i_number = inode_create(fd, superblock_data, 0);
				new_inode_data = inode_get(fd, superblock_data, i_number);
				DIR_NODE *current_dir_node = (DIR_NODE *)malloc(sizeof(DIR_NODE));
				current_dir_node->inode_number = i_number;
				strcpy(current_dir_node->dir, dir);

				if (lseek(fd, superblock_data->data_offset + input_inode_data->direct_blk[0] * superblock_data->blk_size + input_inode_data->i_size, SEEK_SET)<0) {
					printf("Error in lseek()!\n");
				}

				if (write(fd, (DIR_NODE *)current_dir_node, sizeof(DIR_NODE))>=0) {
					inode_modify(fd, superblock_data, input_inode_data->i_number, "file_num", input_inode_data->file_num + 1);
					inode_modify(fd, superblock_data, input_inode_data->i_number, "i_size", input_inode_data->i_size + sizeof(DIR_NODE));
				} else {
					printf("Error in write()!\n");
				}

				free(current_dir_node);

				int return_value = new_inode_data->i_number;
				free(new_inode_data);
				return return_value;

			} else {
				return -1;
			}
			break;
		case 1:
			if (remain_part==NULL && input_inode_data->i_type == 0) {
				i_number = inode_create(fd, superblock_data, 0);
				new_inode_data = inode_get(fd, superblock_data, i_number);

				DIR_NODE *current_dir_node = (DIR_NODE *)malloc(sizeof(DIR_NODE));
				DIR_NODE *parent_dir_node = (DIR_NODE *)malloc(sizeof(DIR_NODE));

				strcpy(current_dir_node->dir, ".");
				current_dir_node->inode_number = i_number;

				//create
				if (lseek(fd, superblock_data->data_offset + new_inode_data->direct_blk[0] * superblock_data->blk_size + new_inode_data->i_size, SEEK_SET)<0) {
					printf("Error in lseek()!\n");
				}
				if (write(fd, (DIR_NODE *)current_dir_node, sizeof(DIR_NODE))>=0) {
					// printf("Before: file_num: %d, i_size: %d\n",new_inode_data->file_num,new_inode_data->i_size);
					inode_modify(fd, superblock_data, i_number, "file_num", new_inode_data->file_num + 1);
					inode_modify(fd, superblock_data, i_number, "i_size", new_inode_data->i_size + sizeof(DIR_NODE));
					new_inode_data = inode_get(fd, superblock_data, i_number);
					// printf("after: file_num: %d, i_size: %d\n",new_inode_data->file_num,new_inode_data->i_size);
				} else {
					printf("Error in write()!\n");
				}

				strcpy(current_dir_node->dir, dir);
				if (lseek(fd, superblock_data->data_offset + input_inode_data->direct_blk[0] * superblock_data->blk_size + input_inode_data->i_size, SEEK_SET)<0) {
					printf("Error in lseek()!\n");
				}

				if (write(fd, (DIR_NODE *)current_dir_node, sizeof(DIR_NODE))>=0) {
					// printf("Amending inode %d\n", input_inode_data->i_number);
					inode_modify(fd, superblock_data, input_inode_data->i_number, "file_num", input_inode_data->file_num + 1);
					inode_modify(fd, superblock_data, input_inode_data->i_number, "i_size", input_inode_data->i_size + sizeof(DIR_NODE));
				} else {
					printf("Error in write()!\n");
				}

				strcpy(parent_dir_node->dir, "..");
				parent_dir_node->inode_number = input_inode_data->i_number;



				if (lseek(fd, superblock_data->data_offset + new_inode_data->direct_blk[0] * superblock_data->blk_size + new_inode_data->i_size, SEEK_SET)<0) {
					printf("Error in lseek()!\n");
				}
				if (write(fd, (DIR_NODE *)parent_dir_node, sizeof(DIR_NODE))>=0) {
					inode_modify(fd, superblock_data, i_number, "file_num", new_inode_data->file_num + 1);
					inode_modify(fd, superblock_data, i_number, "i_size", new_inode_data->i_size + sizeof(DIR_NODE));
				} else {
					printf("Error in write()!\n");
				}

				free(current_dir_node);
				free(parent_dir_node);

				int return_value = new_inode_data->i_number;
				free(new_inode_data);
				return return_value; //TODO: return the file created
			} else {
				return -1;
			}

			//Create directory
			break;
		case 2:
			return -1;
		default:
			return -1;

	}
	return -1; //error;
}

int create_dir_node(int fd, struct superblock* superblock_data) {
	return 1;
}

int inode_create(int fd, struct superblock* superblock_data, int type) {
	struct inode *new_inode_data = (struct inode *)malloc(sizeof(struct inode));
	struct inode *parent_inode_data = (struct inode *)malloc(sizeof(struct inode));
	if (superblock_data->next_available_inode>superblock_data->max_inode || superblock_data->next_available_blk>superblock_data->max_data_blk) {
		return -1;
	}
	new_inode_data->i_type = type;
	new_inode_data->i_number = superblock_data->next_available_inode;
	new_inode_data->file_num = 0;
	new_inode_data->i_size = 0;
	new_inode_data->direct_blk[0] = superblock_data->next_available_blk;
	// superblock_data->next_available_blk++
	// new_inode_data->direct_blk[1] = -1;
	// superblock_data->next_available_blk++
	// new_inode_data->indirect_blk = -1;

	time(&new_inode_data->i_mtime);

	if (lseek(fd, superblock_data->inode_offset + sizeof(struct inode) * superblock_data->next_available_inode, SEEK_SET)<0) {
		printf("Error in lseek()!\n");
	}

	if (write(fd, (struct inode *)new_inode_data, sizeof(struct inode))>=0) {
		// printf("superblock modify called!\n");
		superblock_modify(fd, superblock_data, "next_available_inode", superblock_data->next_available_inode + 1);
		superblock_modify(fd, superblock_data, "next_available_blk", superblock_data->next_available_blk + 1); //TODO: write back to superblock
	} else {
		printf("Error in write()!\n");
	}

	int i_number = new_inode_data->i_number;
	// printf("======================\n");
	// printf("new inode information\n");
	// printf("i_type: %d\n", new_inode_data->i_type);
	// printf("i_number: %d\n", new_inode_data->i_number);
	// printf("file_num: %d\n", new_inode_data->file_num);
	// printf("i_size: %d\n", new_inode_data->i_size);
	// printf("direct_blk [0]: %d\n", new_inode_data->direct_blk[0]);
	// printf("======================\n");

	free(new_inode_data);
	return i_number;

}

struct inode *inode_get(int fd, struct superblock* superblock_data, int i_number) {
	struct inode *new_inode_data = (struct inode *)malloc(sizeof(struct inode));

	if (lseek(fd, superblock_data->inode_offset + sizeof(struct inode) * i_number, SEEK_SET)<0) {
		printf("Error in lseek()!\n");
	}

	if (read(fd, (struct inode *)new_inode_data, sizeof(struct inode))<0) {
		printf("Error in read()!\n");
	}
	//
	// printf("======================\n");
	// printf("[GET]new inode information\n");
	// printf("i_type: %d\n", new_inode_data->i_type);
	// printf("i_number: %d\n", new_inode_data->i_number);
	// printf("file_num: %d\n", new_inode_data->file_num);
	// printf("i_size: %d\n", new_inode_data->i_size);
	// printf("direct_blk [0]: %d\n", new_inode_data->direct_blk[0]);
	// printf("======================\n");
	return new_inode_data;

}

int inode_modify(int fd, struct superblock* superblock_data, int i_number, char* key, int value) {
	struct inode *inode_data = (struct inode *)malloc(sizeof(struct inode));

	inode_data = inode_get(fd, superblock_data, i_number);

	if (strcmp(key, "i_number")==0) {
		inode_data->i_number = value;
	} else if (strcmp(key, "i_mtime")==0) {
		inode_data->i_mtime = value;
	} else if (strcmp(key, "i_type")==0) {
		inode_data->i_type = value;
	} else if (strcmp(key, "i_size")==0) {
		inode_data->i_size = value;
	} else if (strcmp(key, "i_blocks")==0) {
		inode_data->i_blocks = value;
	} else if (strcmp(key, "direct_blk[0]")==0) {
		inode_data->direct_blk[0] = value;
	} else if (strcmp(key, "direct_blk[1]")==0) {
		inode_data->direct_blk[1] = value;
	} else if (strcmp(key, "indirect_blk")==0) {
		inode_data->indirect_blk = value;
	} else if (strcmp(key, "file_num")==0) {
		inode_data->file_num = value;
	}

	time(&inode_data->i_mtime);

	//
	// printf("======================\n");
	// printf("[MODIFY]new inode information\n");
	// printf("i_type: %d\n", inode_data->i_type);
	// printf("i_number: %d\n", inode_data->i_number);
	// printf("file_num: %d\n", inode_data->file_num);
	// printf("i_size: %d\n", inode_data->i_size);
	// printf("direct_blk [0]: %d\n", inode_data->direct_blk[0]);
	// printf("======================\n");

	if (lseek(fd, superblock_data->inode_offset + sizeof(struct inode) * i_number, SEEK_SET)<0) {
		printf("Error in lseek()!\n");
		return -1;
	}

	if (write(fd, (struct inode *)inode_data, sizeof(struct inode))<0) {
		printf("Error in read()!\n");
		return -1;
	}


	free(inode_data);
	return 0;

}

struct superblock *superblock_get(int fd) {

	struct superblock *superblock_data = (struct superblock *)malloc(sizeof(struct superblock));

	if (lseek(fd, SB_OFFSET, SEEK_SET)<0) {
		printf("Error in lseek()!\n");
		// return -1;
	}

	if (read(fd, (struct superblock*)superblock_data, sizeof(struct superblock))<0) {
		printf("Error in read()!\n");
		// return -1;
	}

	return superblock_data;

}

int superblock_modify(int fd, struct superblock* superblock_data, char* key, int value) {
	if (strcmp(key, "inode_offset")==0) {
		superblock_data->inode_offset = value;
	} else if (strcmp(key, "data_offset")==0) {
		superblock_data->data_offset = value;
	} else if (strcmp(key, "max_inode")==0) {
		superblock_data->max_inode = value;
	} else if (strcmp(key, "max_data_blk")==0) {
		superblock_data->max_data_blk = value;
	} else if (strcmp(key, "next_available_inode")==0) {
		if (value > superblock_data->max_inode) {
			value = superblock_data->max_inode;
		} else {
			superblock_data->next_available_inode = value;
		}
	} else if (strcmp(key, "next_available_blk")==0) {
		if (value>superblock_data->max_data_blk) {
			value = superblock_data->max_data_blk;
		} else {
			superblock_data->next_available_blk = value;
		}
	} else if (strcmp(key, "blk_size")==0) {
		superblock_data->blk_size = value;
	}


	if (lseek(fd, SB_OFFSET, SEEK_SET)<0) {
		printf("Error in lseek()!\n");
		return -1;
	}

	if (write(fd, (struct superblock*)superblock_data, sizeof(struct superblock))<0) {
		printf("Error in read()!\n");
		return -1;
	}

	if (lseek(fd, SB_OFFSET, SEEK_SET)<0) {
		printf("Error in lseek()!\n");
		return -1;
	}


	if (read(fd, (struct superblock*)superblock_data, sizeof(struct superblock))<0) {
		printf("Error in read()!\n");
		return -1;
	}
	// printf("inode offset: %d\n", superblock_data->inode_offset);
	// printf("data offset: %d\n", superblock_data->data_offset);
	// printf("max. inode: %d\n", superblock_data->max_inode);
	// printf("max data block: %d\n", superblock_data->max_data_blk);
	// printf("next available inode: %d\n", superblock_data->next_available_inode);
	// printf("next available block: %d\n", superblock_data->next_available_blk);
	// printf("block size: %d\n", superblock_data->blk_size);
	return 0;

}

int read_data_block(int fd, struct superblock* superblock_data, struct inode* inode_data, void *buf, int offset, int count) {
	int read_bytes = 0;
	int max_bytes = inode_data->i_size;
	if (offset > inode_data->i_size) { //offset is too larget to fit in.
		// printf("offset: %d\n",offset);
		// printf("count: %d\n",count);
		// printf("isize %d\n",inode_data->i_size);
		// printf("Read blocks failed!\n");
		return 0;
	}

	if (count > max_bytes) {
		count = max_bytes;
	}

	if (offset<superblock_data->blk_size) {
		int read_bytes = 0; //non global var
		int max_bytes = inode_data->i_size;
		if (lseek(fd, superblock_data->data_offset + inode_data->direct_blk[0] * superblock_data->blk_size + offset, SEEK_SET)<0) {
			printf("LSEEK ERROR!\n");
		}

		if (count < superblock_data->blk_size) {
			read_bytes = count;
		} else {
			read_bytes = superblock_data->blk_size;
		}

		// printf("offset: %d, read_bytes: %d\n",offset,read_bytes);

		if (read(fd, buf, read_bytes)>=0) {
			if (count < superblock_data->blk_size) {
				return count;
			} else {
				return read_bytes + read_data_block(fd, superblock_data, inode_data, buf, superblock_data->blk_size, count - read_bytes);
			}
		}

	} else if (offset<superblock_data->blk_size * 2){
		int read_bytes = 0;
		int offset_remainder = offset -superblock_data->blk_size;
		if (inode_data->direct_blk[1]<=0) {
			//reclaim new data block
			return 0;
		}
		if (lseek(fd, superblock_data->data_offset + inode_data->direct_blk[1] * superblock_data->blk_size+offset_remainder, SEEK_SET)<0) {
			printf("LSEEK ERROR!\n");
		}


		if (count < superblock_data->blk_size) {
			read_bytes = count;
		} else {
			read_bytes = superblock_data->blk_size;
		}
		// printf("offset: %d, read_bytes: %d\n",offset,read_bytes);

		if (read(fd, buf + offset, read_bytes)>=0) {
			if (count < superblock_data->blk_size) {
				return count;
			} else {
				return read_bytes + read_data_block(fd, superblock_data, inode_data, buf, offset + superblock_data->blk_size, count - read_bytes);
			}

		}
	} else {

		int read_bytes = 0;
		if (inode_data->indirect_blk<=0) {
			//reclaim new data block
			// printf("print 2 ended!\n");
			return 0;
		}

		struct indir_pointer *indir_pointer_data = (struct indir_pointer *)malloc(sizeof(struct indir_pointer));
		int indir_block_position;
		indir_block_position = (count + offset) / superblock_data->blk_size - 2;
		if (lseek(fd, superblock_data->data_offset + inode_data->indirect_blk * superblock_data->blk_size + indir_block_position * sizeof(struct indir_pointer), SEEK_SET)<0) {
			printf("LSEEK ERROR!\n");
		}
		if (read(fd, indir_pointer_data, sizeof(struct indir_pointer))>=0) {

		}


		if (lseek(fd, superblock_data->data_offset + indir_pointer_data->block_index * superblock_data->blk_size, SEEK_SET)<0) {
			printf("LSEEK ERROR!\n");
		}


		if (count < superblock_data->blk_size) {
			read_bytes = count;
		} else {
			read_bytes = superblock_data->blk_size;
		}
		// printf("offset: %d, read_bytes: %d\n",offset,read_bytes);

		if (read(fd, buf + offset,read_bytes)>=0) {
			if (count < superblock_data->blk_size) {
				// printf("Size of file: %d\n", inode_get(fd, superblock_data, inode_data->i_number)->i_size);
				return count;
			} else {
				return read_bytes + read_data_block(fd, superblock_data, inode_data, buf, offset + superblock_data->blk_size, count - read_bytes);
			}
			//TODO: I-node-manipulation
		}



		//TODO: handle indirect pointer issue
	}

	return read_bytes;
}

int write_data_block(int fd, struct superblock* superblock_data, struct inode* inode_data, void *buf, int offset, int count) {
	int write_bytes = 0;
	int max_bytes = inode_data->i_size;

	if (count > MAX_FILE_SIZE) {
		count = MAX_FILE_SIZE;
	}

	if (offset<superblock_data->blk_size) { //direct pointer [0]
		int write_bytes = 0;
		if (lseek(fd, superblock_data->data_offset + inode_data->direct_blk[0] * superblock_data->blk_size, SEEK_SET)<0) {
			printf("LSEEK ERROR!\n");
		}

		if (count < superblock_data->blk_size) {
			write_bytes = count;
		} else {
			write_bytes = superblock_data->blk_size;
		}

		if (write(fd, buf + offset, write_bytes)>=0) {
			inode_modify(fd, superblock_data, inode_data->i_number, "i_size", write_bytes + offset);
			if (count < superblock_data->blk_size) {
				// printf("Size of file: %d\n", inode_get(fd, superblock_data, inode_data->i_number)->i_size);
				return count;
			} else {
				return write_bytes + write_data_block(fd, superblock_data, inode_data, buf, offset + superblock_data->blk_size, count - write_bytes);
			}
			//TODO: I-node-manipulation
		}

	} else if (offset<superblock_data->blk_size * 2) { //direct pointer [1]
		int write_bytes = 0;
		if (inode_data->direct_blk[1]<=0) {
			//reclaim new data block
			inode_data->direct_blk[1] = superblock_data->next_available_blk;
			inode_modify(fd,superblock_data,inode_data->i_number, "direct_blk[1]", inode_data->direct_blk[1]);
			inode_data = inode_get(fd, superblock_data, inode_data->i_number);
			superblock_modify(fd, superblock_data, "next_available_blk", superblock_data->next_available_blk + 1);
		}

		if (lseek(fd, superblock_data->data_offset + inode_data->direct_blk[1] * superblock_data->blk_size, SEEK_SET)<0) {
			printf("LSEEK ERROR!\n");
		}

		if (count < superblock_data->blk_size) {
			write_bytes = count;
		} else {
			write_bytes = superblock_data->blk_size;
		}

		if (write(fd, buf + offset, write_bytes)>=0) {
			inode_modify(fd, superblock_data, inode_data->i_number, "i_size", write_bytes + offset);
			// printf("count: %d, write bytes: %d\n",count,write_bytes);

			if (count < superblock_data->blk_size) {
				// printf("Size of file: %d\n", inode_get(fd, superblock_data, inode_data->i_number)->i_size);
				return count;
			} else {
				return write_bytes + write_data_block(fd, superblock_data, inode_data, buf, offset + superblock_data->blk_size, count - write_bytes);
			}
			//TODO: I-node-manipulation
		}
	} else {
		int write_bytes;
		if (inode_data->indirect_blk<=0) {
			//reclaim new data block
			inode_data->indirect_blk = superblock_data->next_available_blk;
			inode_modify(fd,superblock_data,inode_data->i_number, "indirect_blk", inode_data->indirect_blk);
			inode_data = inode_get(fd, superblock_data, inode_data->i_number);
			superblock_modify(fd, superblock_data, "next_available_blk", superblock_data->next_available_blk + 1);
		}

		int new_pointer_block_position = (offset + count) / superblock_data->blk_size - 2;
		struct indir_pointer *indir_pointer_data = (struct indir_pointer *)malloc(sizeof(struct indir_pointer));
		indir_pointer_data->block_index = superblock_data->next_available_blk;
		superblock_modify(fd, superblock_data, "next_available_blk", superblock_data->next_available_blk + 1);
		if (lseek(fd, superblock_data->data_offset + inode_data->indirect_blk * superblock_data->blk_size + new_pointer_block_position * sizeof(struct indir_pointer), SEEK_SET)<0) {
			printf("LSEEK ERROR!\n");
		}
		if (write(fd, indir_pointer_data, sizeof(struct indir_pointer))>=0) {

		}
		if (lseek(fd, superblock_data->data_offset + indir_pointer_data->block_index * superblock_data->blk_size, SEEK_SET)<0) {
			printf("LSEEK ERROR!\n");
		}
		if (count < superblock_data->blk_size) {
			write_bytes = count;
		} else {
			write_bytes = superblock_data->blk_size;
		}

		// printf("offset: %d, write_bytes: %d\n",offset,write_bytes);


		if (write(fd, buf + offset, write_bytes)>=0) {
			inode_modify(fd, superblock_data, inode_data->i_number, "i_size", write_bytes + offset);
			if (count < superblock_data->blk_size) {
				// printf("Size of file: %d\n", inode_get(fd, superblock_data, inode_data->i_number)->i_size);
				return count;
			} else {
				return write_bytes + write_data_block(fd, superblock_data, inode_data, buf, offset + superblock_data->blk_size, count - write_bytes);
			}
			//TODO: I-node-manipulation
		}


	}

	return write_bytes;
}

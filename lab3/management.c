#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "layout.h"
#include "information.h"

int main () {
//Attain the layout of record file
	int fd = open(file_name, O_RDWR, 0660);
	if (!fd) {
		printf("Error in open()\n");
		return -1;
	}
	struct layout *metadata = (struct layout *)malloc(sizeof(struct layout));
	if (read(fd, (struct layout *)metadata, sizeof(struct layout))<0) {
		printf("Error in read() of attainning the layout\n");
		// POLYU Lucy Jack
		return -1;
	}
//store POLYU's structure
	struct information *POLYU = (struct information *)malloc (sizeof(struct information));
	POLYU->num = 0;
	POLYU->sub_0 = 0;
	// printf("%d\n",metadata->offset_stu);
	if (lseek(fd, metadata->offset_stu, SEEK_SET)<0) {
		printf("Error in lseek() of storing POLYU\n");
		return -1;
	}
	if (write(fd, (void *)POLYU, sizeof (struct information))<0) {
		printf("Error in write() of storing POLYU\n");
		return -1;
	}
//store POLYU's mapping on its first block
	struct mapping *entry = (struct mapping *)malloc (sizeof (struct mapping));
	printf("Lseek offset: %d\n",metadata->offset_sub + POLYU->sub_0 * homework_size);
	if (lseek (fd, metadata->offset_sub + POLYU->sub_0 * homework_size, SEEK_SET)<0) {
		printf("Error in lseek() of storing mapping\n");
		return -1;
	}
	strcpy(entry->name, "Lucy");
	entry->num = 1;
	if (write(fd, (void *)entry, sizeof(struct mapping))<0) {
		printf("Error in write() of storing mapping\n");
		return -1;
	}
	strcpy(entry->name, "Jack");
	entry->num = 2;
	if (write(fd, (void *)entry, sizeof(struct mapping))<0) {
		printf("Error in write() of storing mapping\n");
		return -1;
	}
//store Lucy's and Jack's information
	struct information *Lucy = (struct information *)malloc (sizeof(struct information));
	Lucy->num = 1;
	Lucy->sub_0 = 1;
	Lucy->sub_1 = 2;
	printf("offset_stu: %d\n",metadata->offset_stu);
	if (lseek(fd, metadata->offset_stu + (Lucy->num) * sizeof(struct information), SEEK_SET)<0) {
		printf("Error in lseek() of storing Lucy structure information\n");
		return -1;
	}

	if (write(fd, (void *)Lucy, sizeof (struct information))<0) {
		printf("Error in write() of storing Lucy structure information\n");
		return -1;
	}

	struct information *Jack = (struct information *)malloc (sizeof(struct information));
	Jack->num = 2;
	if (write(fd, (void *)Jack, sizeof (struct information))<0) {
		printf("Error in write() of storing Jack structure information\n");
		return -1;
	}
//store Lucy's homework
	char math[50] = "5+5=10";
	char english[50] = "hello world";
//store Lucy's Math homework
	printf("%d\n",metadata->offset_sub);
	printf("%d\n",POLYU->sub_0);
	if (lseek(fd, metadata->offset_sub + Lucy->sub_0 * homework_size, SEEK_SET)<0) {
		printf("Error in lseek() of storing Math homework\n");
		return -1;
	}
	if (write(fd, (void *)math, sizeof(math))<0) {
		printf("Error in write() of storing Math homework\n");
		return -1;
	}
//store Lucy's English homework
	if (lseek(fd, metadata->offset_sub + Lucy->sub_1 * homework_size, SEEK_SET)<0) {
		printf("Error in lseek() of storing English homework\n");
		return -1;
	}
	if (write(fd, (void *)english, sizeof(english))<0) {
		printf("Error in write() of storing English homework\n");
		return -1;
	}
	close(fd);
//TODO: retrieve Lucy's homework.
	Lucy->num = 0; //Assume she forget the number, we have to find that out.

	return 0;
}

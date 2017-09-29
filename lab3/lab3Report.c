#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "layout.h"
#include "information.h"

int main() {
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
	struct information *POLYU = (struct information *)malloc (sizeof(struct information));
	if (read(fd, (void *)POLYU, sizeof (struct information))<0) {
		printf("Error in write() of storing POLYU\n");
		return -1;
	}

	struct mapping *entry = (struct mapping *)malloc (sizeof (struct mapping));

	// printf("Lseek offset: %d\n",metadata->offset_sub + POLYU->sub_0 * homework_size);
	if (lseek (fd, metadata->offset_sub + POLYU->sub_0 * homework_size, SEEK_SET)<0) {
		printf("Error in lseek() of storing mapping\n");
		return -1;
	}

	do {
		if (read(fd, (void *)entry, sizeof(struct mapping))<0) {
			printf("Error in write() of storing mapping\n");
			return -1;
		}
		if (strcmp(entry->name, "Lucy")==0) {
			struct information *Lucy = (struct information *)malloc (sizeof(struct information));
            char math[50] = "5+5=10";
            char english[50] = "hello world";

			if (lseek(fd, metadata->offset_stu + entry->num * sizeof(struct information), SEEK_SET)<0) {
				printf("Error in lseek() of storing Lucy structure information\n");
				return -1;
			}
			if (read(fd, (struct information *)Lucy, sizeof (struct information))<0) {
				printf("Error in write() of storing Lucy structure information\n");
				return -1;
			}

            if (lseek(fd, metadata->offset_sub + Lucy->sub_0 * homework_size, SEEK_SET)<0) {
                printf("Error in lseek() of storing Math homework\n");
                return -1;
            }
            if (read(fd, math, sizeof(math))<0) {
                printf("Error in write() of storing Math homework\n");
                return -1;
            }
            printf("Lucy's Math homework: %s\n", math);

            //store Lucy's English homework
            if (lseek(fd, metadata->offset_sub + Lucy->sub_1 * homework_size, SEEK_SET)<0) {
                printf("Error in lseek() of storing English homework\n");
                return -1;
            }
            if (read(fd, english, sizeof(english))<0) {
                printf("Error in write() of storing English homework\n");
                return -1;
            }
            printf("Lucy's English homework: %s\n", english);
            close(fd);
            break;
		}
		/* code */
	} while (entry->num>0);

	return 0;
}

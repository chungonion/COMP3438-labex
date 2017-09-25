#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

const int STRUCT_ENTITIES_NOS = 4;
const int BUFFER_LENGTH = 100;
int fd = 0;

struct Books {
	char title[50];
	char author[50];
	char subject[100];
	int book_id;
};


/* function declaration */
void printBook(int entry);
void writeBookToFile(struct Books *book);
void openFd();
void closeFd();
int lseekOffset(int offset);

int main() {
	struct Books * Book1;       /* Declare pointer Book1 of type Book */
	struct Books * Book2;       /* Declare pointer Book2 of type Book */

	//Dynamically allocate memory space for Book1 and Book2
	Book1 = malloc(sizeof *Book1); //TODO: here!
	Book2 = malloc(sizeof *Book2);

	/* book 1 specification */
	strcpy(Book1->title, "C Programming");
	strcpy(Book1->author, "Nuha Ali");
	strcpy(Book1->subject, "C Programming Tutorial");
	Book1->book_id = 6495407;

	/* book 2 specification */
	strcpy(Book2->title, "Telecom Billing");
	strcpy(Book2->author, "Zara Ali");
	strcpy(Book2->subject, "Telecom Billing Tutorial");
	Book2->book_id = 6495700;

	//TODO: instead of printing the book, output the book as a file
	//TODO: using open() and write() and
	openFd();
	// printBook(Book1);

	// printBook(Book2);
	writeBookToFile(Book1);
	writeBookToFile(Book2);
	/* print Book1 info by passing address of Book1 */
	printBook(1);
	/* print Book2 info by passing address of Book2 */
	printBook(2);
	closeFd();
	return 0;
}
// void printBook(struct Books *book) {
//  if (fd <=0) {
//      return;
//  }
//  printf("Book title : %s\n", book->title);
//  printf("Book author : %s\n", book->author);
//  printf("Book subject : %s\n", book->subject);
//  printf("Book book_id : %d\n", book->book_id);
// }

void printBook(int entry) {
	char buf[100];
	int n;
	if (fd <=0) {
		return;
	}

	//print the particulars:
	if (lseekOffset((entry - 1) * STRUCT_ENTITIES_NOS * BUFFER_LENGTH + 0 * BUFFER_LENGTH)==1) {
		return;
	}
	n = read(fd,buf,sizeof(buf));
	buf[n] = 0;
	printf("Book title : %s\n",buf);
	memset(buf,0,sizeof(buf));

	if (lseekOffset((entry - 1) * STRUCT_ENTITIES_NOS * BUFFER_LENGTH + 1 * BUFFER_LENGTH)==1) {
		return;
	}
	n = read(fd,buf,sizeof(buf));
	buf[n] = 0;
	printf("Book title : %s\n",buf);
	memset(buf,0,sizeof(buf));


	if (lseekOffset((entry - 1) * STRUCT_ENTITIES_NOS * BUFFER_LENGTH + 2 * BUFFER_LENGTH)==1) {
		return;
	}
	n = read(fd,buf,sizeof(buf));
	buf[n] = 0;
	printf("Book title : %s\n",buf);
	memset(buf,0,sizeof(buf));

	if (lseekOffset((entry - 1) * STRUCT_ENTITIES_NOS * BUFFER_LENGTH + 3 * BUFFER_LENGTH)==1) {
		return;
	}
	n = read(fd,buf,sizeof(buf));
	buf[n] = 0;
	printf("Book title : %s\n",buf);
	memset(buf,0,sizeof(buf));

	printf("\n");

}

void writeBookToFile(struct Books *book) {
	char buf[100]; //Initialise the memory, ensure no other characters inside
	int n;
	if (fd <=0) {
		return;
	}
	memset(buf,0,sizeof(buf));
	strcpy(buf,book->title);
	write(fd,buf,sizeof(buf));
	// printf("%lu\n",sizeof(book->title));
	// write(fd," ",1);
	memset(buf,0,sizeof(buf));
	strcpy(buf,book->author);
	write(fd,buf,sizeof(buf));
	// write(fd,book->author,sizeof(book->author));
	// write(fd," ",1);
	memset(buf,0,sizeof(buf));
	strcpy(buf,book->subject);
	write(fd,buf,sizeof(buf));
	// write(fd,book->subject,sizeof(book->subject));
	// write(fd," ",1);
	memset(buf,0,sizeof(buf));
	sprintf(buf, "%d", book->book_id);
	write(fd,buf,sizeof(buf));
	// write(fd," ",1);
}

void openFd() {
	if ((fd = open("mybook", O_CREAT | O_TRUNC | O_RDWR, 00644)) < 0) {
		printf("Error in open()\n");
		exit(-1);
	}
}

void closeFd() {
	close(fd);
}

int lseekOffset(int offset) {
	if (lseek(fd,offset, SEEK_SET) < 0) {
		printf("Error in lseek()\n");
		return 1;
	}
	else{
		return 0;
	}
}

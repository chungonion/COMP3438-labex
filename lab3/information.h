#ifndef _INFORMATION_
#define _INFORMATION_

//The file stores students' information
const char *file_name = "info.txt";

struct information{
	int num;          //objection structure number
	int sub_0;        //the first block ID
	int sub_1;        //the second block ID
};

struct mapping{
	char name[15];     //objection name
	int num;           //objection structure number
};

#endif

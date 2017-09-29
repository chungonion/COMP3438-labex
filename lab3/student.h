#ifndef _STUDENT_
#define _STUDENT_

//The file stores students' information
const char *file_name = "info.txt";

struct student {
	int num;              //student's number
	char name[15];        //student's name
	int sub_0;            //the first subject homework block ID
	int sub_1;            //the second subject homework block ID
};

#endif

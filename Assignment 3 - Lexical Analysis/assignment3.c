#include <stdio.h>
#include <string.h>
#include <fcntl.h>

/* Some definitions */
#define MAXBUFSIZE 1000
#define Max_Token_Size 100
#define AB TOKEN_AB

typedef enum {
	KEYWORD,
	COMMA,
	SEMICOLON,
	ASSIGN,
	PERIOD,
	NUM,
	PLUS,
	MINUS,
	MUL,
	DIV,
	LBRACE,
	RBRACE,
	ID
} TokenType;

/*----- The prototype of functions in this functions---*/
int get_next_token(void);
int scan(void);
void print_token(TokenType token);
int fail(int cstate);
int letter_check(char input);
int num_check(char input);
int keyword_check(char *input);

/*--------------- The Global variables----------------*/
int fd; /* File descriptor to the input file*/

char input_buf[MAXBUFSIZE]; /* Maximum buffer size */

char *start_pt; /*The pointer pointing to the starting  position for
                                    the current token recongnizatioin*/

int forward;                    /*The distance from start_pt for the
                                    current token recongnization*/
char token_val[Max_Token_Size]; /* The token lexeme value*/

/*----------Function: print_token()--------------------
    Print token based on definition and requirment
   ------------------------------------------------------*/
void print_token(TokenType token) {
	switch (token) {
		case KEYWORD:
			printf("KEYWORD\t\t\t| %s\n", token_val);
			break;
		case COMMA:
			printf("COMMA\t\t\t| \n");
			break;
		case SEMICOLON:
			printf("SEMICOLON\t\t| \n");
			break;
		case ASSIGN:
			printf("ASSIGN\t\t\t| \n");
			break;
		case PERIOD:
			printf("PERIOD\t\t\t| \n");
			break;
		case NUM:
			printf("NUM\t\t\t| %s\n", token_val);
			break;
		case PLUS:
			printf("PLUS\t\t\t| \n");
			break;
		case MINUS:
			printf("MINUS\t\t\t| \n");
			break;
		case MUL:
			printf("MUL\t\t\t| \n");
			break;
		case DIV:
			printf("DIV\t\t\t| \n");
			break;
		case LBRACE:
			printf("LBRACE\t\t\t| \n");
			break;
		case RBRACE:
			printf("RBRACE\t\t\t| \n");
			break;
		case ID:
			printf("ID\t\t\t| %s\n", token_val);
			break;
		default:
			break;
	}
}

/*---------Function: fail()----------------------------
   Based on the current state, to decide the next state
    current state < 4, next state <- 4
    current state < 9, next state <- 9
    current state < 12, next state <- 12
   -------------------------------------------------------*/
int fail(int cstate) {
	int next_state;
	forward = 0;
	// printf("failed!\n");
	if (cstate < 4) {
		next_state = 4;
	} else if (cstate < 9) {
		next_state = 9;
	} else if (cstate < 12) {
		next_state = 12;
	} else{
		next_state = 12;
	}

	return next_state;
}

/*--------Function: get_next_token()-----------------------
   Return the next token.
   Note: Return -1 if normal finish;
         Return -2 if there is non-recongnizd character
   ---------------------------------------------------------*/
int get_next_token() {
	char c;
	int i;
	int current_state = 0;
	forward = 0;

	while (1) {
		c = start_pt[forward];
		// printf("C: %c\n",c);

		switch (current_state) {
			case 0:
				if (c == ' ' || c == '\t' || c == '\n') {
					start_pt++;
					current_state = 0;
				} else {
					current_state = 1;
				}
				break;

			case 1:
				if (letter_check(c)) {
					forward++;
					current_state = 2;
				} else {
					current_state = fail(current_state);
				}
				break;

			case 2:
				if (letter_check(c) || num_check(c)) {
					forward++;
					current_state = 2;
				} else {
					current_state = 3;
				}
				break;
			case 3:
				forward--;
				for (i = 0; i <= forward; i++)
					token_val[i] = start_pt[i];
				token_val[i] = '\0';
				return ID;

			case 4:
				if (num_check(c)) {
					forward++;
					current_state = 5;
				} else {
					current_state = fail(current_state);
				}
				break;

			case 5:
				if (num_check(c)) {
					forward++;
					current_state = 5;
				} else if (c == '.') {
					forward++;
					current_state = 6;
				} else {
					current_state = fail(current_state);
				}

				break;

			case 6:
				if (num_check(c)) {
					forward++;
					current_state = 7;
				} else {
					current_state = fail(current_state);
				}

				break;

			case 7:
				if (num_check(c)) {
					forward++;
					current_state = 7;
				} else {
					current_state = 8;
				}

			case 8:
				forward--;
				for (i = 0; i <= forward; i++)
					token_val[i] = start_pt[i];
				token_val[i] = '\0';
				return NUM;

			case 9:
				if (num_check(c)) {
					forward++;
					current_state = 10;
				} else {
					current_state = fail(current_state);
				}
				break;
			case 10:
				if (num_check(c)) {
					forward++;
					current_state = 10;
				} else {
					current_state = 11;
				}
				break;
			case 11:
				forward--;
				for (i = 0; i <= forward; i++)
					token_val[i] = start_pt[i];
				token_val[i] = '\0';
				return NUM;

			case 12:
				switch (c) {
					case ',':
						current_state = 13;
						break;
					case ';':
						current_state = 14;
						break;
					case '=':
						current_state = 15;
						break;
					case '.':
						current_state = 16;
						break;
					case '+':
						current_state = 17;
						break;
					case '-':
						current_state = 18;
						break;
					case '*':
						current_state = 19;
						break;
					case '/':
						current_state = 20;
						break;
					case '(':
						current_state = 21;
						break;
					case ')':
						current_state = 22;
						break;
					case '\0':
						return -1;
					default:
						return -2;
				}
				break;
			case 13:
				return COMMA;
			case 14:
				return SEMICOLON;
			case 15:
				return ASSIGN;
			case 16:
				return PERIOD;
			case 17:
				return PLUS;
			case 18:
				return MINUS;
			case 19:
				return MUL;
			case 20:
				return DIV;
			case 21:
				return LBRACE;
			case 22:
				return RBRACE;

			default:
				printf("Wrong state (should not occur)\n");
				return -2;
		}

	}
}

/*-------------Function: scan() ---------------------------
    Get the return token and call print_token to print it
   ----------------------------------------------------------*/
int scan() {
	int ret, rbytes;

	start_pt = input_buf;

	/* Read the input file into a buffer */
	/* Only consider the simple case when the buffer is big enough*/
	rbytes = read(fd, input_buf, MAXBUFSIZE);

	if (rbytes < 0) {
		printf("Could not read from the file\n");
		close(fd);
		return -1;
	} else if (rbytes == 0) {
		printf("The input file is empty.\n");
		close(fd);
		return -1;
	} else {
		input_buf[rbytes] = '\0'; /*the file ends with \0*/ 
		//set input_buf[rbytes] instead of input_buf[rbytes-1] to tackle file not ending in \n
	}

	printf("Token Name\t\t|Token Value \n");
	printf("--------------------------------------------\n");
	while ((ret = get_next_token()) >= 0) {
		if (ret == ID) {
			if (keyword_check(token_val)) {
				ret = KEYWORD;
			}
		}
		print_token(ret);
		// printf("Forward: %d\n",forward);
		start_pt = start_pt + forward + 1;
	}

	if (ret == -1)
		printf("\nSucessfuly finish scan.\n");
	else
		printf("\nError: Non-recongnizable character in the input file.\n");

	close(fd);
	return 1;
}

int letter_check(char input) {
	if ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z')) {
		// printf("letter:%c passed! \n", letter);
		return 1;
	} else {
		// printf("letter:%c failed! \n", letter);

		return 0;
	}
}

int num_check(char input) {
	if (input >= '0' && input <= '9') {
		return 1;
	} else {
		return 0;
	}
}

int keyword_check(char *input) {
	if (strcmp("var", token_val) == 0 || strcmp("begin", token_val) == 0 || strcmp("end", token_val) == 0) {
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[]) {
	/* Check the number of input*/
	if (argc != 2) {
		printf("Usage: scan input_file \n");
		return -1;
	}

	/* Open the source fie*/
	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		printf("Can not open the file (File_Name:%s).\n", argv[1]);
		return -1;
	}

	/* Process lexical analysis*/
	if (scan() > 0)
		return 1;
	else
		return -1;
}

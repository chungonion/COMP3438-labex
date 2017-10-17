#include <stdio.h>
#include <stdlib.h>

int main () {
	int input_num;

	do {
		int num_divisor = 1;
        int is_polydivisible = 1; // 1 = true, 0 = false;
		printf("Please give me an input: ");
		scanf("%d", &input_num); //ask for an int input, and put that to input_num variable
		if (input_num<0 || input_num >2076543270) { //Invalid input as specified
            printf("Invalid input!\n");
			continue; //back to ask for input;
		}

        do{
            if (input_num/num_divisor>0){
                //Check how many digits are there, by multiply the divisor by 10
                //if the divisor>input_num, indicating the inputnum is (log divisor -1) digits.
                num_divisor*=10;
            }else{
                int num_digits = 1;
                num_divisor/=10; //divide by 10, so that input_num >= num_divisor
                do{
                    int quotient = input_num/num_divisor;
                    //e.g input_num = 101200, divisor will be 100000 for the firsttime
                    //quotient = 1; and num_digits = 1;
                    //second time: divisor will be 10000, and quotient = 10, num digits = 2;
                    // printf("digits: %d, quotient: %d\n",num_digits,quotient);
                    if (quotient%num_digits!=0){
                        is_polydivisible =0; //if one does not match the def. of polydivisible,
                        //then mark as not polydivisible
                    }
                    num_divisor/=10; //divide by 10;
                    num_digits++;

                }while(num_divisor>0);


                break;
            }

        }while(1);

        if (is_polydivisible==1){
            printf("Poly ok!\n");
        }else{
            printf("not poly!\n");
        }

        break;

	} while (1);


	return 0;
}

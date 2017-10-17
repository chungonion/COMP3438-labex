#include <stdio.h>
#include <stdlib.h>

int main () {
	int input_num;

	do {
		int num_divisor = 1;
        int is_polydivisible = 1; // 1 = true, 0 = false;
		printf("Please give me an input: ");
		scanf("%d", &input_num);
		if (input_num<0 || input_num >2076543270) {
            printf("Invalid input!\n");
			continue;
		}

        do{
            if (input_num/num_divisor>0){
                // printf("%d\n",num_divisor);
                num_divisor*=10;
                // num_digits++;

            }else{
                int num_digits = 1;

                num_divisor/=10;
                // num_digits--;
                do{
                    int quotient = input_num/num_divisor;
                    // printf("digits: %d, quotient: %d\n",num_digits,quotient);
                    if (quotient%num_digits!=0){
                        is_polydivisible =0;
                    }
                    num_divisor/=10;
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

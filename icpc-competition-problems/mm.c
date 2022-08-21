#include <stdio.h>
#include <string.h>
#define SIZE 1000000

int main()
{
    int num = 0;
    int result = 0;
    char ch_result[SIZE] = {0};
    char IDnumber[SIZE] = {0};
    scanf("%s", IDnumber);
    for(int i = 0; i < SIZE; i++){
        num = (int)(IDnumber[i] - '0');
        if(num < 0){
            break;
        }
        result += num;
    }
    while(result >= 10){
        for(int i = 0; i < SIZE; i++){
            IDnumber[i] = '0';
        }
        sprintf(IDnumber, "%d", result);
        result = 0;
        for(int i = 0; i < SIZE; i++){
            num = (int)(IDnumber[i] - '0');
            if(num < 0){
                break;
            }
            result += num;
        }
    }
    sprintf(ch_result, "%d", result);
    printf("%s\n", ch_result);
    return 0;
}

#include <stdio.h>

int main() {
    int distance[1001] = {0};
    int speed[1001] = {0};
    int n = 0;
    int total_time = 0;
    int minimal = 10000;
    
    scanf("%d %d", &n, &total_time);

    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &distance[i], &speed[i]);
        if(minimal > speed[i]){
            minimal = speed[i];
        }
    }

    double low_limits = -minimal + 1e-9;
    double high_limits = 10000000;

    for (int i = 0; i < 100; ++i) {
        double middle_number = (low_limits + high_limits) / 2;
        double round_middle = 0;
        for (int i = 0; i < n; i++) {
            round_middle += ((double)distance[i]) / (speed[i] + middle_number);
        }
        if (round_middle > total_time)
            low_limits = middle_number;
        else
            high_limits = middle_number;
    }
    char ch_result[1100] = {0};
    sprintf(ch_result, "%f", low_limits);
    printf("%s\n", ch_result);
    return 0;
}

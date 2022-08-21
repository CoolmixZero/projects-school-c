#include <stdio.h>
#include <math.h>
#define M_PI 3.1415926535	

typedef struct sphere {
    double r;
    double x;
    double y;
    double z;
} sphere;

sphere spheres_number(double buff_r, double buff_x, double buff_y, double buff_z) {
    sphere old_num;

    old_num.r = buff_r;
    old_num.x = buff_x;
	old_num.y = buff_y;
    old_num.z = buff_z;
	
    return old_num;
}

double find_volume(sphere radius) {
    return 4.0 / 3 * M_PI * radius.r * radius.r * radius.r;
}

int main(){
    int n_holes = 0;
    int n_slices = 0;
    scanf("%d %d", &n_holes, &n_slices);
	sphere holes[n_holes];
    double vol = 100000L * 100000L * 100000L;
    for (int i = 0; i < n_holes; ++i) {
        double r = 0, x = 0, y = 0, z = 0;
        scanf("%lf %lf %lf %lf", &r, &x, &y, &z);
        holes[i] = spheres_number(r, x, y, z);
        vol -= find_volume(holes[i]);
    }

    double current = 0;
    double slice = vol / n_slices;

    for (int i = 1; i < n_slices; ++i) {
        double high_limit = 100000.0;
        double low_limit = current;

        while (high_limit - low_limit > 1e-7) {
            double middle_limit = (low_limit + high_limit) / 2;
            double newVol = 100000L * 100000L * middle_limit;
            for (int i = 0; i < n_holes; ++i) {
                if (holes[i].z + holes[i].r < middle_limit){
                    newVol -= find_volume(holes[i]);
                }
                else if (holes[i].z - holes[i].r <= middle_limit) {
                    double limit_position = middle_limit - holes[i].z;
                    double holeVol = holes[i].r * holes[i].r * limit_position - limit_position * limit_position * limit_position / 3.0 + 2.0 / 3 * holes[i].r * holes[i].r * holes[i].r;
                    holeVol *= M_PI;
                    newVol -= holeVol;
                }
            }
            double volumeFromLeft = newVol;

            if (volumeFromLeft > i * slice){
             	high_limit = middle_limit;
            } else {
    			low_limit = middle_limit;
            }
        }
        double result = (low_limit + high_limit) / 2;

        printf("%.9f\n", (result - current) / 1000.0);
        current = result;
    }
    printf("%.9f\n", (100000.0 - current) / 1000.0);
}

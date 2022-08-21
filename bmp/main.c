#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "transformations.h"

int main() {
    FILE* BMP = fopen("./assets/denzil.bmp", "rb");

    struct bmp_image* image = read_bmp(BMP);

    fclose(BMP);

    FILE* BMP_OUT = fopen("output.bmp", "wb");

    struct bmp_image* temp_image = flip_horizontally(image);
    write_bmp(BMP_OUT, temp_image);
    free_bmp_image(temp_image);

    temp_image = flip_vertically(image);
    write_bmp(BMP_OUT, temp_image);
    free_bmp_image(temp_image);

    temp_image = rotate_right(image);
    write_bmp(BMP_OUT, temp_image);
    free_bmp_image(temp_image);

    temp_image = rotate_left(image);
    write_bmp(BMP_OUT, temp_image);
    free_bmp_image(temp_image);

    const char* colors = { "rrggbb" };
    temp_image = extract(image, colors);
    write_bmp(BMP_OUT, temp_image);
    free_bmp_image(temp_image);

    float factor = 1.7;
    temp_image = scale(image, factor);
    write_bmp(BMP_OUT, temp_image);
    free_bmp_image(temp_image);

    uint32_t settings_for_crop[4] = { 1, 3, 20, 40 };
    temp_image = crop(image, settings_for_crop[0], settings_for_crop[1], settings_for_crop[2], settings_for_crop[3]);
    write_bmp(BMP_OUT, temp_image);
    free_bmp_image(temp_image);

    free_bmp_image(image);
    fclose(BMP_OUT);
    return 0;
}

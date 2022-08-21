#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

struct bmp_header* read_bmp_header(FILE* stream)
{
    if (stream == NULL) {
        return NULL;
    }

    struct bmp_header* bmp_head = (struct bmp_header*)malloc(sizeof(struct bmp_header));
    fseek(stream, 0, SEEK_SET);
    fread(bmp_head, sizeof(struct bmp_header), 1, stream);

    if ((bmp_head->type != 0x4d42) || (bmp_head == NULL)) {
        free(bmp_head);
        return NULL;
    }

    return bmp_head;
}

struct pixel* read_data(FILE* stream, const struct bmp_header* header)
{
    if (stream == NULL || header == NULL) {
        return NULL;
    }

    const int pixels_amount = header->height * header->width;
    int padd = (4 - (header->width * 3) % 4) % 4;

    struct pixel* colors = malloc(pixels_amount * sizeof(struct pixel));

    fseek(stream, sizeof(struct bmp_header), SEEK_SET);
    int index_p = 0;   // pixel index for colors
    for (int i = 0; i < header->height; i++) {
        for (int j = 0; j < header->width; j++) {
            fread(&colors[index_p], sizeof(struct pixel), 1, stream);
            index_p++;
        }
        fseek(stream, padd, SEEK_CUR);
    }
    return colors;
}

struct bmp_image* read_bmp(FILE* stream) {
    if (stream == NULL) {
        return NULL;
    }

    struct bmp_image* image = malloc(sizeof(struct bmp_image));

    image->header = read_bmp_header(stream);
    if (image->header == NULL) {
        fprintf(stderr, "Error: This is not a BMP file.\n");
        free_bmp_image(image);
        return NULL;
    }

    image->data = read_data(stream, image->header);
    if (image->data == NULL) {
        fprintf(stderr, "Error: This is not a BMP file.\n");
        free_bmp_image(image);
        return NULL;
    }

    return image;
}

bool write_bmp(FILE* stream, const struct bmp_image* image)
{
    if (!stream || image == NULL || image->header == NULL || image->data == NULL) {
        return 0;
    }

    fwrite(image->header, sizeof(struct bmp_header), 1, stream);

    int padd = (4 - (image->header->width * 3) % 4) % 4;
    int index_p = 0;
    for (int i = 0; i < image->header->height; i++) {
        for (int j = 0; j < image->header->width; j++) {
            fwrite(&image->data[index_p], sizeof(struct pixel), 1, stream);
            index_p++;
        }
        for (int k = 0; k < padd; ++k) {
            putc('0', stream);
        }
    }

    return 1;
}

void free_bmp_image(struct bmp_image* image) {
    if (!image) {
        return;
    }
    if (image->header) {
        free(image->header);
    }
    if (image->data) {
        free(image->data);
    }
    free(image);
}

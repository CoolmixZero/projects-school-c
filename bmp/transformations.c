#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "transformations.h"

struct bmp_image* flip_horizontally(const struct bmp_image* image) {
    if (image == NULL) {
        return NULL;
    }

    struct bmp_image* transformed_image = (struct bmp_image*)malloc(sizeof(struct bmp_image));

    transformed_image->header = malloc(sizeof(struct bmp_header));
    memcpy(transformed_image->header, image->header, sizeof(struct bmp_header));

    transformed_image->data = malloc(sizeof(struct pixel) * transformed_image->header->height * transformed_image->header->width);
    memcpy(transformed_image->data, image->data, sizeof(struct pixel) * transformed_image->header->height * transformed_image->header->width);

    for (size_t h = 0; h < image->header->height; ++h) {
        for (size_t w = 0; w < image->header->width; ++w) {
            transformed_image->data[h * transformed_image->header->width + w] = image->data[h * image->header->width + image->header->width - w - 1];
        }
    }

    return transformed_image;
}

struct bmp_image* flip_vertically(const struct bmp_image* image) {
    if (image == NULL) {
        return NULL;
    }

    struct bmp_image* transformed_image = (struct bmp_image*)malloc(sizeof(struct bmp_image));

    transformed_image->header = malloc(sizeof(struct bmp_header));
    memcpy(transformed_image->header, image->header, sizeof(struct bmp_header));

    transformed_image->data = malloc(sizeof(struct pixel) * transformed_image->header->height * transformed_image->header->width);
    memcpy(transformed_image->data, image->data, sizeof(struct pixel) * transformed_image->header->height * transformed_image->header->width);

    for (size_t h = 0; h < image->header->height; ++h) {
        for (size_t w = 0; w < image->header->width; ++w) {
            transformed_image->data[h * transformed_image->header->width + w] = image->data[(image->header->height - 1 - h) * image->header->width + w];
        }
    }

    return transformed_image;
}

struct bmp_image* rotate_right(const struct bmp_image* image) {
    if (image == NULL) {
        return NULL;
    }

    struct bmp_image* transformed_image = (struct bmp_image*)malloc(sizeof(struct bmp_image));

    transformed_image->header = malloc(sizeof(struct bmp_header));
    memcpy(transformed_image->header, image->header, sizeof(struct bmp_header));

    transformed_image->data = malloc((sizeof(struct pixel) * transformed_image->header->width + ((4 - (transformed_image->header->width * 3) % 4) % 4)) * transformed_image->header->height);
    memcpy(transformed_image->data, image->data, sizeof(struct pixel) * transformed_image->header->height * transformed_image->header->width);

    transformed_image->header->width = image->header->height;
    transformed_image->header->height = image->header->width;


    for (size_t h = 0; h < transformed_image->header->height; ++h) {
        for (size_t w = 0; w < transformed_image->header->width; ++w) {
            transformed_image->data[(h * transformed_image->header->width) + transformed_image->header->width - w - 1] = image->data[(w * transformed_image->header->height) + h];
        }
    }

    return transformed_image;
}

struct bmp_image* rotate_left(const struct bmp_image* image) {
    if (image == NULL) {
        return NULL;
    }

    struct bmp_image* transformed_image = (struct bmp_image*)malloc(sizeof(struct bmp_image));

    transformed_image->header = malloc(sizeof(struct bmp_header));
    memcpy(transformed_image->header, image->header, sizeof(struct bmp_header));

    transformed_image->data = malloc((sizeof(struct pixel) * transformed_image->header->width + ((4 - (transformed_image->header->width * 3) % 4) % 4)) * transformed_image->header->height);
    memcpy(transformed_image->data, image->data, sizeof(struct pixel) * transformed_image->header->height * transformed_image->header->width);

    transformed_image->header->width = image->header->height;
    transformed_image->header->height = image->header->width;


    for (size_t h = 0; h < transformed_image->header->height; ++h) {
        for (size_t w = 0; w < transformed_image->header->width; ++w) {
            transformed_image->data[(h * transformed_image->header->width) + w] = image->data[(w * transformed_image->header->height) + transformed_image->header->height - h - 1];
        }
    }

    return transformed_image;
}

struct bmp_image* crop(const struct bmp_image* image, const uint32_t start_y, const uint32_t start_x, const uint32_t height, const uint32_t width) {
    if (image == NULL || image->data == NULL || image->header == NULL || start_x < 0 || start_x > image->header->width || start_y < 0 || start_y > image->header->height) {
        return NULL;
    }

    struct bmp_image* transformed_image = (struct bmp_image*)malloc(sizeof(struct bmp_image));

    transformed_image->data = malloc((height * width) * sizeof(struct pixel));
    memcpy(transformed_image->data, image->data, sizeof(struct pixel) * (height * width));

    transformed_image->header = malloc(sizeof(struct bmp_header));
    memcpy(transformed_image->header, image->header, sizeof(struct bmp_header));

    transformed_image->header->height = height;
    transformed_image->header->width = width;

    for (size_t h = 0; h < height; ++h)
    {
        for (size_t w = 0; w < width; ++w)
        {
            transformed_image->data[(h * width) + w] = image->data[(h * image->header->width) + w];
        }
    }
    return transformed_image;
}

struct bmp_image* scale(const struct bmp_image* image, float factor) {
    if (image == NULL || image->header == NULL || image->data == NULL || factor <= 0) {
        return NULL;
    }

    struct bmp_image* transformed_image = (struct bmp_image*)malloc(sizeof(struct bmp_image));

    transformed_image->header = malloc(sizeof(struct bmp_header));
    memcpy(transformed_image->header, image->header, sizeof(struct bmp_header));

    uint32_t height = image->header->height * factor;
    uint32_t width = image->header->width * factor;

    transformed_image->data = malloc((height * width) * sizeof(struct pixel));
    memcpy(transformed_image->data, image->data, sizeof(struct pixel) * (height * width));

    transformed_image->header->width = width;
    transformed_image->header->height = height;

    for (size_t h = 0; h < height; ++h)
    {
        for (size_t w = 0; w < width; ++w)
        {
            transformed_image->data[(h * width) + w] = image->data[(int)((floor(h / factor)) * image->header->width + (floor(w / factor)))];
        }
    }
    height = round(image->header->height * factor);
    width = round(image->header->width * factor);
    
    return transformed_image;

}

struct bmp_image* extract(const struct bmp_image* image, const char* colors_to_keep) {
    if (image == NULL || colors_to_keep == NULL || image->data == NULL) {
        return NULL;
    }

    bool red = 0;
    bool green = 0;
    bool blue = 0;

    for (size_t color = 0; color < strlen(colors_to_keep); ++color)
    {
        if (colors_to_keep[color] == 'r') {
            red = 1;
            continue;
        }
        if (colors_to_keep[color] == 'g') {
            green = 1;
            continue;
        }
        if (colors_to_keep[color] == 'b') {
            blue = 1;
            continue;
        }
        return NULL;
    }


    struct bmp_image* transformed_image = (struct bmp_image*)malloc(sizeof(struct bmp_image));

    transformed_image->header = malloc(sizeof(struct bmp_header));
    memcpy(transformed_image->header, image->header, sizeof(struct bmp_header));

    transformed_image->data = malloc(sizeof(struct pixel) * transformed_image->header->height * transformed_image->header->width);
    memcpy(transformed_image->data, image->data, sizeof(struct pixel) * transformed_image->header->height * transformed_image->header->width);

    for (size_t h = 0; h < transformed_image->header->height; ++h) {
        for (size_t w = 0; w < transformed_image->header->width; ++w) {
            if (blue) {
                transformed_image->data[(w * image->header->height) + h].blue = image->data[(w * image->header->height) + h].blue;
            }
            else {
                transformed_image->data[(w * image->header->height) + h].blue = 0;
            }
            if (green) {
                transformed_image->data[(w * image->header->height) + h].green = image->data[(w * image->header->height) + h].green;
            }
            else {
                transformed_image->data[(w * image->header->height) + h].green = 0;
            }
            if (red) {
                transformed_image->data[(w * image->header->height) + h].red = image->data[(w * image->header->height) + h].red;
            }
            else {
                transformed_image->data[(w * image->header->height) + h].red = 0;
            }
        }
    }

    return transformed_image;
}

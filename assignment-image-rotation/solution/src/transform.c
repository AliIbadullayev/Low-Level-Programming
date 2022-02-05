//
// Created by Alibaba Ibadullaev on 26.12.2021.
//
#include <stdio.h>
#include <stdlib.h>

#include "transform.h"

static size_t image_area(int32_t width, int32_t height){
    return sizeof (uint8_t) * width * height * 3;
}

struct image rotate(struct image* img){
    if (!img) return (struct image) {0};
    //Here I create a temporary struct image to copy in it a rotated image
    struct image temp = {0};
    size_t area = image_area(img -> width, img -> height);
    temp.data = malloc(area);
    if (!temp.data){
        fprintf(stderr, "Cannot allocate memory by transform!");
        return (struct image) {0};
    }

    //Here I change a width and height of image
    temp.width = img -> height;
    temp.height = img -> width;

    int64_t counter = 0; // counter for area with padding
    int64_t x_axis  = 0; // pixel in x coordinate
    int64_t y_axis = (temp.width - 1) * temp.height; // for counter of height of image

    while (counter < area){
        if (x_axis == temp.width ) {
            x_axis = 0;
            y_axis++;
            continue;
        }
        for (int pixel = 0; pixel < 3; pixel ++){ //this code is for output 3 color of pixels (g(reen) b(lue) r(ed)) and for writing the color of pixels to struct image
            temp.data[counter/3].pointer[pixel] = img -> data [y_axis - (x_axis * temp.height)].pointer[pixel];
        }
        counter = counter + 3;
        x_axis ++;
    }

    return temp;
}

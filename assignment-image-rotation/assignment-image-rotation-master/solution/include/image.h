//<<Header file of image cleared from their header (bmh_header)>>
//
// Created by Alibaba Ibadullaev on 25.12.2021.
//

#ifndef ASSIGNMENT_IMAGE_ROTATION_MASTER_IMAGE_H
#define ASSIGNMENT_IMAGE_ROTATION_MASTER_IMAGE_H
#include <inttypes.h>

struct pixel {
uint8_t pointer[3]; // pointer to blue green red colors inside pixels
};

struct image{
    uint64_t width, height;
    struct pixel* data;
};

#endif //ASSIGNMENT_IMAGE_ROTATION_MASTER_IMAGE_H

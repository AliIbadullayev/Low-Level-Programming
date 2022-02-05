//<<Header file for bmp header>>
//
// Created by Alibaba Ibadullaev on 25.12.2021.
//

#ifndef ASSIGNMENT_IMAGE_ROTATION_MASTER_BMPHEADER_H
#define ASSIGNMENT_IMAGE_ROTATION_MASTER_BMPHEADER_H

#include <inttypes.h>

struct bmp_header
{
    uint16_t bfType;
    uint32_t  bfileSize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t  biHeight;
    uint16_t  biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t  biClrImportant;
}__attribute__((packed));


#endif //ASSIGNMENT_IMAGE_ROTATION_MASTER_BMPHEADER_H

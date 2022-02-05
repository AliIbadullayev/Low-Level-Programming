// <<Header file for read write open and close file>>
//
// Created by Alibaba Ibadullaev on 25.12.2021.
//

#ifndef ASSIGNMENT_IMAGE_ROTATION_MASTER_RWOCFILE_H
#define ASSIGNMENT_IMAGE_ROTATION_MASTER_RWOCFILE_H

#include <stdio.h>
#include "image.h"

enum read_status  {
    READ_OK,
    READ_INVALID_SIGNATURE,
    READ_INVALID_BITS,
    READ_INVALID_HEADER,
    READ_INVALID_POINTER
};



enum read_status from_bmp( FILE* in, struct image* img );

/*  serializer   */
enum  write_status  {
    WRITE_OK,
    WRITE_ERROR
};

enum write_status to_bmp( FILE* out, struct image const* img );

enum open_status {
    OPEN_OK,
    OPEN_ERROR
};

enum open_status open_file( FILE** in, char* file_name, char* rights);

enum close_status {
    CLOSE_OK,
    CLOSE_ERROR
};

enum close_status close_file( FILE* f );

void image_destroy(struct image* img);

#endif //ASSIGNMENT_IMAGE_ROTATION_MASTER_RWOCFILE_H

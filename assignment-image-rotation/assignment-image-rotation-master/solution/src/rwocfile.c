#include <stdbool.h>
#include <stdlib.h>


#include "rwocfile.h"
#include "bmpheader.h"
// Header file for Read Write Open and Close file
//
// Created by Alibaba Ibadullaev on 25.12.2021.
//
static const uint16_t bfType = 19778;
static const uint32_t bOffBits = 54;
static const uint32_t biSize = 40;
static const uint32_t bfReserved = 0;
static const uint16_t biPlanes = 1 ;
static const uint16_t biBitCount = 24;
static const uint32_t biCompression = 0;
static const uint32_t biXPelsPerMeter = 2834;
static const uint32_t biYPelsPerMeter = 2834;


enum open_status open_file(FILE** in, char* file_name, char* rights){
    *in = fopen(file_name, rights);
    return *in ? OPEN_OK : OPEN_ERROR;
}

enum close_status close_file(FILE* f){
    return !f ? CLOSE_OK : fclose(f) == EOF ? CLOSE_ERROR : CLOSE_OK;
}

inline static size_t read_header( FILE* f, struct bmp_header* header ) {
    return fread( header, sizeof( struct bmp_header ), 1, f );
}

static uint8_t extra_padding(int32_t len){
    if (len % 4 == 0) return 0;
    else {
        return 4 - ((3) * (len) % 4);
    }
}

//This is formula for size with padding * 3 (I * 3 because we have 3 color)
static size_t area_with_padding(int32_t width, int32_t height){
    return (extra_padding(width ) + (3 * width) ) * (height);
}

//This is formula for size without padding * 3 (I * 3 because we have 3 color)
static size_t area(int32_t width, int32_t height){
    return 3 * sizeof(uint8_t) * (height) * (width);
}

enum read_status from_bmp( FILE* in, struct image* img ){
    if (!in || !img) {
        return READ_INVALID_POINTER;
    }
    //Here I read header of BMP file and forget him
    struct bmp_header header = {0};

    if( read_header(in, &header) == 0 && ferror(in)){
        return READ_INVALID_HEADER;
    } else if (feof(in)) {
        return READ_INVALID_SIGNATURE;
    } else {
        //Here I write cleared from BMP-header file in struct image
        img -> width = header.biWidth;
        img -> height = header.biHeight;

        //Cleared area is area without padding (3->count of pixel * width * height)
        size_t cleared_area = area(img -> width, img -> height);
        //Allocate mem for struct image || unsigned char - pixels color size
        img -> data = malloc (cleared_area*sizeof(struct pixel));
        if (!img -> data){
            fprintf(stderr, "Error by allocation of memory to an image!");
            return false;
        }

        //Make buffer to write init a cleared from BMP-header image file (but with paddings )
        const size_t area_padding = area_with_padding(img -> width, img -> height);
        uint8_t buffer[area_padding];

        if( fread(buffer, sizeof(buffer), 1, in) == 0 && ferror(in)){
            image_destroy(img);
            return READ_INVALID_BITS;
        } else if (feof(in)) {
            image_destroy(img);
            return READ_INVALID_SIGNATURE;
        } else {
            int counter = 0; // counter for area with padding
            int x_axis  = 0; // pixel in x coordinate
            int y_axis = 0; // for counter of height of image

            while (counter < area_padding){
                if (x_axis  == img -> width ) {
                    x_axis = 0;
                    counter = counter + extra_padding(img -> width);
                    y_axis++;
                    continue;
                }
                for (int pixel = 0; pixel < 3; pixel ++){ //this code is for output 3 color of pixels (g(reen) b(lue) r(ed)) and for writing the color of pixels to struct image
                    img -> data [x_axis + ( y_axis * (img->width))] .pointer[pixel] = buffer[counter+pixel];
                }
                counter = counter + 3;
                x_axis ++;
            }
            return READ_OK;
        }
    }
}

void generate_header(struct bmp_header* header,  int32_t width, int32_t height){
    header->bfType = bfType;
    header->bfileSize = area_with_padding(width, height) + 54;
    header->bfReserved = bfReserved;
    header->bOffBits = bOffBits;
    header->biSize = biSize;
    header->biWidth = width;
    header->biHeight = height;
    header->biPlanes = biPlanes;
    header->biBitCount = biBitCount;
    header->biCompression = biCompression;
    header->biSizeImage = area_with_padding(width, height);
    header->biXPelsPerMeter = biXPelsPerMeter;
    header->biYPelsPerMeter = biYPelsPerMeter;
}

enum write_status to_bmp(FILE* out, const struct image* img){
    if (!out || !img) {
        return WRITE_ERROR;
    }
    else {
        fprintf(stdout, "File is ready to write init.\n");
        struct bmp_header header = {0};
        generate_header(&header, img->width, img->height);
        int8_t buffer [area_with_padding(img -> width, img -> height)];

        int counter = 0; // counter for area with padding
        int x_axis  = 0; // pixel in x coordinate
        int y_axis = 0; // for counter of height of image
        uint8_t padding = extra_padding(img->width);

        while (counter < area_with_padding(img -> width, img -> height)){
            if (x_axis  == img -> width ) {
                x_axis = 0;
                //Here I fill paddings at the end of each row
                for (int curr_padding = 0; curr_padding < padding; curr_padding++){
                    buffer[counter+curr_padding] = 0;
                }
                counter = counter + extra_padding(img -> width);
                y_axis++;
                continue;
            }
            for (int pixel = 0; pixel < 3; pixel ++){ //this code is for output 3 color of pixels (g(reen) b(lue) r(ed)) and for writing the color of pixels to struct image
                buffer[counter+pixel] = img -> data [x_axis + ( y_axis * (img->width))] .pointer[pixel];
            }
            counter = counter + 3;
            x_axis ++;
        }


        if(fwrite (&header, sizeof(struct bmp_header), 1, out) != 1){
            return WRITE_ERROR;
        }
        if(fwrite (buffer, sizeof(buffer), 1, out) != 1){
            return WRITE_ERROR;
        }

        return WRITE_OK;
    }
}

void image_destroy(struct image* img) {
    if (!img) return;
    free(img->data);
}





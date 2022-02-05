#include <stdio.h>

#include "rwocfile.h"
#include "transform.h"

int main( int argc, char** argv ) {
    if (argc < 3) printf("Not enough arguments \n" );

    struct image img = {0};
    FILE *f_read ;
    enum open_status open_status_in = open_file(&f_read, argv[1], "rb");
    if (open_status_in == OPEN_ERROR){
         fprintf(stderr, "Input file is not opened!");
         return -41;
    }

    //Here I open source BMP file and write it to struct image
    enum read_status read_status =  from_bmp(f_read, &img);
    if (read_status == READ_INVALID_SIGNATURE){
        fprintf(stderr, "EOF error occurs!");
        close_file(f_read);
        return -43;
    }
    if (read_status == READ_INVALID_HEADER){
        fprintf(stderr, "You have some troubles with reading of BMP header");
        close_file(f_read);
        return -43;
    }
    if (read_status == READ_INVALID_BITS){
        fprintf(stderr, "You have some troubles with reading of image struct!");
        close_file(f_read);
        return -43;
    }
    if (read_status == READ_INVALID_POINTER){
        fprintf(stderr, "You have invalid pointer for reading from file!");
        close_file(f_read);
        return -43;
    }
    enum close_status close_status_in = close_file(f_read);
    if(close_status_in == CLOSE_ERROR) {
        fprintf(stderr, "Cannot close file!\n");
        return -42;
    }
    
    FILE * f_write;
    enum open_status open_status_out = open_file(&f_write, argv[2], "wb");
    if (open_status_out == OPEN_ERROR){
        fprintf(stderr, "Output file is not opened!");
        return -41;
    }

    //Here I rotate my image
    struct image img_changed ={0};
    img_changed = rotate(&img);

    //Here I write my struct image in destination file and destroy this struct after that
    enum write_status write_status = to_bmp(f_write, &img_changed);
    if (write_status == WRITE_ERROR){
        fprintf(stderr, "You have some troubles with writing into file!");
        close_file(f_write);
        return  -43;
    }

    image_destroy(&img);
    image_destroy(&img_changed);

    enum close_status close_status_out = close_file(f_write);
    if(close_status_out == CLOSE_ERROR) {
        fprintf(stderr, "Cannot close file!\n");
        return -42;
    }
    return 0;
}

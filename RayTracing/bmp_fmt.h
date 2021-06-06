#ifndef _BMP_FMT_H_
#define _BMP_FMT_H_

#include <stdio.h>

typedef struct __attribute__((packed)) {
   unsigned char type[2];                   /* Magic identifier            */
   unsigned int size;                       /* File size in bytes          */
   unsigned short int reserved1, reserved2;
   unsigned int offset;                     /* Offset to image data, bytes */

   unsigned int info_size;               /* Header size in bytes      */
   int width,height;                /* Width and height of image */
   unsigned short int planes;       /* Number of colour planes   */
   unsigned short int bits;         /* Bits per pixel            */
   unsigned int compression;        /* Compression type          */
   unsigned int imagesize;          /* Image size in bytes       */
   int xresolution,yresolution;     /* Pixels per meter          */
   unsigned int ncolours;           /* Number of colours         */
   unsigned int importantcolours;   /* Important colours         */
} bmp_header;

inline bmp_header* gen_bmp_header(int width, int height)
{
    bmp_header* header = (bmp_header*)calloc(sizeof(bmp_header), 1);
    header->type[0] = 'B'; header->type[1] = 'M';
    header->size = 54 + (width*height)*3;
    header->offset = 54;

    header->info_size = 40;
    header->width = width;
    header->height = height;
    header->planes = 1;
    header->bits = 24;

    return header;
}

#endif
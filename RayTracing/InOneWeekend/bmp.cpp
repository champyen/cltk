#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#include "cltkpp.h"
#include "bmp_fmt.h"

void bmp_example(void)
{
    // Image
    int image_width = 256;
    int image_height = 256;

    // Render
    cltk::context ctx;
    shared_ptr<cltk::buffer> buf = ctx.allocBuffer(image_width*image_width*3);
    shared_ptr<cltk::library> lib = ctx.getLibrary("bmp.cl", "", "");
    shared_ptr<cltk::function> func = lib->getFunction("bmp");

    size_t gsize[3] = {image_width, image_height, 1 };

    func->NDR(2, gsize, NULL).exec(buf);
    {
        int* hostbuf = (int*)buf->map();

        bmp_header *header = gen_bmp_header(image_width, image_height);
        FILE *fptr = fopen("bmp.bmp", "wb");
        fwrite(header, sizeof(bmp_header), 1, fptr);
        fwrite(hostbuf, 3, image_width*image_height, fptr);
        fclose(fptr);

        free(header);
        buf->unmap();
    }

}

int main(void)
{
    bmp_example();

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#include "cltkpp.h"
#include "vec3.h"
#include "bmp_fmt.h"

void ray_example(void)
{
    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = static_cast<int>(image_width / aspect_ratio);;

    // Render
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    cltk::context ctx;
    shared_ptr<cltk::buffer> buf = ctx.allocBuffer(image_width*image_width*3);
    shared_ptr<cltk::library> lib = ctx.getLibrary("ray.cl", "", "");
    shared_ptr<cltk::function> func = lib->getFunction("ray_test");

    size_t gsize[3] = {image_width, image_height, 1 };

    func->NDR(2, gsize, NULL).exec(buf, origin, horizontal, vertical, lower_left_corner);
    {
        int* hostbuf = (int*)buf->map();

        bmp_header *header = gen_bmp_header(image_width, image_height);
        FILE *fptr = fopen("ray.bmp", "wb");
        fwrite(header, sizeof(bmp_header), 1, fptr);
        fwrite(hostbuf, 3, image_width*image_height, fptr);
        fclose(fptr);

        free(header);
        buf->unmap();
    }

}

int main(void)
{
    ray_example();

    return 0;
}
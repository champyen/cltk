#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>

#include "cltkpp.h"
#include "bmp_fmt.h"
#include "vec3.h"

#define MAX_OBJS    1024
typedef struct sphere
{
    sphere(cl_float3 c, cl_float r) : center(c), radius(r) {}
    ~sphere(){}
    cl_float3 center;
    cl_float radius;
};

typedef struct viewinfo
{
    viewinfo(float ar, float vh, float fl)
    {
        aspect_ratio = ar;
        viewport_height = vh;
        viewport_width = aspect_ratio * viewport_height;
        focal_length = fl;
    }
    ~viewinfo() {}

    float aspect_ratio;
    float viewport_height;
    float viewport_width;
    float focal_length;
};

typedef struct camera
{
    camera(viewinfo &vi, point3 orig)
    {
        origin = orig.e;
        auto h = vec3(vi.viewport_width, 0, 0);
        auto v = vec3(0, vi.viewport_height, 0);
        auto llc = orig - h/2 - v/2 - vec3(0, 0, vi.focal_length);
        horizontal = h.e;
        vertical = v.e;
        lower_left_corner = llc.e;
    }
    ~camera() {}

    cl_float3 origin;
    cl_float3 lower_left_corner;
    cl_float3 horizontal;
    cl_float3 vertical;
};

void diffuse_example(void)
{
    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = static_cast<int>(image_width / aspect_ratio);;

    cltk::context ctx;
    shared_ptr<cltk::buffer> buf = ctx.allocBuffer(image_width*image_width*3);
    shared_ptr<cltk::buffer> sphere_list = ctx.allocBuffer(sizeof(sphere)*MAX_OBJS);
    shared_ptr<cltk::library> lib = ctx.getLibrary("diffuse.cl", "", "");
    shared_ptr<cltk::function> func = lib->getFunction("diffuse");

    // Camera
    viewinfo vi(aspect_ratio, 2.0, 1.0);
    camera cam(vi, point3(0, 0, 0));

    // World
    int num_sphere = 2;
    sphere *splist = (sphere *)sphere_list->map();
    splist[0] = sphere(cl_float3{0, 0, -1}, 0.5f);
    splist[1] = sphere(cl_float3{0,-100.5,-1}, 100.f);
    sphere_list->unmap();

    srandom(time(NULL));
    unsigned long seed = random();

    // Render
    size_t gsize[3] = {image_width, image_height, 1 };
    func->NDR(2, gsize, NULL).exec(buf, sphere_list, num_sphere, cam, seed);

    {
        int* hostbuf = (int*)buf->map();

        bmp_header *header = gen_bmp_header(image_width, image_height);
        FILE *fptr = fopen("diffuse.bmp", "wb");
        fwrite(header, sizeof(bmp_header), 1, fptr);
        fwrite(hostbuf, 3, image_width*image_height, fptr);
        fclose(fptr);

        free(header);
        buf->unmap();
    }

}

int main(void)
{
    diffuse_example();

    return 0;
}
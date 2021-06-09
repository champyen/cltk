#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>

#include "cltkpp.h"
#include "bmp_fmt.h"
#include "vec3.h"

#define MAX_OBJS    256

enum
{
    RT_M_METAL = 0,
    RT_M_LAMBERTIAN
} RT_M;

typedef struct sphere
{
    sphere(cl_float3 c, cl_float r, cl_float3 a, cl_uint m, cl_float f) : center(c), radius(r), albedo(a), material(m), fuzz(f < 1 ? f : 1){}
    ~sphere(){}
    cl_float3 center;
    cl_float radius;
    cl_float3 albedo;
    cl_float fuzz;
    cl_uint  material;
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

void material_example(void)
{
    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = static_cast<int>(image_width / aspect_ratio);;

    cltk::context ctx;
    shared_ptr<cltk::buffer> buf = ctx.allocBuffer(image_width*image_width*3);
    shared_ptr<cltk::buffer> sphere_list = ctx.allocBuffer(sizeof(sphere)*MAX_OBJS);
    shared_ptr<cltk::library> lib = ctx.getLibrary("material.cl", "", "");
    shared_ptr<cltk::function> func = lib->getFunction("material");

    // Camera
    viewinfo vi(aspect_ratio, 2.0, 1.0);
    camera cam(vi, point3(0, 0, 0));

    // World
    int num_sphere = 0;
    sphere *splist = (sphere *)sphere_list->map();
    splist[num_sphere++] = sphere(cl_float3{0,-100.5,-1}, 100.f, cl_float3{0.8, 0.8, 0.0}, RT_M_LAMBERTIAN, 0);
    splist[num_sphere++] = sphere(cl_float3{ 0.0, 0.0,-1.0}, 0.5f, cl_float3{0.7, 0.3, 0.3}, RT_M_LAMBERTIAN, 0);
    splist[num_sphere++] = sphere(cl_float3{-1.0, 0.0,-1.0}, 0.5f, cl_float3{0.8, 0.8, 0.8}, RT_M_METAL, 0.3);
    splist[num_sphere++] = sphere(cl_float3{ 1.0, 0.0,-1.0}, 0.5f, cl_float3{0.8, 0.6, 0.2}, RT_M_METAL, 1.0);
    sphere_list->unmap();

    srandom(time(NULL));
    unsigned long seed = random();

    // Render
    size_t gsize[3] = {image_width, image_height, 1 };
    func->NDR(2, gsize, NULL).exec(buf, sphere_list, num_sphere, cam, seed);

    {
        int* hostbuf = (int*)buf->map();

        bmp_header *header = gen_bmp_header(image_width, image_height);
        FILE *fptr = fopen("material.bmp", "wb");
        fwrite(header, sizeof(bmp_header), 1, fptr);
        fwrite(hostbuf, 3, image_width*image_height, fptr);
        fclose(fptr);

        free(header);
        buf->unmap();
    }

}

int main(void)
{
    material_example();

    return 0;
}
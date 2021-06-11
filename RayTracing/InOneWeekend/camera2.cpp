#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>

#include "cltkpp.h"
#include "bmp_fmt.h"
#include "vec3.h"

#define MAX_OBJS    256

#include "material.h"

typedef struct sphere
{
    sphere(cl_float3 c, cl_float r, cl_float3 a, cl_uint m, cl_float f) : center(c), radius(r), albedo(a), material(m), fuzz(f){}
    ~sphere(){}
    cl_float3 center;
    cl_float radius;
    cl_float3 albedo;
    union{
        cl_float fuzz;
        cl_float ir;
    };
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
    camera(
        point3 lookfrom,
        point3 lookat,
        vec3   vup,
        float vfov, // vertical field-of-view in degrees
        float aspect_ratio,
        float aperture,
        float focus_dist
    ){
        float theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        auto w = unit_vector(lookfrom - lookat);
        auto u = unit_vector(cross(vup, w));
        auto v = cross(w, u);
        _w = w.e; _u = u.e; _v = v.e;

        origin = lookfrom.e;
        vec3 hori = focus_dist * viewport_width * u;
        horizontal = hori.e;
        vec3 vert = focus_dist * viewport_height * v;
        vertical = vert.e;
        lower_left_corner = (lookfrom - hori/2 - vert/2 - focus_dist*w).e;

        lens_radius = aperture / 2;
    }
    ~camera() {}

    cl_float3 origin;
    cl_float3 lower_left_corner;
    cl_float3 horizontal;
    cl_float3 vertical;
    cl_float3 _u, _v, _w;
    cl_float lens_radius;
};

void camera_example(void)
{
    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = static_cast<int>(image_width / aspect_ratio);;

    cltk::context ctx;
    shared_ptr<cltk::buffer> buf = ctx.allocBuffer(image_width*image_width*3);
    shared_ptr<cltk::buffer> sphere_list = ctx.allocBuffer(sizeof(sphere)*MAX_OBJS);
    shared_ptr<cltk::library> lib = ctx.getLibrary("camera2.cl", "", "");
    shared_ptr<cltk::function> func = lib->getFunction("camera2");

    // Camera
    point3 lookfrom(3,3,2);
    point3 lookat(0,0,-1);
    vec3 vup(0,1,0);
    auto dist_to_focus = (lookfrom-lookat).length();
    auto aperture = 2.0;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
    //camera cam(point3(-2,2,1), point3(0,0,-1), vec3(0,1,0), 20, aspect_ratio);

    // World
    int num_sphere = 0;
    sphere *splist = (sphere *)sphere_list->map();
    splist[num_sphere++] = sphere(cl_float3{0,-100.5,-1}, 100.f, cl_float3{0.8, 0.8, 0.0}, RT_M_LAMBERTIAN, 0);
    splist[num_sphere++] = sphere(cl_float3{ 0.0, 0.0,-1.0}, 0.5f, cl_float3{0.3, 0.3, 0.7}, RT_M_LAMBERTIAN, 0.0);
    splist[num_sphere++] = sphere(cl_float3{-1.0, 0.0,-1.0}, 0.5f, cl_float3{1.0, 1.0, 1.0}, RT_M_DIELECTRIC, 1.5);
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
        FILE *fptr = fopen("camera2.bmp", "wb");
        fwrite(header, sizeof(bmp_header), 1, fptr);
        fwrite(hostbuf, 3, image_width*image_height, fptr);
        fclose(fptr);

        free(header);
        buf->unmap();
    }

}

int main(void)
{
    camera_example();

    return 0;
}
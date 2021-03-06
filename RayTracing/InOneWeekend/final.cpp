#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>

#define CLTKCPP_DEBUG
#include "cltkpp.h"
#include "bmp_fmt.h"
#include "vec3.h"

#define MAX_OBJS    2048

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

void final_example(void)
{
    srand(time(NULL));
    // Image
    auto aspect_ratio = 3.0 / 2.0;
    int image_width = 600;
    int image_height = static_cast<int>(image_width / aspect_ratio);;

    cltk::context ctx;
    shared_ptr<cltk::buffer> buf = ctx.allocBuffer(image_width*image_width*3, "buffer");
    shared_ptr<cltk::buffer> sphere_list = ctx.allocBuffer(sizeof(sphere)*MAX_OBJS, "sphere list");
    shared_ptr<cltk::library> lib = ctx.getLibrary("final.cl", "", "");
    shared_ptr<cltk::function> func = lib->getFunction("final");

    // Camera
    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // World
    int num_sphere = 0;
    sphere *splist = (sphere *)sphere_list->map();
    splist[num_sphere++] = sphere(cl_float3{0,-1000,0}, 1000.f, cl_float3{0.5, 0.5, 0.5}, RT_M_LAMBERTIAN, 0);
    for (int a = -9; a < 9; a++) {
        for (int b = -9; b < 9; b++) {
            auto choose_mat = random_float();
            point3 center(a + 0.9*random_float(), 0.2, b + 0.9*random_float());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {

                if (choose_mat < 0.75) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    splist[num_sphere++] = sphere(center.e, 0.2, albedo.e, RT_M_LAMBERTIAN, 0);
                } else if (choose_mat < 0.9) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_float(0, 0.5);
                    splist[num_sphere++] = sphere(center.e, 0.2, albedo.e, RT_M_METAL, fuzz);
                } else {
                    // glass
                    auto albedo = color(1, 1, 1);
                    splist[num_sphere++] = sphere(center.e, 0.2, albedo.e, RT_M_DIELECTRIC, 1.5);
                }
            }
        }
    }

    splist[num_sphere++] = sphere(cl_float3{0, 1, 0}, 1.0, cl_float3{1, 1, 1}, RT_M_DIELECTRIC, 1.5);
    splist[num_sphere++] = sphere(cl_float3{-4, 1, 0}, 1.0, cl_float3{0.4, 0.2, 0.1}, RT_M_LAMBERTIAN, 0.f);
    splist[num_sphere++] = sphere(cl_float3{4, 1, 0}, 1.0, cl_float3{0.7, 0.6, 0.5}, RT_M_METAL, 0.f);

    sphere_list->unmap();

    // Render
    size_t gsize[3] = {image_width, image_height, 1 };
    int samples_per_pixel = 100;
    unsigned long seed = random();
    printf("samples per pixel - %d\n", samples_per_pixel);
    func->NDR(2, gsize, NULL).exec(buf, sphere_list, num_sphere, cam, samples_per_pixel, seed);
    {
        int* hostbuf = (int*)buf->map();

        bmp_header *header = gen_bmp_header(image_width, image_height);
        FILE *fptr = fopen("final.bmp", "wb");
        fwrite(header, sizeof(bmp_header), 1, fptr);
        fwrite(hostbuf, 3, image_width*image_height, fptr);
        fclose(fptr);

        free(header);
        buf->unmap();
    }

}

int main(void)
{
    final_example();

    return 0;
}

typedef struct
{
    float3 origin;
    float3 dir;
} ray;

float3 at(ray *r, float t)
{
    return r->origin + t*r->dir;
}

bool hit_sphere(float3 center, float radius, ray *r) {
    float3 oc = r->origin - center;
    float a = dot(r->dir, r->dir);
    float b = 2.0 * dot(oc, r->dir);
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}

float3 ray_color(ray *r) {
    if( hit_sphere((float3)(0.0, 0.0, -1.0), 0.5, r) )
        return (float3)(1.0, 0.0, 0.0);
    float3 unit_direction = normalize(r->dir);
    float t = 0.5*(unit_direction.y + 1.0);
    return (1.0f-t)*(float3)(1.0, 1.0, 1.0) + t*(float3)(0.5f, 0.7f, 1.0f);
}

__kernel void redsphere(
    __global uchar* buf,
    float3 origin,
    float3 horizontal,
    float3 vertical,
    float3 lower_left_corner
){
    int image_width = get_global_size(0);
    int image_height = get_global_size(1);

    int i = get_global_id(0);
    int j = get_global_id(1);

    int ofs = ((j * image_width) + i)*3;

    float u = (float)i / ((float)image_width-1.0);
    float v = (float)j / ((float)image_height-1.0);

    ray r;
    r.dir = lower_left_corner + u*horizontal + v*vertical - origin;
    r.origin = origin;

    float3 color = ray_color(&r);
    uchar3 u8vec3 = convert_uchar3(255.999f * color);

    buf[ofs] = u8vec3.s2;
    buf[ofs+1] = u8vec3.s1;
    buf[ofs+2] = u8vec3.s0;
}

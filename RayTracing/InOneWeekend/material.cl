//#define USE_ULONG_RAND

#ifdef USE_ULONG_RAND
#define STATE_TYPE  ulong
#else
#define STATE_TYPE  uint2
#endif

enum
{
    RT_M_METAL = 0,
    RT_M_LAMBERTIAN
} ;

// comes from https://cas.ee.ic.ac.uk/people/dt10/research/rngs-gpu-mwc64x.html
uint MWC64X(uint2* state)
{
    enum { A=4294883355U};
    uint x=(*state).x, c=(*state).y;  // Unpack the state
    uint res=x^c;                     // Calculate the result
    uint hi=mul_hi(x,A);              // Step the RNG
    x=x*A+c;
    c=hi+(x<c);
    *state=(uint2)(x,c);               // Pack the state back up
    return res;                       // Return the next result
}

float rand_float(STATE_TYPE* state)
{
#ifdef USE_ULONG_RAND
    // comes from https://stackoverflow.com/questions/9912143/how-to-get-a-random-number-in-opencl
    *state = (*state * 0x5DEECE66DL + 0xBL) & ((1L << 48) - 1);
    return (float)(*state >> 16)/(float)0xFFFFFFFF;
#else
    return (float)MWC64X(state)/(float)(0xFFFFFFFF);
#endif
}

float rand_float2(float min, float max, STATE_TYPE* state)
{
    // Returns a random real in [min,max).
    return min + (max-min)*rand_float(state);
}

float3 random_in_unit_sphere(STATE_TYPE* state)
{
    float r = 1.0;
    while (true) {
        float3 p = (float3)(
            rand_float2(-r, r, state),
            rand_float2(-r, r, state),
            rand_float2(-r, r, state)
        );
        if (dot(p, p) < r)
            return p;
    }
}

float3 random_unit_vector(STATE_TYPE* state)
{
    float r = 1.0;
    float3 p = (float3)(
        rand_float2(-r, r, state),
        rand_float2(-r, r, state),
        rand_float2(-r, r, state)
    );
    return normalize(p);
}

float3 random_in_hemisphere(float3 normal, STATE_TYPE* state) {
    float3 in_unit_sphere = random_in_unit_sphere(state);
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

typedef struct
{
    float3 origin;
    float3 dir;
} ray;

float3 at(ray *r, float t)
{
    return r->origin + t*r->dir;
}

typedef struct
{
    float3 origin;
    float3 lower_left_corner;
    float3 horizontal;
    float3 vertical;
} camera;

void get_ray(camera* cam, ray* r, float u, float v)
{
    r->origin = cam->origin;
    r->dir = cam->lower_left_corner + u*cam->horizontal + v*cam->vertical - cam->origin;
}


typedef struct
{
    float3 center;
    float radius;
    float3 albedo;
    float fuzz;
    uint  meterial;
} sphere;

typedef struct
{
    float3 p;
    float3 normal;
    float t;
    bool front_face;

    float3 albedo;
    float fuzz;
    uint meterial;
} hit_record;

void set_face_normal(hit_record* rec, ray* r, float3* outward_normal)
{
    rec->front_face = dot(r->dir, *outward_normal) < 0;
    rec->normal = rec->front_face ? *outward_normal :-*outward_normal;
}

bool near_zero(float3 v)  {
    // Return true if the vector is close to zero in all dimensions.
    float s = 1e-8;
    float3 absv = fabs(v);
    return (absv.s0 < s) && (absv.s1 < s) && (absv.s2 < s);
}

bool lambertian_scatter(ray* r_in, hit_record* rec, float3* attenuation, ray* scattered, STATE_TYPE* state)
{
    float3 scatter_direction = rec->normal + random_unit_vector(state);
    if (near_zero(scatter_direction))
        scatter_direction = rec->normal;
    scattered->origin = rec->p;
    scattered->dir = scatter_direction;
    *attenuation = rec->albedo;
    return true;
}

float3 reflect(float3 v, float3 n) {
    return v - 2*dot(v,n)*n;
}

bool metal_scatter(ray* r_in, hit_record* rec, float3* attenuation, ray* scattered, STATE_TYPE* state)
{
    float3 reflected = reflect(normalize(r_in->dir), rec->normal);
    scattered->origin =  rec->p;
    scattered->dir = reflected + rec->fuzz*random_in_unit_sphere(state);
    *attenuation = rec->albedo;
    return (dot(scattered->dir, rec->normal) > 0);
}

bool sphere_hit(sphere* sp, ray* r, float t_min, float t_max, hit_record* rec)
{
    float3 oc = r->origin - sp->center;
    float a = dot(r->dir, r->dir);
    float half_b = dot(oc, r->dir);
    float c = dot(oc,oc) - sp->radius*sp->radius;

    float discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    float sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    float root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec->t = root;
    rec->p = at(r, rec->t);
    float3 outward_normal = (rec->p - sp->center) / sp->radius;
    set_face_normal(rec, r, &outward_normal);
    rec->albedo = sp->albedo;
    rec->fuzz = sp->fuzz;
    rec->meterial = sp->meterial;

    return true;
}

bool world_hit(ray* r, __global sphere *spl, int num_obj, float t_min, float t_max, hit_record* rec)
{
    bool hit_anything = false;
    float closest_so_far = t_max;

    for(int i = 0; i < num_obj; i++) {
        sphere so = *(spl + i);
        if(sphere_hit(&so, r, t_min, closest_so_far, rec)) {
            hit_anything = true;
            closest_so_far = rec->t;
        }
    }

    return hit_anything;
}

float3 ray_color(ray *r, __global sphere *spl, int num_obj, STATE_TYPE* state, int depth)
{
    float3 ret = 1.0f;

    for(int d = depth; d >= 0; d--){
        hit_record rec;
        if(d == 0){
            ret = 0.0f;
            break;
        }
        if (world_hit(r, spl, num_obj, 0.001f, INFINITY, &rec)) {
            ray scattered;
            float3 attenuation = 1.0f;
            bool ret_scatter = false;
            switch(rec.meterial){
                case RT_M_METAL:
                    ret_scatter = metal_scatter(r, &rec, &attenuation, &scattered, state);
                    break;
                case RT_M_LAMBERTIAN:
                    ret_scatter = lambertian_scatter(r, &rec, &attenuation, &scattered, state);
                    break;
            }
            if(ret_scatter){
                ret *= attenuation;
                *r = scattered;
            } else{
                ret *= 0.0f;
                break;
            }
        } else {
            float3 unit_direction = normalize(r->dir);
            float t = 0.5*(unit_direction.y + 1.0);
            ret *= ((1.0f-t)*(float3)(1.0, 1.0, 1.0) + t*(float3)(0.5f, 0.7f, 1.0f));
            break;
        }
    }

    return ret;
}

__kernel void material(
    __global uchar* buf,
    __global sphere* spl,
    int num_sp,
    camera cam,
    ulong seed
){
    int image_width = get_global_size(0);
    int image_height = get_global_size(1);

    int i = get_global_id(0);
    int j = get_global_id(1);

#ifdef USE_ULONG_RAND
    ulong randseed = seed;
#else
    uint2 randseed = (uint2)((uint)seed, (uint)(seed>>32));
#endif

    int ofs = ((j * image_width) + i)*3;

    float3 color = (float3)(0, 0, 0);

    int samples_per_pixel = 100;
    for(int s = 0; s < samples_per_pixel; s++){
        float u = ((float)i + rand_float(&randseed))/ ((float)image_width-1.0);
        float v = ((float)j + rand_float(&randseed))/ ((float)image_height-1.0);

        ray r;
        get_ray(&cam, &r, u, v);
        color += ray_color(&r, spl, num_sp, &randseed, 50);
    }
    float scale = 1.0f/ (float)(samples_per_pixel);

    uchar3 u8vec3 = convert_uchar3(255.999f * sqrt(color * scale));

    buf[ofs] = u8vec3.s2;
    buf[ofs+1] = u8vec3.s1;
    buf[ofs+2] = u8vec3.s0;
}


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
    float3 p;
    float3 normal;
    float t;
    bool front_face;
} hit_record;

void set_face_normal(hit_record* rec, ray* r, float3* outward_normal)
{
    rec->front_face = dot(r->dir, *outward_normal) < 0;
    rec->normal = rec->front_face ? *outward_normal :-*outward_normal;
}

typedef struct
{
    float3 center;
    float radius;
} sphere;

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
    rec->normal = (rec->p - sp->center) / sp->radius;

    float3 outward_normal = (rec->p - sp->center) / sp->radius;
    set_face_normal(rec, r, &outward_normal);

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

float3 ray_color(ray *r, __global sphere *spl, int num_obj)
{
    hit_record rec;
    if (world_hit(r, spl, num_obj, 0, INFINITY, &rec)) {
        return 0.5f * (rec.normal + (float3)(1,1,1));
    }
    float3 unit_direction = normalize(r->dir);
    float t = 0.5*(unit_direction.y + 1.0);
    return (1.0f-t)*(float3)(1.0, 1.0, 1.0) + t*(float3)(0.5f, 0.7f, 1.0f);
}

__kernel void hittable_list(
    __global uchar* buf,
    __global sphere* spl,
    int num_sp,
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

    float3 color = ray_color(&r, spl, num_sp);
    uchar3 u8vec3 = convert_uchar3(255.999f * color);

    buf[ofs] = u8vec3.s2;
    buf[ofs+1] = u8vec3.s1;
    buf[ofs+2] = u8vec3.s0;
}

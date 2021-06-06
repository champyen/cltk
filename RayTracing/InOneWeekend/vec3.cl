
__kernel void vec3(
    __global uchar* buf
){
    int image_width = get_global_size(0);
    int image_height = get_global_size(1);

    int i = get_global_id(0);
    int j = get_global_id(1);

    int ofs = ((j * image_width) + i)*3;
    float3 color = (float3)(
        0.25,
        (float)j / ((float)image_height-1.0),
        (float)i / ((float)image_width-1.0)
    );

    uchar3 u8color = convert_uchar3(255.999f*color);

    buf[ofs] = u8color.s0;
    buf[ofs+1] = u8color.s1;
    buf[ofs+2] = u8color.s2;
}

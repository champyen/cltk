
__kernel void test(
    __global int* buf
){
    int gidx = get_global_id(0);
    buf[gidx] = gidx;
}

__kernel void test2(
    __global int* buf0,
    __global int* buf1    
){
    int gidx = get_global_id(0);
    buf0[gidx] = buf1[gidx]+1;
}

__kernel void test3(
    __global float* buf,
    int width,
    int height
){
    int gidx = get_global_id(0);
    int gidy = get_global_id(1);

    int ofs = gidy*width + gidx*8;
    int8 idx = (int8){0, 1, 2, 3, 4, 5, 6, 7};
    idx += ofs;
    vstore8(log(convert_float8(idx)), 0, buf + ofs);
}

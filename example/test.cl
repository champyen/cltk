
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


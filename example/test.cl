
__kernel void test(
    __global int* test_buf
){
    int gidx = get_global_id(0);
    test_buf[gidx] = gidx;
}


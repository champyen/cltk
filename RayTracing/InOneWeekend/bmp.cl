
__kernel void bmp(
    __global uchar* buf
){
    int image_width = get_global_size(0);
    int image_height = get_global_size(1);

    int i = get_global_id(0);
    int j = get_global_id(1);

    int ofs = ((j * image_width) + i)*3;

    float r = (float)i / ((float)image_width-1.0);
    float g = (float)j / ((float)image_height-1.0);
    float b = 0.25;

    uchar ir = (uchar)(255.999*r);
    uchar ig = (uchar)(255.999*g);
    uchar ib = (uchar)(255.999*b);

    buf[ofs] = ib;
    buf[ofs+1] = ig;
    buf[ofs+2] = ir;

}

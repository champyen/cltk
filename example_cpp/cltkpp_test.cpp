#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#include "cltkpp.h"

void cltkpp_example(void)
{
    cltk::context ctx;
    shared_ptr<cltk::buffer> buf = ctx.allocBuffer(1024*sizeof(int));
    shared_ptr<cltk::buffer> buf2 = ctx.allocBuffer(1024*sizeof(int));
    shared_ptr<cltk::library> lib = ctx.getLibrary("test.cl", "test.bin", "");
    shared_ptr<cltk::function> func = lib->getFunction("test");
    shared_ptr<cltk::function> func2 = lib->getFunction("test2");
    size_t gsize[3] = {1024, 1, 1 };

    func->NDR(1, gsize, NULL).exec(buf);
    func2->NDR(1, gsize, NULL).exec(buf2, buf);

    int* hostbuf = (int*)buf->map();
    int* hostbuf2 = (int*)buf2->map();
    for(int idx = 0; idx < 16; idx++)
        printf("%03X:%03X ", hostbuf[idx], hostbuf2[idx]);
    printf("\n");
    for(int idx = 1024-16; idx < 1024; idx++)
        printf("%X:%X ", hostbuf[idx], hostbuf2[idx]);
    printf("\n");
    buf->unmap();
    buf2->unmap();

    {
        int width = 4096;
        int height = 3072;
        struct timeval s_time, e_time;
        shared_ptr<cltk::function> func3 = lib->getFunction("test3");
        size_t gsize3[3] = {width/8, height, 0 };
        shared_ptr<cltk::buffer> buf3 = ctx.allocBuffer(width*height*sizeof(float));

        float* hostbuf3 = (float*)calloc(1, width*height*sizeof(float));
        gettimeofday(&s_time, NULL);
        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                hostbuf3[y*width + x] = logf(y*width + x);
            }
        }
        gettimeofday(&e_time, NULL);
        printf("%s CPU log 12M takes %ld us\n", __func__, (e_time.tv_sec - s_time.tv_sec)*1000000 + e_time.tv_usec - s_time.tv_usec);
        free(hostbuf3);

        gettimeofday(&s_time, NULL);
        func3->NDR(2, gsize3, NULL).exec(buf3, width, height);
        gettimeofday(&e_time, NULL);
        printf("%s GPU log 12M takes %ld us\n", __func__, (e_time.tv_sec - s_time.tv_sec)*1000000 + e_time.tv_usec - s_time.tv_usec);
    }
}

int main(void)
{
    cltkpp_example();

    return 0;
}
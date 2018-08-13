#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <sys/time.h>
#include <math.h>
#include "cltk.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void cltk_image_example(void)
{
    cltk_context ctx = cltk_context_create();
    cltk_lib lib = cltk_lib_load(ctx, "example_image/image_test.cl", NULL, "");
    cltk_func func = cltk_func_get(lib, "warp");

    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load("example_image/PSV.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    printf("w, h, c: %d %d %d\n", texWidth, texHeight, texChannels);

    cltk_buffer buf = cltk_buffer_alloc(ctx, 960*540*3);
    cltk_buffer pos = cltk_buffer_alloc(ctx, sizeof(float)*9);
    float *pos_ptr = (float*)cltk_buffer_map(pos);
    //TODO: calculate homography matrix
    pos_ptr[0] = 0.354f;    pos_ptr[1] = 0.385f;
    pos_ptr[2] = 0.646f;    pos_ptr[3] = 0.514f;
    pos_ptr[4] = 0.264f;    pos_ptr[5] = 0.640f;
    pos_ptr[6] = 0.580f;    pos_ptr[7] = 0.829f;
    cltk_buffer_unmap(pos);

    cl_image_format img_fmt = { CL_RGBA, CL_UNORM_INT8 };
    cltk_image_desc img_desc = {
                                    texWidth,
                                    texHeight,
                                    1,
                                    CL_UNORM_INT8,
                                    &img_fmt
                                };
    cltk_image img = cltk_image_alloc(ctx, &img_desc);
    size_t pitch;
    unsigned char *img_ptr = (unsigned char*)cltk_image_map(img, &pitch);
    printf("pitch %ld\n", pitch);
    for(int y = 0; y < texHeight; y++){
        memcpy(img_ptr + y*pitch, pixels + y*texWidth*4, texWidth*4);
    }
    //stbi_write_jpg("in.jpg", texWidth, texHeight, 1, src_ptr, 90);
    cltk_image_unmap(img);
    printf("texture done\n");

    size_t gsize[3] = {960, 540, 1 };

    printf("exec\n");
    CLTK_FUNC_CALL(func, 2, gsize, NULL, img, buf, pos);
    printf("exec done\n");

    unsigned char *buf_ptr = (unsigned char*)cltk_buffer_map(buf);
    stbi_write_jpg("out.jpg", 960, 540, 3, buf_ptr, 95);
    cltk_buffer_unmap(buf);

    cltk_func_release(func);
    cltk_lib_unload(lib);
    cltk_image_free(img);
    cltk_buffer_free(pos);
    cltk_buffer_free(buf);
    cltk_context_destroy(ctx);
}

int main(void)
{
    cltk_image_example();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>

#include <CL/cl.h>
#include "cltk.h"


#define FLT_MAX     __FLT_MAX__


void naive_gemm(float *a, float*b, float *c, int m, int n, int k)
{
    for(int y = 0; y < m; y++){
        for(int x = 0; x < n; x++){
            float sum = 0.f;
            for(int z = 0; z < k; z++){
                sum += (*(a + y*k + z)) * (*(b + z*n + x));
            }
            *(c + y*n + x) = sum;
        }
    }
}

void naive_gemm2(float *a, float*b, float *c, int m, int n, int k)
{
    for(int y = 0; y < m; y++){
        for(int x = 0; x < n; x++){
            float sum = 0;
            for(int z = 0; z < k; z++){
                sum += (*(a + y*k + z)) * (*(b + x*k + z));
            }
            *(c + y*n + x) = sum;
        }
    }
}

#define MAT_M   256
#define MAT_N   4096
#define MAT_K   1024

void result_cmp(float *cpu, float *gpu, int m, int n)
{
    for(int y = 0; y < m; y++){
        for(int x = 0; x < n; x++){
            float val = fabsf(gpu[y*n + x] - cpu[y*n + x]);
            if(val > 1.f){
                printf("%d %d min: %.2f %.2f\n", x, y, gpu[y*MAT_N + x], cpu[y*MAT_N + x]);
            }
        }
    }

}

int main(void)
{
    struct timeval s_time, e_time;
    cltk_context ctx = cltk_context_create();
    cltk_lib lib = cltk_lib_load(ctx, "gemm_cl/gemm.cl", NULL, "");
    cltk_buffer A_buf = cltk_buffer_alloc(ctx, MAT_M*MAT_K*sizeof(float));
    cltk_buffer B_buf = cltk_buffer_alloc(ctx, MAT_N*MAT_K*sizeof(float));
    cltk_buffer C0_buf = cltk_buffer_alloc(ctx, MAT_M*MAT_N*sizeof(float));
    cltk_buffer C1_buf = cltk_buffer_alloc(ctx, MAT_M*MAT_N*sizeof(float));
    cl_image_format img_fmt = { CL_RGBA, CL_FLOAT };
    cltk_image_desc img_desc = {
                                    MAT_N/4,
                                    MAT_K,
                                    1,
                                    CL_FLOAT,
                                    &img_fmt
                                };
    cltk_image B_img = cltk_image_alloc(ctx, &img_desc);
    printf("B pitch %zu usize %zu\n", B_img.pitch, B_img.unit_size);

    srand(time(NULL));

    float *a_buf = (float*)calloc(sizeof(float), MAT_M*MAT_K);
    float *b_buf = (float*)calloc(sizeof(float), MAT_N*MAT_K);
    float *c_buf = (float*)calloc(sizeof(float), MAT_M*MAT_N);
    for(int idx = 0; idx < MAT_M*MAT_K; idx++){
        a_buf[idx] = (float)((rand()%65536)*(rand()%16))/65535.f;
    }
    for(int idx = 0; idx < MAT_K*MAT_N; idx++){
        b_buf[idx] = (float)((rand()%65536)*(rand()%16))/65535.f;
    }
    naive_gemm(a_buf, b_buf, c_buf, MAT_M, MAT_N, MAT_K);

    float *Aptr = (float*)cltk_buffer_map(A_buf);
    float *Bptr = (float*)cltk_buffer_map(B_buf);
    size_t pitch;
    float *Bi_ptr = (float*)cltk_image_map(B_img, &pitch);
    printf("pitch %zu\n", pitch);

    //gettimeofday(&s_time, NULL);
    memcpy(Aptr, a_buf, MAT_M*MAT_K*sizeof(float));
    for(int idx = 0; idx < MAT_K; idx++){
        memcpy(Bi_ptr, b_buf + idx*MAT_N, MAT_N*sizeof(float));
        memcpy(Bptr, b_buf + idx*MAT_N, MAT_N*sizeof(float));
        Bi_ptr += pitch/sizeof(float);
        Bptr += MAT_N;
    }

    //gettimeofday(&e_time, NULL);
    //printf("%s data copy takes %ld us\n", __func__, (e_time.tv_sec - s_time.tv_sec)*1000000 + e_time.tv_usec - s_time.tv_usec);
    cltk_buffer_unmap(A_buf);
    cltk_buffer_unmap(B_buf);
    cltk_image_unmap(B_img);

    int lda = MAT_K;
    int ldc = MAT_N;
    int m = MAT_M;
    int n = MAT_N;
    int k = MAT_K;
    size_t gsize[3] = {MAT_N/4, MAT_M/8, 1 };
    cltk_func func = cltk_func_get(lib, "sgemm_buf_img");
    gettimeofday(&s_time, NULL);
    for(int i = 0; i < 10; i++){
        CLTK_FUNC_CALL(func, 2, gsize, NULL, A_buf, lda, C0_buf, ldc, m, n, k, B_img);
    }
    gettimeofday(&e_time, NULL);
    printf("%s test overhead gemm takes %ld us\n", __func__, (e_time.tv_sec - s_time.tv_sec)*1000000 + e_time.tv_usec - s_time.tv_usec);
    cltk_func_release(func);

    printf("check result 1...\n");
    float *Cptr = (float*)cltk_buffer_map(C0_buf);
    result_cmp(c_buf, Cptr, MAT_M, MAT_N);
    cltk_buffer_unmap(C0_buf);

    cltk_func func2 = cltk_func_get(lib, "sgemm_buf_only");
    gettimeofday(&s_time, NULL);
    for(int i = 0; i < 10; i++){
        CLTK_FUNC_CALL(func2, 2, gsize, NULL, A_buf, lda, C1_buf, ldc, m, n, k, B_buf);
    }
    gettimeofday(&e_time, NULL);
    printf("%s test overhead gemm_buf takes %ld us\n", __func__, (e_time.tv_sec - s_time.tv_sec)*1000000 + e_time.tv_usec - s_time.tv_usec);
    cltk_func_release(func2);

    printf("check result 2...\n");
    Cptr = (float*)cltk_buffer_map(C1_buf);
    result_cmp(c_buf, Cptr, MAT_M, MAT_N);
    cltk_buffer_unmap(C1_buf);

    free(a_buf);
    free(b_buf);
    free(c_buf);

    cltk_buffer_free(A_buf);
    cltk_buffer_free(B_buf);
    cltk_image_free(B_img);
    cltk_buffer_free(C0_buf);
    cltk_buffer_free(C1_buf);
    cltk_lib_unload(lib);
    cltk_context_destroy(ctx);
}

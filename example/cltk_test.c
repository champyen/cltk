#include <stdio.h>
#include <CL/cl.h>
#include <sys/time.h>
#include <math.h>
#include "cltk.h"

/* glue code for test() in test.cl */
void glue_test(cl_command_queue queue, cl_kernel kernel, size_t *gsize, cl_mem buf)
{
    cl_int cl_err = CL_SUCCESS;
    cl_err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buf);

    cl_err = clEnqueueNDRangeKernel(queue, kernel, 1, 0, gsize, NULL, 0, NULL, NULL);
    cl_err = clFinish(queue);
}

/* glue code for test2() in test.cl */
void glue_test2(cl_command_queue queue, cl_kernel kernel, size_t *gsize, cl_mem buf0, cl_mem buf1)
{
    cl_int cl_err = CL_SUCCESS;
    cl_err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buf0);
    cl_err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &buf1);

    cl_err = clEnqueueNDRangeKernel(queue, kernel, 1, 0, gsize, NULL, 0, NULL, NULL);
    cl_err = clFinish(queue);
}

/* glue code for test3() in test.cl */
void glue_test3(cl_command_queue queue, cl_kernel kernel, size_t *gsize, cl_mem buf, int width, int height)
{
    cl_int cl_err = CL_SUCCESS;
    cl_err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buf);
    cl_err = clSetKernelArg(kernel, 1, sizeof(int), &width);
    cl_err = clSetKernelArg(kernel, 2, sizeof(int), &height);

    cl_err = clEnqueueNDRangeKernel(queue, kernel, 2, 0, gsize, NULL, 0, NULL, NULL);
    cl_err = clFinish(queue);
}


/* example implementation in OpenCL directly - lacks of error message and detection */
void ocl_example(void)
{
    cl_int cl_err;
    cl_device_id devices[8];
    cl_context ctx = clCreateContextFromType(NULL, CL_DEVICE_TYPE_GPU, NULL, NULL, &cl_err);
    cl_err = clGetContextInfo(ctx, CL_CONTEXT_DEVICES, 8*sizeof(cl_device_id), devices, NULL);
    cl_command_queue queue = clCreateCommandQueue(ctx, devices[0], 0, &cl_err);
    cl_program program;
    {
        FILE *fptr = fopen("example/test.cl", "r");
        char *src_str = NULL;
        int src_len = fseek(fptr, 0L, SEEK_END);
        src_len = ftell(fptr);
        fseek(fptr, 0L, SEEK_SET);
        src_str = (char*)calloc(1, src_len+1);
        fread(src_str, 1, src_len, fptr);
        fclose(fptr);
        program = clCreateProgramWithSource(ctx, 1, (const char**)(&src_str), NULL, &cl_err);
    }
    cl_err = clBuildProgram(program, 1, devices, "", NULL, NULL);
    cl_kernel kernel = clCreateKernel(program, "test", &cl_err);
    cl_kernel kernel2 = clCreateKernel(program, "test2", &cl_err);
    cl_mem buffer = clCreateBuffer(ctx, CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_WRITE, 1024*sizeof(int), NULL, &cl_err);
    cl_mem buffer2 = clCreateBuffer(ctx, CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_WRITE, 1024*sizeof(int), NULL, &cl_err);
    size_t gsize[3] = {1024, 0, 0 };

    glue_test(queue, kernel, gsize, buffer);
    glue_test2(queue, kernel2, gsize, buffer2, buffer);


    int* hostbuf = (int*)clEnqueueMapBuffer(queue, buffer, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, 1024*sizeof(int), 0, NULL, NULL, &cl_err);
    int* hostbuf2 = (int*)clEnqueueMapBuffer(queue, buffer2, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, 1024*sizeof(int), 0, NULL, NULL, &cl_err);
    for(int idx = 0; idx < 16; idx++)
        printf("%03X:%03X ", hostbuf[idx], hostbuf2[idx]);
    printf("\n");
    for(int idx = 1024-16; idx < 1024; idx++)
        printf("%X:%0X ", hostbuf[idx], hostbuf2[idx]);
    printf("\n");
    cl_err = clEnqueueUnmapMemObject(queue, buffer, hostbuf, 0, NULL, NULL);
    cl_err = clEnqueueUnmapMemObject(queue, buffer2, hostbuf2, 0, NULL, NULL);

    {
        int width = 4096;
        int height = 3072;
        struct timeval s_time, e_time;
        cl_kernel kernel3 = clCreateKernel(program, "test3", &cl_err);
        size_t gsize3[3] = {width/8, height, 0 };
        cl_mem buffer3 = clCreateBuffer(ctx, CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_WRITE, width*height*sizeof(float), NULL, &cl_err);

        float* hostbuf3 = (float*)calloc(1, width*height*sizeof(float));
        gettimeofday(&s_time, NULL);
        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                hostbuf3[y*width + x] = logf(y*width + x);
            }
        }
        gettimeofday(&e_time, NULL);
        printf("%s: CPU log 12M takes %ld us\n", __func__, (e_time.tv_sec - s_time.tv_sec)*1000000 + e_time.tv_usec - s_time.tv_usec);
        free(hostbuf3);

        gettimeofday(&s_time, NULL);
        glue_test3(queue, kernel3, gsize3, buffer3, width, height);
        gettimeofday(&e_time, NULL);
        printf("%s GPU log 12M takes %ld us\n", __func__, (e_time.tv_sec - s_time.tv_sec)*1000000 + e_time.tv_usec - s_time.tv_usec);

        clReleaseMemObject(buffer3);
        clReleaseKernel(kernel3);
    }

    clReleaseMemObject(buffer2);
    clReleaseMemObject(buffer);
    clReleaseKernel(kernel2);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(ctx);
}

/* example implementation with CLTK */
void cltk_example(void)
{
    cltk_context ctx = cltk_context_create();
    cltk_buffer buf = cltk_buffer_alloc(ctx, 1024*sizeof(int));
    cltk_buffer buf2 = cltk_buffer_alloc(ctx, 1024*sizeof(int));
    cltk_lib lib = cltk_lib_load(ctx, "example/test.cl", "");
    cltk_func func = cltk_func_get(lib, "test");
    cltk_func func2 = cltk_func_get(lib, "test2");
    size_t gsize[3] = {1024, 0, 0 };

    CLTK_FUNC_CALL(func, 1, gsize, NULL, buf);
    CLTK_FUNC_CALL(func2, 1, gsize, NULL, buf2, buf);

    int* hostbuf = (int*)cltk_buffer_map(buf);
    int* hostbuf2 = (int*)cltk_buffer_map(buf2);
    for(int idx = 0; idx < 16; idx++)
        printf("%03X:%03X ", hostbuf[idx], hostbuf2[idx]);
    printf("\n");
    for(int idx = 1024-16; idx < 1024; idx++)
        printf("%X:%X ", hostbuf[idx], hostbuf2[idx]);
    printf("\n");
    cltk_buffer_unmap(buf);
    cltk_buffer_unmap(buf2);

    {
        int width = 4096;
        int height = 3072;
        struct timeval s_time, e_time;
        cltk_func func3 = cltk_func_get(lib, "test3");
        size_t gsize3[3] = {width/8, height, 0 };
        cltk_buffer buf3 = cltk_buffer_alloc(ctx, width*height*sizeof(float));

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
        CLTK_FUNC_CALL(func3, 2, gsize3, NULL, buf3, width, height);
        gettimeofday(&e_time, NULL);
        printf("%s GPU log 12M takes %ld us\n", __func__, (e_time.tv_sec - s_time.tv_sec)*1000000 + e_time.tv_usec - s_time.tv_usec);

        cltk_func_release(func3);
        cltk_buffer_free(buf3);
    }

    cltk_func_release(func2);
    cltk_func_release(func);
    cltk_lib_unload(lib);
    cltk_buffer_free(buf2);
    cltk_buffer_free(buf);
    cltk_context_destroy(ctx);
}

int main(void)
{
    ocl_example();
    cltk_example();
}

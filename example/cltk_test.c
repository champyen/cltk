#include <stdio.h>
#include <CL/cl.h>
#include "cltk.h"

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
    cl_mem buffer = clCreateBuffer(ctx, CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_WRITE, 1024*sizeof(int), NULL, &cl_err);
    size_t gsize[3] = {1024, 0, 0 };

    cl_err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer);

    cl_err = clEnqueueNDRangeKernel(queue, kernel, 1, 0, gsize, NULL, 0, NULL, NULL);
    cl_err = clFinish(queue);

    int* hostbuf = (int*)clEnqueueMapBuffer(queue, buffer, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, 1024*sizeof(int), 0, NULL, NULL, &cl_err);
    for(int idx = 0; idx < 16; idx++)
        printf("%03X ", hostbuf[idx]);
    printf("\n");
    for(int idx = 1024-16; idx < 1024; idx++)
        printf("%X ", hostbuf[idx]);
    printf("\n");
    cl_err = clEnqueueUnmapMemObject(queue, buffer, hostbuf, 0, NULL, NULL);

    clReleaseMemObject(buffer);
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
    cltk_lib lib = cltk_lib_load(ctx, "example/test.cl", "");
    cltk_func func = cltk_func_get(lib, "test");
    size_t gsize[3] = {1024, 0, 0 };

    CLTK_FUNC_CALL(func, 1, gsize, NULL, buf);

    int* hostbuf = (int*)cltk_buffer_map(buf);
    for(int idx = 0; idx < 16; idx++)
        printf("%03X ", hostbuf[idx]);
    printf("\n");
    for(int idx = 1024-16; idx < 1024; idx++)
        printf("%X ", hostbuf[idx]);
    printf("\n");
    cltk_buffer_unmap(buf);

    cltk_func_release(func);
    cltk_lib_unload(lib);
    cltk_buffer_free(buf);
    cltk_context_destroy(ctx);
}

int main(void)
{
    ocl_example();
    cltk_example();
}

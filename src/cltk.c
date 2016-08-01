
#include <stdio.h>
#include <stdlib.h>
#include "cltk_internal.h"

#define _CALL_STR_ARG(_1, _2, N, ...) N

#define CLTK_CL( CLTK_CALL, msg ) { \
    cl_int _cltk_err = CL_SUCCESS; \
    CLTK_CALL; \
    if(_cltk_err != CL_SUCCESS){ \
        printf("error: %s\n", cltk_error_message(_cltk_err)); \
        printf msg; \
        exit(0); \
    } \
}

static const char* cltk_error_str[] =
{
    // Error Codes
      "CL_SUCCESS"                                      //   0
    , "CL_DEVICE_NOT_FOUND"                             //  -1
    , "CL_DEVICE_NOT_AVAILABLE"                         //  -2
    , "CL_COMPILER_NOT_AVAILABLE"                       //  -3
    , "CL_MEM_OBJECT_ALLOCATION_FAILURE"                //  -4
    , "CL_OUT_OF_RESOURCES"                             //  -5
    , "CL_OUT_OF_HOST_MEMORY"                           //  -6
    , "CL_PROFILING_INFO_NOT_AVAILABLE"                 //  -7
    , "CL_MEM_COPY_OVERLAP"                             //  -8
    , "CL_IMAGE_FORMAT_MISMATCH"                        //  -9
    , "CL_IMAGE_FORMAT_NOT_SUPPORTED"                   //  -10
    , "CL_BUILD_PROGRAM_FAILURE"                        //  -11
    , "CL_MAP_FAILURE"                                  //  -12

    , "CL_MISALIGNED_SUB_BUFFER_OFFSET"                 //  -13
    , "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST"    //  -14
    , "CL_COMPILE_PROGRAM_FAILURE"                      //  -15
    , "CL_LINKER_NOT_AVAILABLE"                         //  -16
    , "CL_LINK_PROGRAM_FAILURE"                         //  -17
    , "CL_DEVICE_PARTITION_FAILED"                      //  -18
    , "CL_KERNEL_ARG_INFO_NOT_AVAILABLE"                //  -19

    , ""        //  -20
    , ""        //  -21
    , ""        //  -22
    , ""        //  -23
    , ""        //  -24
    , ""        //  -25
    , ""        //  -26
    , ""        //  -27
    , ""        //  -28
    , ""        //  -29

    , "CL_INVALID_VALUE"                                //  -30
    , "CL_INVALID_DEVICE_TYPE"                          //  -31
    , "CL_INVALID_PLATFORM"                             //  -32
    , "CL_INVALID_DEVICE"                               //  -33
    , "CL_INVALID_CONTEXT"                              //  -34
    , "CL_INVALID_QUEUE_PROPERTIES"                     //  -35
    , "CL_INVALID_COMMAND_QUEUE"                        //  -36
    , "CL_INVALID_HOST_PTR"                             //  -37
    , "CL_INVALID_MEM_OBJECT"                           //  -38
    , "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR"              //  -39
    , "CL_INVALID_IMAGE_SIZE"                           //  -40
    , "CL_INVALID_SAMPLER"                              //  -41
    , "CL_INVALID_BINARY"                               //  -42
    , "CL_INVALID_BUILD_OPTIONS"                        //  -43
    , "CL_INVALID_PROGRAM"                              //  -44
    , "CL_INVALID_PROGRAM_EXECUTABLE"                   //  -45
    , "CL_INVALID_KERNEL_NAME"                          //  -46
    , "CL_INVALID_KERNEL_DEFINITION"                    //  -47
    , "CL_INVALID_KERNEL"                               //  -48
    , "CL_INVALID_ARG_INDEX"                            //  -49
    , "CL_INVALID_ARG_VALUE"                            //  -50
    , "CL_INVALID_ARG_SIZE"                             //  -51
    , "CL_INVALID_KERNEL_ARGS"                          //  -52
    , "CL_INVALID_WORK_DIMENSION"                       //  -53
    , "CL_INVALID_WORK_GROUP_SIZE"                      //  -54
    , "CL_INVALID_WORK_ITEM_SIZE"                       //  -55
    , "CL_INVALID_GLOBAL_OFFSET"                        //  -56
    , "CL_INVALID_EVENT_WAIT_LIST"                      //  -57
    , "CL_INVALID_EVENT"                                //  -58
    , "CL_INVALID_OPERATION"                            //  -59
    , "CL_INVALID_GL_OBJECT"                            //  -60
    , "CL_INVALID_BUFFER_SIZE"                          //  -61
    , "CL_INVALID_MIP_LEVEL"                            //  -62
    , "CL_INVALID_GLOBAL_WORK_SIZE"                     //  -63
    , "CL_INVALID_PROPERTY"                             //  -64
    , "CL_INVALID_IMAGE_DESCRIPTOR"                     //  -65
    , "CL_INVALID_COMPILER_OPTIONS"                     //  -66
    , "CL_INVALID_LINKER_OPTIONS"                       //  -67
    , "CL_INVALID_DEVICE_PARTITION_COUNT"               //  -68
    , "CL_INVALID_PIPE_SIZE"                            //  -69
    , "CL_INVALID_DEVICE_QUEUE"                         //  -70
    , "CL_UNKNOWN_ERROR_CODE"
};


const char* cltk_error_message(int err)
{
    int idx = -err;
    if(idx > 70 || idx < 0)
        return cltk_error_str[70];
    else
        return cltk_error_str[idx];
}

cltk_context cltk_context_create(void)
{
    cl_device_id devices[8];
    cltk_context ctx = (cltk_context)calloc(1, sizeof(_cltk_context));
    CLTK_CL(ctx->context = clCreateContextFromType(NULL, CL_DEVICE_TYPE_GPU, NULL, NULL, &_cltk_err), (""));
    CLTK_CL(_cltk_err = clGetContextInfo(ctx->context, CL_CONTEXT_DEVICES, 8*sizeof(cl_device_id), devices, NULL), (""));
    CLTK_CL(ctx->queue = clCreateCommandQueue(ctx->context, devices[0], 0, &_cltk_err), (""));
    return ctx;
}

void cltk_context_destroy(cltk_context ctx)
{
    CLTK_CL(_cltk_err = clReleaseContext(ctx->context), (""));
    free(ctx);
}

cltk_lib cltk_lib_load(cltk_context ctx, char *filename, char *buildopt)
{
    cltk_lib lib = NULL;
    FILE *fptr = fopen(filename, "r");
    if(fptr){
        cl_program program = NULL;
        char *src_str = NULL;
        int src_len = fseek(fptr, 0L, SEEK_END);
        src_len = ftell(fptr);
        fseek(fptr, 0L, SEEK_SET);
        src_str = (char*)calloc(1, src_len+1);
        fread(src_str, 1, src_len, fptr);
        fclose(fptr);
        CLTK_CL(program = clCreateProgramWithSource(ctx->context, 1, (const char**)(&src_str), NULL, &_cltk_err), (""));
        if(program){
            char *log_dump;
            size_t log_len;
            cl_device_id devices[8];
            lib = (cltk_lib)calloc(1, sizeof(_cltk_lib));
            lib->program = program;
            lib->ctx = ctx;
            CLTK_CL(_cltk_err = clGetContextInfo(ctx->context, CL_CONTEXT_DEVICES, 8*sizeof(cl_device_id), devices, NULL), (""));
            CLTK_CL(_cltk_err = clBuildProgram(lib->program, 1, devices, buildopt, NULL, NULL), (""));
            CLTK_CL(_cltk_err =  clGetProgramBuildInfo(lib->program, devices[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &log_len), (""));
            log_dump = (char*)calloc(1, log_len);
            CLTK_CL(_cltk_err =  clGetProgramBuildInfo(lib->program, devices[0], CL_PROGRAM_BUILD_LOG, log_len, log_dump, NULL), ("") );
            printf("build log: %s\n", log_dump);
        }
        free(src_str);
    }
    return lib;
}

void cltk_lib_unload(cltk_lib lib)
{
    CLTK_CL(_cltk_err = clReleaseProgram(lib->program), ("") );
    free(lib);
}

cltk_func cltk_func_get(cltk_lib lib, char *funcname)
{
    cltk_func func = NULL;
    cl_kernel kernel = NULL;
    CLTK_CL(kernel = clCreateKernel(lib->program, funcname, &_cltk_err), ("get kernel %s failed\n", funcname));
    if(kernel){
        func = (cltk_func)calloc(1, sizeof(_cltk_func));
        func->ctx = lib->ctx;
        func->kernel = kernel;
    }
    return func;
}

void cltk_func_release(cltk_func func)
{
    if(func){
        CLTK_CL( _cltk_err = clReleaseKernel(func->kernel), (""));
        free(func);
    }
}

void cltk_func_setenv(cltk_func func, int dim, size_t* global_size, size_t *local_size)
{
    func->dimension = dim;
    func->global = global_size;
    func->local = local_size;
}

void cltk_func_setarg(cltk_func func, int arg_size, void *arg_ptr)
{
    if(arg_size > 8){
        _cltk_buffer *buffer = ((cltk_buffer*)arg_ptr)->mem;
        CLTK_CL(_cltk_err = clSetKernelArg(func->kernel, func->arg_index, sizeof(cl_mem), &(buffer->memory)), (""));
    }
    else{
        CLTK_CL(_cltk_err = clSetKernelArg(func->kernel, func->arg_index, arg_size, arg_ptr), (""));
    }
    func->arg_index++;
}

void cltk_func_exec(cltk_func func)
{
    CLTK_CL(_cltk_err = clEnqueueNDRangeKernel(func->ctx->queue, func->kernel, func->dimension, 0, func->global, NULL, 0, NULL, NULL), (""));
    CLTK_CL(_cltk_err = clFinish(func->ctx->queue), (""));
    func->arg_index = 0;
}

void cltk_func_async_exec(cltk_func func)
{
    CLTK_CL(_cltk_err = clEnqueueNDRangeKernel(func->ctx->queue, func->kernel, func->dimension, 0, func->global, NULL, 0, NULL, NULL), (""));
    CLTK_CL(_cltk_err = clFlush(func->ctx->queue), (""));
}

void cltk_func_wait(cltk_func func)
{
    CLTK_CL(_cltk_err = clFinish(func->ctx->queue), (""));
    func->arg_index = 0;
}

cltk_buffer cltk_buffer_alloc(cltk_context ctx, int buf_size)
{
    cl_mem cl_buf = NULL;
    cltk_buffer buffer;
    CLTK_CL( cl_buf = clCreateBuffer(ctx->context, CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_WRITE, buf_size, NULL, &_cltk_err), (""));
    if(cl_buf){
        buffer.mem = (cltk_mem)calloc(1, sizeof(_cltk_buffer));
        buffer.mem->signature = 0x5167924F;
        buffer.mem->ctx = ctx;
        buffer.mem->memory = cl_buf;
        buffer.mem->size = buf_size;
    }
    return buffer;
}

void cltk_buffer_free(cltk_buffer buffer)
{
    if(buffer.mem){
        cltk_buffer_unmap(buffer);
        CLTK_CL(_cltk_err = clReleaseMemObject(buffer.mem->memory), (""));
        free(buffer.mem);
    }
}

void* cltk_buffer_map(cltk_buffer buffer)
{
    if(buffer.mem->is_mapped == 0){
        CLTK_CL(buffer.mem->hostptr = clEnqueueMapBuffer(buffer.mem->ctx->queue, buffer.mem->memory, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, buffer.mem->size, 0, NULL, NULL, &_cltk_err), (""));
        buffer.mem->is_mapped = 1;
    }
    return buffer.mem->hostptr;
}

void cltk_buffer_unmap(cltk_buffer buffer)
{
    if(buffer.mem && buffer.mem->is_mapped){
        CLTK_CL(_cltk_err = clEnqueueUnmapMemObject(buffer.mem->ctx->queue, buffer.mem->memory, buffer.mem->hostptr, 0, NULL, NULL), (""));
        buffer.mem->is_mapped = 0;
        buffer.mem->hostptr = NULL;
    }
}

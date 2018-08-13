
#include <stdio.h>
#include <stdlib.h>
#include "cltk_internal.h"

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
    , "CL_UNKNOWN_ERROR_CODE"				//  -71
};

#define CLTK_MAX_ERRIDX					71

const char* cltk_error_message(int err)
{
    int idx = -err;
    if(idx > CLTK_MAX_ERRIDX || idx < 0)
        return cltk_error_str[CLTK_MAX_ERRIDX];
    else
        return cltk_error_str[idx];
}

cltk_context cltk_context_create(void)
{
    /*
    cl_device_id device;
    cl_uint size;
    cltk_context ctx = (cltk_context)calloc(1, sizeof(_cltk_context));
    CLTK_CL(ctx->context = clCreateContextFromType(NULL, CL_DEVICE_TYPE_GPU, NULL, NULL, &_cltk_err), ("%s clCreateContextFromType\n", __func__));
    CLTK_CL(_cltk_err = clGetContextInfo(ctx->context, CL_CONTEXT_DEVICES, 1, &devices, NULL), ("%s clGetContextInfo 1\n", __func__));
    CLTK_CL(ctx->queue = clCreateCommandQueue(ctx->context, devices[0], 0, &_cltk_err), ("%s clCreateCommandQueue\n", __func__));
    */
    cl_platform_id plat_id;
    cl_device_id dev_id;

    cltk_context ctx = (cltk_context)calloc(1, sizeof(_cltk_context));
    CLTK_CL(_cltk_err = clGetPlatformIDs(1, &plat_id, NULL), ("%s %d\n", __FUNCTION__, __LINE__));
    CLTK_CL(_cltk_err = clGetDeviceIDs(plat_id, CL_DEVICE_TYPE_GPU, 1, &dev_id, NULL), ("%s %d\n", __FUNCTION__, __LINE__));
    CLTK_CL(ctx->context = clCreateContext(NULL, 1, &dev_id, NULL, NULL, &_cltk_err), ("%s %d\n", __FUNCTION__, __LINE__));
    CLTK_CL(ctx->queue = clCreateCommandQueue(ctx->context, dev_id, 0, &_cltk_err), ("%s %d\n", __FUNCTION__, __LINE__));
    return ctx;
}

void cltk_context_destroy(cltk_context ctx)
{
    CLTK_CL(_cltk_err = clReleaseCommandQueue(ctx->queue), ("%s %d\n", __FUNCTION__, __LINE__));
    CLTK_CL(_cltk_err = clReleaseContext(ctx->context), ("%s %d\n", __FUNCTION__, __LINE__));
    free(ctx);
}

cltk_lib cltk_lib_bin_load(cltk_context ctx, char *bin, size_t bin_len)
{
    cltk_lib lib = NULL;
    cl_device_id devices[8];
    CLTK_CL(_cltk_err = clGetContextInfo(ctx->context, CL_CONTEXT_DEVICES, 8*sizeof(cl_device_id), devices, NULL), ("%s %d\n", __FUNCTION__, __LINE__));
    cl_program program = NULL;
    CLTK_CL(program = clCreateProgramWithBinary(ctx->context, 1, devices, &bin_len, (const unsigned char**)(&bin), NULL, &_cltk_err), ("%s %d\n", __FUNCTION__, __LINE__));
    if(program){
        char *log_dump;
        size_t log_len;
        lib = (cltk_lib)calloc(1, sizeof(_cltk_lib));
        lib->program = program;
        lib->ctx = ctx;
        CLTK_CL(_cltk_err = clBuildProgram(lib->program, 1, devices, "", NULL, NULL), ("%s %d\n", __FUNCTION__, __LINE__));
    }
    return lib;
}

cltk_lib cltk_lib_str_load(cltk_context ctx, char *src_str, char *cache_fname, char *buildopt)
{
    cltk_lib lib = NULL;
    cl_program program = NULL;
    CLTK_CL(program = clCreateProgramWithSource(ctx->context, 1, (const char**)(&src_str), NULL, &_cltk_err), ("%s %d\n", __FUNCTION__, __LINE__));
    if(program){
        char *log_dump;
        size_t log_len;
        cl_device_id devices[8];
        lib = (cltk_lib)calloc(1, sizeof(_cltk_lib));
        lib->program = program;
        lib->ctx = ctx;
        CLTK_CL(_cltk_err = clGetContextInfo(ctx->context, CL_CONTEXT_DEVICES, 8*sizeof(cl_device_id), devices, NULL), ("%s %d\n", __FUNCTION__, __LINE__));
        CLTK_CL(_cltk_err = clBuildProgram(lib->program, 1, devices, buildopt, NULL, NULL), ("%s %d\n", __FUNCTION__, __LINE__));
        CLTK_CL(_cltk_err =  clGetProgramBuildInfo(lib->program, devices[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &log_len), ("%s %d\n", __FUNCTION__, __LINE__));
        log_dump = (char*)calloc(1, log_len);
        CLTK_CL(_cltk_err =  clGetProgramBuildInfo(lib->program, devices[0], CL_PROGRAM_BUILD_LOG, log_len, log_dump, NULL), ("%s %d\n", __FUNCTION__, __LINE__) );
        printf("build log: %s\n", log_dump);
        
        if(cache_fname){
            size_t bin_size;
            size_t ret_size;
            CLTK_CL(_cltk_err = clGetProgramInfo(lib->program, CL_PROGRAM_BINARY_SIZES, sizeof(size_t), &bin_size, &ret_size), ("%s %d\n", __FUNCTION__, __LINE__));
            if(bin_size){
                FILE *fptr = fopen(cache_fname, "wb");
                if(fptr){
                    unsigned char *bin_buf = (unsigned char*)malloc(bin_size + 128);
                    CLTK_CL(_cltk_err = clGetProgramInfo(lib->program, CL_PROGRAM_BINARIES, bin_size, &bin_buf, &ret_size), ("%s %d\n", __FUNCTION__, __LINE__));
                    fwrite(bin_buf, bin_size, 1, fptr);
                    fclose(fptr);
                    free(bin_buf);
                }
            }
        }
    }
    return lib;
}

static int cltk_file_load(char *fname, char *mode, char **buf)
{
    int flen = 0;
    *buf = NULL;
    FILE *fptr = fopen(fname, mode);
    if(fptr){
        flen = fseek(fptr, 0L, SEEK_END);
        flen = ftell(fptr);
        fseek(fptr, 0L, SEEK_SET);
        *buf = (char*)calloc(1, flen+1);
        fread(*buf, 1, flen, fptr);
        fclose(fptr);
    }
    return flen;
}

cltk_lib cltk_lib_load(cltk_context ctx, char *filename, char *cache_fname, char *buildopt)
{
    cltk_lib lib = NULL;
    if(cache_fname){
        char *bin_data = NULL;
        size_t bin_len = cltk_file_load(cache_fname, "rb", &bin_data);
        if(bin_data){
            lib = cltk_lib_bin_load(ctx, bin_data, bin_len);
            free(bin_data);
        }
    }
    if(lib == NULL){
        #if 0
        FILE *fptr = fopen(filename, "r");
        if(fptr){
            char *src_str = NULL;
            int src_len = fseek(fptr, 0L, SEEK_END);
            src_len = ftell(fptr);
            fseek(fptr, 0L, SEEK_SET);
            src_str = (char*)calloc(1, src_len+1);
            fread(src_str, 1, src_len, fptr);
            fclose(fptr);
            lib = cltk_lib_str_load(ctx, src_str, buildopt);
            free(src_str);
        }
        #else
        char *src_str = NULL;
        int sr_len = cltk_file_load(filename, "r", &src_str);
        if(src_str){
            lib = cltk_lib_str_load(ctx, src_str, cache_fname, buildopt);
            free(src_str);
        }
        #endif
    }
    return lib;
}

void cltk_lib_unload(cltk_lib lib)
{
    CLTK_CL(_cltk_err = clReleaseProgram(lib->program), ("%s %d\n", __FUNCTION__, __LINE__) );
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
        CLTK_CL( _cltk_err = clReleaseKernel(func->kernel), ("%s %d\n", __FUNCTION__, __LINE__));
        free(func);
    }
}

void cltk_func_setndr(cltk_func func, int dim, size_t* global_size, size_t *local_size)
{
    func->dimension = dim;
    func->global = global_size;
    func->local = local_size;
}

void cltk_func_setarg(cltk_func func, int arg_size, void *arg_ptr)
{
    _cltk_mem *mem_buf = (_cltk_mem*)arg_ptr;
    if(arg_size == sizeof(_cltk_mem) && (mem_buf->signature == CLTK_BUF_SIGNATURE | mem_buf->signature == CLTK_IMG_SIGNATURE)){
        _cltk_buffer *buffer = ((cltk_buffer*)arg_ptr)->mem;
        CLTK_CL(_cltk_err = clSetKernelArg(func->kernel, func->arg_index, sizeof(cl_mem), &(buffer->memory)), ("%s %d\n", __FUNCTION__, __LINE__));
    }
    else{
        CLTK_CL(_cltk_err = clSetKernelArg(func->kernel, func->arg_index, arg_size, arg_ptr), ("%s %d\n", __FUNCTION__, __LINE__));
    }
    func->arg_index++;
}

void cltk_func_exec(cltk_func func)
{
    CLTK_CL(_cltk_err = clEnqueueNDRangeKernel(func->ctx->queue, func->kernel, func->dimension, 0, func->global, func->local, 0, NULL, NULL), ("%s %d\n", __FUNCTION__, __LINE__));
    CLTK_CL(_cltk_err = clFinish(func->ctx->queue), ("%s %d\n", __FUNCTION__, __LINE__));
    func->arg_index = 0;
}

void cltk_func_async_exec(cltk_func func)
{
    CLTK_CL(_cltk_err = clEnqueueNDRangeKernel(func->ctx->queue, func->kernel, func->dimension, 0, func->global, NULL, 0, NULL, NULL), ("%s %d\n", __FUNCTION__, __LINE__));
    CLTK_CL(_cltk_err = clFlush(func->ctx->queue), ("%s %d\n", __FUNCTION__, __LINE__));
}

void cltk_func_wait(cltk_func func)
{
    CLTK_CL(_cltk_err = clFinish(func->ctx->queue), ("%s %d\n", __FUNCTION__, __LINE__));
    func->arg_index = 0;
}

cltk_buffer cltk_buffer_alloc(cltk_context ctx, int buf_size)
{
    cl_mem cl_buf = NULL;
    cltk_buffer buffer;
    CLTK_CL( cl_buf = clCreateBuffer(ctx->context, CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_WRITE, buf_size, NULL, &_cltk_err), ("%s %d\n", __FUNCTION__, __LINE__));
    if(cl_buf){
        buffer.signature = CLTK_BUF_SIGNATURE;
        cltk_buf buf = (cltk_buf)calloc(1, sizeof(_cltk_buffer));
        buf->ctx = ctx;
        buf->memory = cl_buf;
        buf->size = buf_size;
        buffer.mem = buf;
    }
    return buffer;
}

void cltk_buffer_free(cltk_buffer buffer)
{
    cltk_buf buf = buffer.mem;
    if(buf){
        cltk_buffer_unmap(buffer);
        CLTK_CL(_cltk_err = clReleaseMemObject(buf->memory), ("%s %d\n", __FUNCTION__, __LINE__));
        free(buf);
    }
}

void* cltk_buffer_map(cltk_buffer buffer)
{
    cltk_buf buf = buffer.mem;
    if(buf->is_mapped == 0){
        CLTK_CL(buf->hostptr = clEnqueueMapBuffer(buf->ctx->queue, buf->memory, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, buf->size, 0, NULL, NULL, &_cltk_err), ("%s %d\n", __FUNCTION__, __LINE__));
        buf->is_mapped = 1;
    }
    return buf->hostptr;
}

void cltk_buffer_unmap(cltk_buffer buffer)
{
    cltk_buf buf = buffer.mem;
    if(buf && buf->is_mapped){
        CLTK_CL(_cltk_err = clEnqueueUnmapMemObject(buf->ctx->queue, buf->memory, buf->hostptr, 0, NULL, NULL), ("%s %d\n", __FUNCTION__, __LINE__));
        CLTK_CL(_cltk_err = clFinish(buf->ctx->queue), ("%s %d\n", __FUNCTION__, __LINE__));
        buf->is_mapped = 0;
        buf->hostptr = NULL;
    }
}

cltk_image cltk_image_alloc(cltk_context ctx, cltk_image_desc* desc)
{
    cltk_image image;
    image.signature = CLTK_IMG_SIGNATURE;

    cl_mem cl_img = NULL;
    cl_image_format default_fmt = {CL_INTENSITY, desc->unit_type};
    cl_image_format *img_fmt = (desc->img_fmt == NULL) ? &default_fmt : desc->img_fmt;
    CLTK_CL(cl_img = clCreateImage2D(ctx->context, CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_WRITE, img_fmt, desc->width, desc->height, 0, NULL, &_cltk_err), ("%s %d\n", __FUNCTION__, __LINE__));
    if(cl_img){
        cltk_img img = (cltk_img)calloc(1, sizeof(_cltk_image));
        img->ctx = ctx;
        img->memory = cl_img;
        image.width = desc->width;
        image.height = desc->height;
        CLTK_CL( _cltk_err = clGetImageInfo(cl_img, CL_IMAGE_ELEMENT_SIZE, sizeof(size_t), &(image.unit_size), NULL), ("%s %d\n", __FUNCTION__, __LINE__));
        CLTK_CL( _cltk_err = clGetImageInfo(cl_img,  CL_IMAGE_ROW_PITCH, sizeof(size_t), &(image.pitch), NULL), ("%s %d\n", __FUNCTION__, __LINE__));
        image.pitch /= image.unit_size;
        printf("%s w[%d] h[%d] p[%zu] us[%zu]\n", __func__, image.width, image.height, image.pitch, image.unit_size);

        image.mem = img;
    }
    return image;
}

void cltk_image_free(cltk_image image)
{
    cltk_img img = image.mem;
    if(img){
        cltk_image_unmap(image);
        CLTK_CL(_cltk_err = clReleaseMemObject(img->memory), ("%s %d\n", __FUNCTION__, __LINE__));
        free(img);
    }
}

void* cltk_image_map(cltk_image image, size_t *pitch)
{
    cltk_img img = image.mem;
    if(img->is_mapped == 0){
        const size_t origin[3] = {0, 0, 0};
        const size_t region[3] = {(size_t)image.width, (size_t)image.height, 1};
        size_t slice_pitch;
        CLTK_CL(img->hostptr = clEnqueueMapImage(img->ctx->queue, img->memory, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, origin, region, pitch, &slice_pitch, 0, NULL, NULL, &_cltk_err), ("%s %d\n", __FUNCTION__, __LINE__));
        img->is_mapped = 1;
    }
    return img->hostptr;
}

void cltk_image_unmap(cltk_image image)
{
    cltk_img img = image.mem;
    if(img->is_mapped){
        CLTK_CL(_cltk_err = clEnqueueUnmapMemObject(img->ctx->queue, img->memory, img->hostptr, 0, NULL, NULL), ("%s %d\n", __FUNCTION__, __LINE__));
        CLTK_CL(_cltk_err = clFinish(img->ctx->queue), ("%s %d\n", __FUNCTION__, __LINE__));
        img->hostptr = NULL;
        img->is_mapped = 0;
    }
}

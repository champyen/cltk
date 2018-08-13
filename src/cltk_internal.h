#ifndef _CLTK_INTERNAL_H_
#define _CLTK_INTERNAL_H_

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>
#include <stdint.h>
#include "cltk.h"

#define CLTK_BUF_SIGNATURE      0x6675625F6B746C63
#define CLTK_IMG_SIGNATURE      0x676D695F6B746C63

typedef struct _cltk_context{
    cl_context              context;
    cl_command_queue        queue;
} _cltk_context;

typedef struct _cltk_lib{
    cltk_context            ctx;
    cl_program              program;
} _cltk_lib;

typedef struct _cltk_func{
    cltk_context            ctx;
    cl_kernel               kernel;
    cl_int                  dimension;
    int32_t                 arg_index;
    size_t                  *global;
    size_t                  *local;
} _cltk_func;

typedef struct _cltk_buffer{
    cltk_context            ctx;
    cl_mem                  memory;
    void                    *hostptr;
    cl_uint                 size;
    int32_t                 is_mapped;
} _cltk_buffer;

typedef struct _cltk_image{
    cltk_context            ctx;
    cl_mem                  memory;
    void                    *hostptr;
    int32_t                 is_mapped;
} _cltk_image;

typedef struct {
    int64_t                 signature;
    void*                   _cltk_memptr;
    int32_t                 width;
    int32_t                 height;
    size_t                  pitch;
    size_t                  unit_size;
} _cltk_mem;

#endif

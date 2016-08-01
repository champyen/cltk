#ifndef _CLTK_INTERNAL_H_
#define _CLTK_INTERNAL_H_

#include <CL/cl.h>
#include "cltk.h"

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
    int                     dimension;
    int                     arg_index;
    size_t                  *global;
    size_t                  *local;
} _cltk_func;

typedef struct _cltk_buffer{
    int                     signature;
    cl_mem                  memory;
    cltk_context            ctx;
    void                    *hostptr;
    cl_uint                 size;
    int                     is_mapped;
} _cltk_buffer;

#endif

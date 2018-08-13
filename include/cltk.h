#ifndef _CLTK_H_
#define _CLTK_H_

#ifdef __cplusplus
extern "C" {
#endif

#define _ARGS_HANDLE(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _A, _B, _C, _D, _E, _F, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1A, _1B, _1C, _1D, _1E, _1F, N, ...) N

#define CLTK_SET_ARG(func,x)    { cltk_func_setarg(func, sizeof(x), &x); }

#define _ARG_0(...)
#define _ARG_1(func,x) CLTK_SET_ARG(func,x)
#define _ARG_2(func,x, ...) CLTK_SET_ARG(func,x) _ARG_1(func,__VA_ARGS__)
#define _ARG_3(func,x, ...) CLTK_SET_ARG(func,x) _ARG_2(func,__VA_ARGS__)
#define _ARG_4(func,x, ...) CLTK_SET_ARG(func,x) _ARG_3(func,__VA_ARGS__)
#define _ARG_5(func,x, ...) CLTK_SET_ARG(func,x) _ARG_4(func,__VA_ARGS__)
#define _ARG_6(func,x, ...) CLTK_SET_ARG(func,x) _ARG_5(func,__VA_ARGS__)
#define _ARG_7(func,x, ...) CLTK_SET_ARG(func,x) _ARG_6(func,__VA_ARGS__)
#define _ARG_8(func,x, ...) CLTK_SET_ARG(func,x) _ARG_7(func,__VA_ARGS__)
#define _ARG_9(func,x, ...) CLTK_SET_ARG(func,x) _ARG_8(func,__VA_ARGS__)
#define _ARG_A(func,x, ...) CLTK_SET_ARG(func,x) _ARG_9(func,__VA_ARGS__)
#define _ARG_B(func,x, ...) CLTK_SET_ARG(func,x) _ARG_A(func,__VA_ARGS__)
#define _ARG_C(func,x, ...) CLTK_SET_ARG(func,x) _ARG_B(func,__VA_ARGS__)
#define _ARG_D(func,x, ...) CLTK_SET_ARG(func,x) _ARG_C(func,__VA_ARGS__)
#define _ARG_E(func,x, ...) CLTK_SET_ARG(func,x) _ARG_D(func,__VA_ARGS__)
#define _ARG_F(func,x, ...) CLTK_SET_ARG(func,x) _ARG_E(func,__VA_ARGS__)
#define _ARG_10(func,x, ...) CLTK_SET_ARG(func,x) _ARG_F(func,__VA_ARGS__)
#define _ARG_11(func,x, ...) CLTK_SET_ARG(func,x) _ARG_10(func,__VA_ARGS__)
#define _ARG_12(func,x, ...) CLTK_SET_ARG(func,x) _ARG_11(func,__VA_ARGS__)
#define _ARG_13(func,x, ...) CLTK_SET_ARG(func,x) _ARG_12(func,__VA_ARGS__)
#define _ARG_14(func,x, ...) CLTK_SET_ARG(func,x) _ARG_13(func,__VA_ARGS__)
#define _ARG_15(func,x, ...) CLTK_SET_ARG(func,x) _ARG_14(func,__VA_ARGS__)
#define _ARG_16(func,x, ...) CLTK_SET_ARG(func,x) _ARG_15(func,__VA_ARGS__)
#define _ARG_17(func,x, ...) CLTK_SET_ARG(func,x) _ARG_16(func,__VA_ARGS__)
#define _ARG_18(func,x, ...) CLTK_SET_ARG(func,x) _ARG_17(func,__VA_ARGS__)
#define _ARG_19(func,x, ...) CLTK_SET_ARG(func,x) _ARG_18(func,__VA_ARGS__)
#define _ARG_1A(func,x, ...) CLTK_SET_ARG(func,x) _ARG_19(func,__VA_ARGS__)
#define _ARG_1B(func,x, ...) CLTK_SET_ARG(func,x) _ARG_1A(func,__VA_ARGS__)
#define _ARG_1C(func,x, ...) CLTK_SET_ARG(func,x) _ARG_1B(func,__VA_ARGS__)
#define _ARG_1D(func,x, ...) CLTK_SET_ARG(func,x) _ARG_1C(func,__VA_ARGS__)
#define _ARG_1E(func,x, ...) CLTK_SET_ARG(func,x) _ARG_1D(func,__VA_ARGS__)
#define _ARG_1F(func,x, ...) CLTK_SET_ARG(func,x) _ARG_1E(func,__VA_ARGS__)

#define CLTK_FUNC_CALL(func, dim, gsize, lsize, ...) { \
    cltk_func_setndr(func, dim, gsize, lsize); \
    _ARGS_HANDLE("ignored", ##__VA_ARGS__, \
    _ARG_1F, _ARG_1E, _ARG_1D, _ARG_1C, _ARG_1B, _ARG_1A, _ARG_19, _ARG_18, \
    _ARG_17, _ARG_16, _ARG_15, _ARG_14, _ARG_13, _ARG_12, _ARG_11, _ARG_10, \
    _ARG_F, _ARG_E, _ARG_D, _ARG_C, _ARG_B, _ARG_A, _ARG_9, _ARG_8, \
    _ARG_7, _ARG_6, _ARG_5, _ARG_4, _ARG_3, _ARG_2, _ARG_1, _ARG_0)(func, ##__VA_ARGS__) \
    cltk_func_exec(func); \
}

typedef struct _cltk_context*           cltk_context;
typedef struct _cltk_lib*               cltk_lib;
typedef struct _cltk_func*              cltk_func;
typedef struct _cltk_buffer*            cltk_buf;
typedef struct _cltk_image*             cltk_img;

typedef struct {
    long long           signature;
    cltk_buf            mem;
    int32_t                 width;
    int32_t                 height;
    size_t                  pitch;
    size_t                  unit_size;
} cltk_buffer;

typedef enum{
    CLTK_SNORM_INT8 = 0x10D0,
    CLTK_SNORM_INT16,                              //0x10D1
    CLTK_UNORM_INT8,                               //0x10D2
    CLTK_UNORM_INT16,                              //0x10D3
    CLTK_UNORM_SHORT_565,                          //0x10D4
    CLTK_UNORM_SHORT_555,                          //0x10D5
    CLTK_UNORM_INT_101010,                         //0x10D6
    CLTK_SIGNED_INT8,                              //0x10D7
    CLTK_SIGNED_INT16,                             //0x10D8
    CLTK_SIGNED_INT32,                             //0x10D9
    CLTK_UNSIGNED_INT8,                            //0x10DA
    CLTK_UNSIGNED_INT16,                           //0x10DB
    CLTK_UNSIGNED_INT32,                           //0x10DC
    CLTK_HALF_FLOAT,                               //0x10DD
    CLTK_FLOAT,                                    //0x10DE
    CLTK_UNORM_INT24
} cltk_image_unit;

typedef struct {
    long long           signature;
    cltk_img            mem;
    int32_t                 width;
    int32_t                 height;
    size_t                  pitch;
    size_t                  unit_size;
} cltk_image;

typedef struct {
    int32_t             width;
    int32_t             height;
    int32_t             depth;
    cltk_image_unit     unit_type;
    void                *img_fmt;
} cltk_image_desc;

cltk_context cltk_context_create(void);
void cltk_context_destroy(cltk_context ctx);
cltk_lib cltk_lib_load(cltk_context ctx, char *filename, char *cache_fname, char *buildopt);
cltk_lib cltk_lib_bin_load(cltk_context ctx, char *bin, size_t bin_len);
cltk_lib cltk_lib_str_load(cltk_context ctx, char *src_str, char *cache_fname, char *buildopt);
void cltk_lib_unload(cltk_lib lib);

cltk_func cltk_func_get(cltk_lib lib, char *funcname);
void cltk_func_release(cltk_func func);
void cltk_func_setndr(cltk_func func, int dim, size_t* global_size, size_t *local_size);
void cltk_func_setarg(cltk_func func, int arg_size, void *arg_ptr);
void cltk_func_exec(cltk_func func);
void cltk_func_async_exec(cltk_func func);
void cltk_func_wait(cltk_func func);

cltk_buffer cltk_buffer_alloc(cltk_context ctx, int size);
void cltk_buffer_free(cltk_buffer buf);
void *cltk_buffer_map(cltk_buffer buf);
void cltk_buffer_unmap(cltk_buffer buf);

cltk_image cltk_image_alloc(cltk_context ctx, cltk_image_desc* desc);
void cltk_image_free(cltk_image img);
void *cltk_image_map(cltk_image img, size_t *ret_pitch);
void cltk_image_unmap(cltk_image img);

#ifdef __cplusplus
}
#endif

#endif

#ifndef _CLTK_H_
#define _CLTK_H_

#ifdef __cplusplus
extern "C" {
#endif

#define _ARGS_HANDLE(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _A, _B, _C, _D, _E, _F, N, ...) N

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

#define CLTK_FUNC_CALL(func, dim, gsize, lsize, ...) { \
    cltk_func_setenv(func, dim, gsize, lsize); \
    _ARGS_HANDLE("ignored", ##__VA_ARGS__, \
    _ARG_F, _ARG_E, _ARG_D, _ARG_C, _ARG_B, _ARG_A, _ARG_9, _ARG_8, \
    _ARG_7, _ARG_6, _ARG_5, _ARG_4, _ARG_3, _ARG_2, _ARG_1, _ARG_0)(func, ##__VA_ARGS__) \
    cltk_func_exec(func); \
}

typedef struct _cltk_context*           cltk_context;
typedef struct _cltk_lib*               cltk_lib;
typedef struct _cltk_func*              cltk_func;
typedef struct _cltk_buffer*            cltk_mem;
typedef struct {
    long long           signature;
    cltk_mem            mem;
} cltk_buffer;

cltk_context cltk_context_create(void);
void cltk_context_destroy(cltk_context);
cltk_lib cltk_lib_load(cltk_context, char*, char*);
void cltk_lib_unload(cltk_lib);

cltk_func cltk_func_get(cltk_lib, char*);
void cltk_func_release(cltk_func);
void cltk_func_setenv(cltk_func, int, size_t*, size_t*);
void cltk_func_setarg(cltk_func, int, void*);
void cltk_func_exec(cltk_func);
void cltk_func_async_exec(cltk_func);
void cltk_func_wait(cltk_func);

cltk_buffer cltk_buffer_alloc(cltk_context, int);
void cltk_buffer_free(cltk_buffer);
void *cltk_buffer_map(cltk_buffer);
void cltk_buffer_unmap(cltk_buffer);

#ifdef __cplusplus
}
#endif

#endif

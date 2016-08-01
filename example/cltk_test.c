#include <stdio.h>
#include "cltk.h"

int main(void)
{
    cltk_context ctx = cltk_context_create();
    cltk_buffer buf = cltk_buffer_alloc(ctx, 1024*sizeof(int));
    cltk_lib lib = cltk_lib_load(ctx, "test.cl", "");
    cltk_func func = cltk_func_get(lib, "test");
    size_t gsize[3] = {1024, 0, 0 };

    CLTK_FUNC_CALL(func, 1, gsize, NULL, buf);

    int* hostbuf = cltk_buffer_map(buf);
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

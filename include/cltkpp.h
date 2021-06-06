#ifndef _CLTKPP_H_
#define _CLTKPP_H_

#include <string>
#include <memory>
using namespace std;

#include "cltk.h"

#ifdef CLTKCPP_DEBUG
#include <stdio.h>
#define CLTKCPP_MSG printf
#else
#define CLTKCPP_MSG
#endif

namespace cltk {

class buffer {
    public:
        buffer(cltk_buffer b) {
            buf = b;
            CLTKCPP_MSG("buffer alloc %p\n", buf);
        }
        ~buffer() {
            cltk_buffer_free(buf);
            CLTKCPP_MSG("buffer release %p\n", buf);
        }

        void *map() {
            if(!mapped)
                buf_ptr = cltk_buffer_map(buf);
            return buf_ptr;
        }

        void unmap() {
            if(mapped)
                cltk_buffer_unmap(buf);
            buf_ptr = NULL;
        }

        cltk_buffer& getBuffer() { return buf; }

    private:
        bool mapped = false;
        void *buf_ptr = NULL;
        cltk_buffer buf;
};

class function {
    public:
        function(cltk_func f) {
            func = f;
            CLTKCPP_MSG("func alloc %p\n", func);
        }
        ~function() {
            cltk_func_release(func);
            CLTKCPP_MSG("func release %p\n", func);
        }

        function& NDR(int n, size_t *gs, size_t *ls)
        {
            ndr = n;
            gsize[0] = gs[0];
            gsize[1] = gs[1];
            gsize[2] = gs[2];

            if(ls != NULL){
                lsize[0] = ls[0];
                lsize[1] = ls[1];
                lsize[2] = ls[2];
            }

            cltk_func_setndr(func, ndr, gsize, (ls == NULL ? NULL : lsize));
            return *this;
        }

        void exec(shared_ptr<buffer> &arg) {
            cltk_func_setarg(func, sizeof(arg->getBuffer()), &(arg->getBuffer()));
            cltk_func_exec(func);
        }

        template<typename T>
        void exec(T &arg) {
            cltk_func_setarg(func, sizeof(T), &arg);
            cltk_func_exec(func);
        }

        template<typename... Ts>
        void exec(shared_ptr<buffer> &arg, Ts&&... args) {
            cltk_func_setarg(func, sizeof(arg->getBuffer()), &(arg->getBuffer()));
            exec(std::forward<Ts>(args)...);
        }

        template<typename... Ts, typename T>
        void exec(T &arg, Ts&&... args) {
            cltk_func_setarg(func, sizeof(T), &arg);
            exec(std::forward<Ts>(args)...);
        }

    private:
        cltk_func func;
        int ndr = 1;
        size_t gsize[3] = {1};
        size_t lsize[3] = {1};
        int argIdx = 0;
};

class library {
    public:
        library(cltk_lib l) {
            lib = l;
            CLTKCPP_MSG("lib alloc %p", lib);
        }
        ~library() {
            cltk_lib_unload(lib);
            CLTKCPP_MSG("lib release %p", lib);
        }

        shared_ptr<function> getFunction(string fname){
            cltk_func func = cltk_func_get(lib, (char*)fname.c_str());
            return make_shared<function>(func);
        }

    private:
        cltk_lib lib;
};

class context {
    public:
        context() {
            ctx = cltk_context_create();
            CLTKCPP_MSG("context alloc %p\n", ctx);
        }
        ~context() {
            cltk_context_destroy(ctx);
            CLTKCPP_MSG("context release %p\n", ctx);
        }

        shared_ptr<library> getLibrary(string src, string cache = "", string buildOpt ="") {
            cltk_lib lib = cltk_lib_load(ctx, (char*)src.c_str(), (char*)cache.c_str(), (char*)buildOpt.c_str());
            return make_shared<library>(lib);
        }

        shared_ptr<library> getLibrary(string bin, size_t size){
            cltk_lib lib = cltk_lib_bin_load(ctx, (char*)bin.c_str(), size);
            return make_shared<library>(lib);
        }

        shared_ptr<buffer> allocBuffer(int size){
            cltk_buffer buf = cltk_buffer_alloc(ctx, size);
            return make_shared<buffer>(buf);
        }

    private:
        cltk_context ctx;
};


}

#endif
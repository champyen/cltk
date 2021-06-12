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

class image {
    public:
        image(cltk_image i, cltk_image_desc &idesc, string iname) {
            img = i;
            desc = idesc;
            name = iname;
            CLTKCPP_MSG("image %s alloc %p\n", name.c_str(), img);
        }
        ~image() {
            CLTKCPP_MSG("image %s release %p\n", name.c_str(), img);
            cltk_image_free(img);
        }

        void* map(size_t *retPitch) {
            if(!mapped)
                img_ptr = cltk_image_map(img, retPitch);
            return img_ptr;
        }

        void unmap() {
            if(mapped)
                cltk_image_unmap(img);
            img_ptr = NULL;
        }

        cltk_image& getImage() { return img; }

    private:
        bool mapped = false;
        void *img_ptr = NULL;
        cltk_image img;
        cltk_image_desc desc;
        string name;
};

class buffer {
    public:
        buffer(cltk_buffer b, string bname) {
            buf = b;
            name = bname;
            CLTKCPP_MSG("buffer %s alloc %p\n", name.c_str(), buf);
        }
        ~buffer() {
            CLTKCPP_MSG("buffer %s release %p\n", name.c_str(), buf);
            cltk_buffer_free(buf);
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
        string name;
};

class function {
    public:
        function(cltk_func f, string fname) {
            func = f;
            name = fname;
            CLTKCPP_MSG("func %s alloc %p\n", name.c_str(), func);
        }
        ~function() {
            CLTKCPP_MSG("func %s release %p\n", name.c_str(), func);
            cltk_func_release(func);
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

        void exec(shared_ptr<image> &arg) {
            cltk_func_setarg(func, sizeof(arg->getImage()), &(arg->getImage()));
            cltk_func_exec(func);
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
        void exec(shared_ptr<image> &arg, Ts&&... args) {
            cltk_func_setarg(func, sizeof(arg->getImage()), &(arg->getImage()));
            exec(std::forward<Ts>(args)...);
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
        string name;
};

class library {
    public:
        library(cltk_lib l, string lname) {
            lib = l;
            name = lname;
            CLTKCPP_MSG("lib %s alloc %p\n", name.c_str(), lib);
        }
        ~library() {
            CLTKCPP_MSG("lib %s release %p\n", name.c_str(), lib);
            cltk_lib_unload(lib);
        }

        shared_ptr<function> getFunction(string fname){
            cltk_func func = cltk_func_get(lib, (char*)fname.c_str());
            return make_shared<function>(func, fname);
        }

    private:
        cltk_lib lib;
        string name;
};

class context {
    public:
        context(string cname = "") {
            ctx = cltk_context_create();
            name = cname;
            CLTKCPP_MSG("context %s alloc %p\n", name.c_str(), ctx);
        }
        ~context() {
            CLTKCPP_MSG("context %s release %p\n", name.c_str(), ctx);
            cltk_context_destroy(ctx);
        }

        shared_ptr<library> getLibrary(string src, string cache = "", string buildOpt ="") {
            cltk_lib lib = cltk_lib_load(ctx, (char*)src.c_str(), (char*)cache.c_str(), (char*)buildOpt.c_str());
            return make_shared<library>(lib, src);
        }

        shared_ptr<library> getLibrary(string bin, size_t size){
            cltk_lib lib = cltk_lib_bin_load(ctx, (char*)bin.c_str(), size);
            return make_shared<library>(lib, bin);
        }

        shared_ptr<buffer> allocBuffer(int size, string bname = ""){
            cltk_buffer buf = cltk_buffer_alloc(ctx, size);
            return make_shared<buffer>(buf, bname);
        }

        shared_ptr<image> allocImage(cltk_image_desc &img_desc, string iname = ""){
            cltk_image img = cltk_image_alloc(ctx, &img_desc);
            return make_shared<image>(img, img_desc, iname);
        }

    private:
        cltk_context ctx;
        string name;
};


}

#endif
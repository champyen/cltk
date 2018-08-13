# cltk
CLTK - OpenCL ToolKit

Simply Your OpenCL program.
Glue-Code free OpenCL Programming!

OpenCL provides the power of computing. However, even for a simple example, writing host code is boring and messy.
OpenCL Programming Obstacles:  
1. **Initialization**:  
    There are some lists(platform,device,context,command queue.. etc) must be gotten for setup. This code snippet can be reused. But it will make your code messy.  
2. **Buffer Allocation**:  
    There are some configurations for buffer allocation, but what we always need is the one with better performance.  
3. **Queue Manipulation**:  
    The command queue provides flexibility for device control and manipulation, but it takes effort to understand and make use of asynchronous control model.  
4. **Kernel Execution**:  
    The major problem of kernel execution is argument passing. For each kernel function a snippet glue code is needed for calling clSetKernelArg. Then clEnqueueNDRangeKernel is called after argument setting. The sequences of glue code snippets are similar with small difference of various argument passing. This make writing OpenCL code quite boring.  

CLTK currently focuses on computing with OpenCL 1.x. And it provides reduced model for most application coding scenario. It eliminates writing boring, redundant and messy OpenCL host code. Help OpenCL programmers to devote themself on device code optimization.

The fundamental idea of CLTK is to treat each ".cl" file as a ".so" file. Due to the heterogeneity between CPU and GPU, it is impossible to create seamless API with CPU with OpenCL 1.x. Some abstract objects are created for creating better design.

1. **cltk_context**:  
    the CLTK runtime instance
2. **cltk_library**:  
    .cl library abstract object (similar to a handle returned by dlopen)
3. **cltk_function**:   
    CL function object (as function pointer retured by dlsym)
4. **cltk_buffer/cltk_image**:  
    buffer/image object for kernel function usage.

The main reason of not using OpenCL 2.x is the memory model. (And there are only few runtimes support 2.x) OpenCL 2.x features with Shared Virtual Memory(SVM). But the granularity of SVM support makes programmer difficult to maintain single implementation for all OpenCL platform(or tied to specific platform). IMHO, portibility is an important feature of OpenCL. The original design of OpenCL 1.x provides DSP/FPGA-friendly framework. 


# Building Guide
0. Currently, CLTK is under development. For convenience, you should use Linux for test CLTK.
1. please setup an OpenCL runtime, the below package names are used in ubuntu 
    * package "ocl-icd-libopencl1" "opencl-headers"
        - for x86_64 platform : sudo ln -sf /usr/lib/x86_64-linux-gnu/libOpenCL.so.1 /usr/lib/libOpenCL.so
        - for x86 platform : sudo ln -sf /usr/lib/i386-linux-gnu/libOpenCL.so.1 /usr/lib/libOpenCL.so
    * for AMD GPUs, you should install "mesa-opencl-icd"
        - this package depends on libclc,llvm
        - for better compatibility, newer version is suggested, you can add PPAs of "paulo-miguel-dias/mesa" "paulo-miguel-dias/llvm-snapshot" before your installation.
    * for Intel GPUs, you should install "beignet-opencl-icd"
    * for Nvidia GPUs, since nouveau hasn't support OpenCL yet, you have to install official Nvidia runtime. 
    * for runtime check and information, please install "clinfo" and launch clinfo command in your shell.
3. $ git submodule update --init
   $ make
    * after this libcltk.so can be used
4. $ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./
5. $ ./cltk_test
6. you can see similar output:  
$ ./cltk_test  
000:001 001:002 002:003 003:004 004:005 005:006 006:007 007:008 008:009 009:00A 00A:00B 00B:00C 00C:00D 00D:00E 00E:00F 00F:010  
3F0:3F1 3F1:3F2 3F2:3F3 3F3:3F4 3F4:3F5 3F5:3F6 3F6:3F7 3F7:3F8 3F8:3F9 3F9:3FA 3FA:3FB 3FB:3FC 3FC:3FD 3FD:3FE 3FE:3FF 3FF:400   
ocl_example: CPU log 12M takes 1320953 us  
ocl_example GPU log 12M takes 150645 us  
build log:  
000:001 001:002 002:003 003:004 004:005 005:006 006:007 007:008 008:009 009:00A 00A:00B 00B:00C 00C:00D 00D:00E 00E:00F 00F:010   
3F0:3F1 3F1:3F2 3F2:3F3 3F3:3F4 3F4:3F5 3F5:3F6 3F6:3F7 3F7:3F8 3F8:3F9 3F9:3FA 3FA:3FB 3FB:3FC 3FC:3FD 3FD:3FE 3FE:3FF 3FF:400   
cltk_example CPU log 12M takes 1318090 us  
cltk_example GPU log 12M takes 78607 us  

The demo image of example_image comes from:
https://www.techradar.com/news/7-best-ps-vita-games-the-top-titles-to-grace-sonys-handheld

# Other Resources
If CLTK doesn't meet your requirements, you can check the following project (I got the list from CF4OCL's page)
* [CF4OCL](https://fakenmc.github.io/cf4ocl/ "cf4ocl")
    A well-devloped OpenCL framework. It provides all the power given by OpenCL with better design.
    For using CF4OCL, you still have to understand OpenCL first.
* [Simple OpenCL](https://github.com/morousg/simple-opencl "simple-opencl")
* [OpenCL Utility](https://github.com/Oblomov/CLU "CLU")
* [Computing Language Utility](https://github.com/Computing-Language-Utility/CLU "CLU")
    In Computing Language Utility, it provides glue code generator.
* [OCL-MLA](http://tuxfan.github.io/ocl-mla/ "OCL-MLA")
* [OpenCL kernel extractor](https://github.com/anyc/ocl-ke "ocl-ke")
    It is a good utility for dumpping kernel binary

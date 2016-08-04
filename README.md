# cltk
CLTK - CL ToolKit

Simply Your OpenCL program.
Glue-code free OpenCL Programming!

OpenCL provides the power of computing. However, even for a simple example, writing host code is boring and messy.

1. Initialization: There are some lists(platform,device,context,command queue.. etc) must be gotten for setup. This code snippet can be reused. But it will make your code messy. 
2. Buffer Allocation: There are some configurations for buffer allocation, but what we always need is the one with better performance.
3. Queue Manipulation: The command queue provides flexibility for device control and manipulation, but it takes effort to make use of asynchronous control model. 
4. Kernel Execution: The major problem of kernel execution is argument passing. For each kernel function a snippet glue code is needed for calling clSetKernelArg. Then clEnqueueNDRangeKernel is called after argument setting. The sequences of glue code snippets are similar with small difference of various argument passing. This make writing OpenCL code quite boring. 

CLTK currently focuses on computing with OpenCL 1.x. And it provides reduced model for most application coding scenario. It eliminates writing boring, redundant and messy OpenCL host code. Help OpenCL programmers to devote themself on device code optimization.

The fundamental idea of CLTK is to treat each ".cl" file as a ".so" file. Due to the heterogeneity between CPU and GPU, it is impossible to create seamless API with CPU with OpenCL 1.x. Some abstract objects are created for creating better design.

1. cltk_context - the CLTK instance
2. cltk_library - .cl file runtime object (similar to a handle returned by dlopen)
3. cltk_function - CL function object (as function pointer retured by dlsym)
4. cltk_buffer/cltk_image - buffer/image object for kernel function usage.

The main reason of not using OpenCL 2.x is the memory model. (And there are only few runtimes support 2.x) OpenCL 2.x features with Shared Virtual Memory(SVM). But the granularity of SVM support makes programmer difficult to maintain single implementation for all OpenCL platform(or tied to specific platform). IMHO, portibility is an important feature of OpenCL. The original design of OpenCL 1.x provides DSP/FPGA-friendly framework. 

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

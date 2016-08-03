# cltk
CLTK - CL ToolKit

Simply Your OpenCL program.
glue-code free OpenCL Programming!

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
4. cltk_buffer/cltk_image - buffer object for function target.

The main reason of not using OpenCL 2.x is the memory model. (And there are only few runtimes support 2.x) OpenCL 2.x features with Shared Virtual Memory(SVM). But the granularity of SVM support makes programmer difficult to maintain single implementation for all OpenCL platform(or tied to specific platform). IMHO, portibility is an important feature of OpenCL. The original design of OpenCL 1.x provides DSP/FPGA-friendly framework. 

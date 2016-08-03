# cltk
CLTK - CL ToolKit

Simply Your OpenCL program.
glue-code free OpenCL Programming!

OpenCL provides the power of computing. However, even for a simple example, writing host code is boring and messy.

1. initialization
    There are some lists(platform,device,context,command queue.. etc) must be gotten for setup. This code snippet can be reused. But it will make your code messy. 
2. buffer allocation
    There are some configurations for buffer allocation, but what we always need is the one with better performance.
3. queue manipulation
    The command queue provides flexibility for device manipulation, but it takes effort to make use of asynchronous control model. 
4. kernel execution
    The major problem of kernel execution is argument passing. For each kernel function a snippet glue code is needed for calling clSetKernelArg. Then clEnqueueNDRangeKernel is called after argument setting. The sequences of glue code snippets are similar with small difference of various argument passing. This make writing OpenCL code quite boring. 
    
CLTK currently focuses on GPU computing. And it provides reduced model for most application coding scenario. It eliminates writing boring, redundant and messy OpenCL host code. Help OpenCL programmers to devote themself on device code optimization.

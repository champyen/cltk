const sampler_t sampler_warp = CLK_NORMALIZED_COORDS_TRUE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_LINEAR;

__kernel void warp(
    __read_only image2d_t input,
    __global uchar* buf,
    __global float* map
){
    int gidx = get_global_id(0);
    int gidy = get_global_id(1);

    float fx = (float)gidx/(float)get_global_size(0);
    float fy = (float)gidy/(float)get_global_size(1);
    float3 frgb;

    #if 0

    float4 ratio = (float4)((1-fx)*(1-fy), fx*(1-fy), (1-fx)*fy, fx*fy);
    float4 pos_x = (float4)(map[0], map[2], map[4], map[6]);
    float4 pos_y = (float4)(map[1], map[3], map[5], map[7]);
    float2 pos_dst = (float2)(dot(ratio, pos_x), dot(ratio, pos_y));

    #else
    //Perspective Warping
    float3 M1 = (float3)(map[0], map[1], map[2]);
    float3 M2 = (float3)(map[3], map[4], map[5]);
    float3 M3 = (float3)(map[6], map[7], map[8]);

    float3 pos_src = (float3)(fx, fy, 1.0);
    float div_factor = dot(M3, pos_src);
    float2 pos_dst = (float2)(dot(M1, pos_src)/div_factor, dot(M2, pos_src)/div_factor);

    #endif

    frgb = read_imagef(input, sampler_warp, pos_dst).s012;
    frgb *= 255.f;

    vstore3(convert_uchar3(frgb), 0, buf + ((gidy * get_global_size(0)) + gidx)*3 );
}

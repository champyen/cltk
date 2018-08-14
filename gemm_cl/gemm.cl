__kernel void sgemm_buf_img(
   __global const float *A,
   const int lda,
   __global float *C,
   const int ldc,
   const int m,
   const int n,
   const int k,
   __read_only image2d_t Bi
){
    int gx = get_global_id(0) << 2;
    int gy = get_global_id(1) << 3;

    if ((gx < n) && (gy < m)){
        float4 a[8];
        float4 b[4];
        float4 c[8];

        for (int i = 0; i < 8; i++){
            c[i] = 0.0f;
        }
        A += gy * lda;

        for (int pos = 0; pos < k; pos += 4){
            for (int i = 0; i < 4; i++){
                b[i] = read_imagef(Bi, (int2)(gx >> 2, pos + i));
            }

            for (int i = 0; i < 8; i++){
                a[i] = vload4(0, A + mul24(i, lda) + pos);
                c[i] += a[i].x * b[0] + a[i].y * b[1] + a[i].z * b[2] + a[i].w * b[3];
            }

        }

        for (int i = 0; i < 8; i++){
            int C_offs = mul24((gy + i), ldc) + gx;
            vstore4(c[i], 0, C + C_offs);
        }
    }
}



__kernel void sgemm_buf_only(
   __global const float *A,
   const int lda,
   __global float *C,
   const int ldc,
   const int m,
   const int n,
   const int k,
   __global const float *B
){

    int gx = get_global_id(0) << 2;
    int gy = get_global_id(1) << 3;

    if ((gx < n) && (gy < m)){
        float4 a[8];
        float4 b[4];
        float4 c[8];

        for (int i = 0; i < 8; i++){
            c[i] = 0.0f;
        }
        A += gy * lda;

        for (int pos = 0; pos < k; pos += 4){
            for (int i = 0; i < 4; i++){
                //b[i] = read_imagef(Bi, (int2)(gx >> 2, pos + i));
                b[i] = vload4(0, B + (pos + i)*ldc + gx);
            }

            for (int i = 0; i < 8; i++){
                a[i] = vload4(0, A + mul24(i, lda) + pos);
                c[i] += a[i].x * b[0] + a[i].y * b[1] + a[i].z * b[2] + a[i].w * b[3];
            }

        }

        for (int i = 0; i < 8; i++){
            int C_offs = mul24((gy + i), ldc) + gx;
            vstore4(c[i], 0, C + C_offs);
        }
    }
}

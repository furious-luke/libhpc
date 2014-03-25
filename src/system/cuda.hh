#ifndef libhpc_system_cuda_hh
#define libhpc_system_cuda_hh

#ifdef __CUDACC__

#define CUDA_DEVICE_HOST        __device__ __host__
#define CUDA_DEVICE_HOST_INLINE __device__ __host__ __forceinline__

#else

#define CUDA_DEVICE_HOST
#define CUDA_DEVICE_HOST_INLINE inline

#endif

#endif
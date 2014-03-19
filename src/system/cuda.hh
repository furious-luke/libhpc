#ifndef libhpc_system_cuda_hh
#define libhpc_system_cuda_hh

#ifdef __CUDACC__

#define CUDA_DEVICE_HOST __device__ __host__

#else

#define CUDA_DEVICE_HOST

#endif

#endif

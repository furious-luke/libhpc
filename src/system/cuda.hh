// Copyright 2012 Luke Hodkinson

// This file is part of libhpc.
// 
// libhpc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// libhpc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with libhpc.  If not, see <http://www.gnu.org/licenses/>.

#ifndef libhpc_system_cuda_hh
#define libhpc_system_cuda_hh

#ifdef __CUDACC__

#define CUDA_DEV          __device__
#define CUDA_DEV_HOST     __device__ __host__
#define CUDA_DEV_HOST_INL __device__ __host__ __forceinline__
#define CUDA_GLO_HOST     __global__ __host__

#else

#define CUDA_DEV
#define CUDA_DEV_HOST
#define CUDA_DEV_HOST_INL inline
#define CUDA_GLO_HOST

#endif

#endif

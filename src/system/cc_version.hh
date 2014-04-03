#ifndef libhpc_system_cc_version_hh
#define libhpc_system_cc_version_hh

#ifdef CXX_0X
#undef CXX_0X
#endif
#if (__cplusplus >= 199711L) && !defined(__NVCC__)
#define CXX_0X 1
#endif

#endif

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

#ifndef libhpc_unit_test_test_case_cuda_hh
#define libhpc_unit_test_test_case_cuda_hh

#ifdef __NVCC__

#include "test_case.hh"

#define TEST_CASE_CUDA( name )                                  \
   CUDA_DEV_HOST                                                \
   void UNIQUE_LINE( __hpc_test_case__ )();                     \
   ::hpc::test::test_case<> ANON(                               \
      name, "", UNIQUE_LINE( __hpc_test_case__ ) );             \
   ::hpc::test::test_case<> ANON(                               \
      name"/cuda", "", UNIQUE_LINE( __hpc_test_case__ ) );      \
   CUDA_DEV_HOST                                                \
   void UNIQUE_LINE( __hpc_test_case__ )()

#else

#define TEST_CASE_CUDA( name )                  \
   TEST_CASE( name )

#endif



#endif

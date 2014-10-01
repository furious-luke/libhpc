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

#ifndef hpc_unit_test_test_case_hh
#define hpc_unit_test_test_case_hh

#include <string>
#include <vector>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
#include "result_buffer.hh"

#define SUITE_PREFIX( pre )                     \
   hpc::test::set_suite_prefix ANON( pre )

#define TEST_CASE( name )                               \
   void UNIQUE_LINE( __hpc_test_case__ )(               \
      hpc::test::result_buffer<>& rb );                 \
   hpc::test::test_case ANON(                           \
      name, "", UNIQUE_LINE( __hpc_test_case__ ) );     \
   void UNIQUE_LINE( __hpc_test_case__ )(               \
      hpc::test::result_buffer<>& rb )

#ifdef __CUDACC__

#define TEST_CASE_CUDA( name )                                          \
   CUDA_DEV_HOST                                                        \
   void UNIQUE_LINE( __hpc_test_case__ )(                               \
      hpc::test::result_buffer<>& rb );                                   \
   hpc::test::test_case ANON(                                           \
      name, "", UNIQUE_LINE( __hpc_test_case__ ) );                     \
   __global__                                                           \
   void UNIQUE_LINE( __hpc_cuda_global__ )(                             \
      hpc::test::result_buffer<> rb )                                     \
   {                                                                    \
      UNIQUE_LINE( __hpc_test_case__ )( rb );                           \
   }                                                                    \
   void UNIQUE_LINE( __hpc_cuda_wrap__ )(                               \
      hpc::test::result_buffer<>& rb )                                    \
   {                                                                    \
      char* dev_buf;                                                    \
      cudaMalloc( &dev_buf, 100000 );                                   \
      hpc::test::result_buffer<> dev_rb( dev_buf );                       \
      UNIQUE_LINE( __hpc_cuda_global__ )<<<1,1>>>( dev_buf );           \
      cudaDeviceSynchronize();                                          \
      cudaError_t ec = cudaGetLastError();                              \
      if( ec != cudaSuccess )                                           \
      {                                                                 \
         rb.buffer()[0] = 2;                                            \
         rb.buffer()[1] = 0;                                            \
         std::cout << cudaGetErrorString( ec ) << "\n";                 \
      }                                                                 \
      else                                                              \
      {                                                                 \
         cudaMemcpy( rb.buffer(), dev_buf, 100000,                      \
                     cudaMemcpyDeviceToHost );                          \
      }                                                                 \
   }                                                                    \
   hpc::test::test_case ANON(                                           \
      name"/cuda", "", UNIQUE_LINE( __hpc_cuda_wrap__ ) );              \
   CUDA_DEV_HOST                                                        \
   void UNIQUE_LINE( __hpc_test_case__ )(                               \
      hpc::test::result_buffer<>& rb )

#else // __CUDACC__

#define TEST_CASE_CUDA( name )                  \
   TEST_CASE( name )

#endif // __CUDACC__

namespace hpc {
   namespace test {

      class runner;

      class test_case_base;

      struct test_case_node_t
      {
         test_case_base* tc;
         test_case_node_t* next;
      };

      class test_case_base
      {
      public:

         test_case_base( const std::string& name,
                         const std::string& desc );

         virtual
         void
         run() = 0;

         std::string const&
         name() const;

         std::string const&
         description() const;

         void
         set_runner( test::runner const* runner );

         test::runner const*
         runner() const;

         operator bool() const
         {
            return _succ;
         }

         virtual
         void
         print_results() const = 0;

         bool
         is_parallel() const;

         std::vector<int> const&
         ranks() const;

      protected:

         void
         _add_test_case();

      protected:

         std::string _name;
         std::string _desc;
         test::runner const* _runner;
         bool _succ;
         std::vector<int> _ranks;
      };

      class test_case
         : public test_case_base
      {
      public:

         test_case( const std::string& name,
                    const std::string& desc,
                    boost::function<void(result_buffer<>&)> func );

         virtual
         void
         run();

         virtual
         void
         print_results() const;

      protected:

         boost::function<void(result_buffer<>&)> _func;
         result_buffer<> _rb;
         std::vector<char> _buf;
      };

      class set_suite_prefix
      {
      public:

         set_suite_prefix( char const* pre );
      };

   }
}

#endif

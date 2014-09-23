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

#ifndef libhpc_mpi_logger_hh
#define libhpc_mpi_logger_hh

#ifndef NLOG

#include <typeinfo>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <mpi.h>
#include "libhpc/logging.hh"

#define LOG_MPI( base )				\
   LOG_PUSH( new hpc::mpi::logger( base, 0 ) )

namespace hpc {
   namespace mpi {

      extern double log_base_time;

      ///
      ///
      ///
      class logger
         : public log::file
      {
      public:

         logger( const std::string& filename,
		 unsigned level = 0 );

         virtual
         ~logger();

         virtual void
         open();

         virtual void
         prefix();

      protected:

         int _my_rank;
         std::string _base;
      };
   }
}

#else

#define LOG_MPI( base )

#endif

#endif

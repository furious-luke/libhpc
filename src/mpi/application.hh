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

#ifndef libhpc_mpi_application_hh
#define libhpc_mpi_application_hh

#include "libhpc/main/application.hh"

namespace hpc {
   namespace mpi {

      class application
         : public hpc::application
      {
      public:

         application( int argc,
                      char* argv[],
                      std::string const& info = "" );

         virtual
         ~application();

         int
         rank() const;

         int
         size() const;

      public:

         int _rank;
         int _size;
      };

   }
}

#endif

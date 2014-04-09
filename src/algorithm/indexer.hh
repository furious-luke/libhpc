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

#ifndef libhpc_algorithm_indexer_hh
#define libhpc_algorithm_indexer_hh

#include "libhpc/debug/assert.hh"
#include "libhpc/logging.hh"
#include "libhpc/mpi/comm.hh"

namespace hpc {
   namespace algorithm {

      class indexer
      {
      public:

         indexer( int tag,
                  const mpi::comm& comm = mpi::comm::world );

         void
         set_comm( const mpi::comm& comm );

         void
         set_max_its( unsigned its );

         unsigned long long
         request( unsigned long long size,
                  std::function<void(unsigned long long,unsigned long long)> cb = std::function<void(unsigned long long,unsigned long long)>() );

	 void
	 master( std::function<void(unsigned long long,unsigned long long)> cb = std::function<void(unsigned long long,unsigned long long)>() );

	 unsigned long long
	 base() const;

      protected:

         unsigned long long _base;
         unsigned _max_its;
         int _tag;
         mpi::comm const* _comm;
      };

   }
}

#endif

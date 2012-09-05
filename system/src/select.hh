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

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <boost/iterator/iterator_adaptor.hpp>
#include "libhpc/containers/list.hh"
#include "types.hh"
#include "pipe.hh"

namespace hpc {
   namespace unix {

      class select
      {
      public:

         select();

         void
         add_read( const pipe& pipe );

         void
         add_write( const pipe& pipe );

         void
         wait();

         bool
         can_read( const pipe& pipe );

         bool
         can_write( const pipe& pipe );

      protected:

         unsigned _max;
         fd_set _read, _write;
      };
   }
}

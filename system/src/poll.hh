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

#ifndef libhpc_system_poll_hh
#define libhpc_system_poll_hh

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>
#include <boost/iterator/iterator_adaptor.hpp>
#include "libhpc/containers/list.hh"
#include "types.hh"
#include "pipe.hh"

namespace hpc {
   namespace posix {

      class poll
      {
      public:

         typedef struct pollfd pollfd_type;

      public:

         poll( unsigned size=0 );

         void
         reserve( unsigned size );

         unsigned
         add( const pollfd_type& pollfd );

         unsigned
         add_read( const pipe& pipe );

         unsigned
         add_write( const pipe& pipe );

         void
         pop();

         template< class Iterator >
         void
         add( Iterator start,
              const Iterator& finish )
         {
            while( start != finish )
               add( *start++ );
         }

         void
         wait();

         bool
         can_read( unsigned idx ) const;

         bool
         can_write( unsigned idx ) const;

         unsigned
         size() const;

         vector<pollfd_type>&
         pollfds();

      protected:

         vector<pollfd_type> _pollfds;
      };
   }
}

#endif

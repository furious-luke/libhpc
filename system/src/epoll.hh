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

#ifndef libhpc_system_epoll_hh
#define libhpc_system_epoll_hh

#include <boost/iterator/iterator_adaptor.hpp>
#include <sys/epoll.h>
#include "libhpc/containers/list.hh"
#include "libhpc/containers/optional.hh"
#include "types.hh"
#include "pipe.hh"

namespace hpc {
   namespace posix {

      class epoll
         : public pipe
      {
      public:

         struct event_type
            : public epoll_event
         {
            int
            fd() const;

            void*
            data_ptr() const;

            uint32
            data_uint32() const;

            bool
            ready_in() const;

            bool
            ready_out() const;

            bool
            error() const;

            bool
            hangup() const;
         };

         typedef class epoll_event_iterator iterator;

      public:

         epoll();

         epoll( const pipe& pipe,
                uint32_t events );

         void
         open();

         void
         add( const pipe& pipe,
              uint32_t events,
              epoll_data_t data );

         void
         add( const pipe& pipe,
              uint32_t events );

         void
         add( const pipe& pipe,
              uint32_t events,
              void* data );

         void
         add( const pipe& pipe,
              uint32_t events,
              uint32 data );

         void
         remove( const pipe& pipe );

         void
         modify( const pipe& pipe,
                 uint32_t events,
                 epoll_data_t data );

         void
         modify( const pipe& pipe,
                 uint32_t events );

         void
         modify( const pipe& pipe,
                 uint32_t events,
                 void* data );

         void
         modify( const pipe& pipe,
                 uint32_t events,
                 uint32 data );

         void
         wait( vector<event_type>& events,
               int timeout = -1 );

      protected:

         unsigned _size;
         OMP_LOCK( _lock );
      };

   }
}

#endif

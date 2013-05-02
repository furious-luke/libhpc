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

         typedef struct epoll_event event_type;
         typedef class epoll_event_iterator iterator;

         static const size_t default_max_events;

      public:

         epoll();

         epoll( const pipe& pipe,
                uint32_t events,
                size_t max_events = default_max_events );

         void
         open( size_t max_events = default_max_events );

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
         wait( int timeout = -1 );

         iterator
         begin() const;

         iterator
         end() const;

      protected:

         unsigned _size;
         vector<event_type> _events;
      };

      ///
      ///
      ///
      class epoll_event_iterator
         : public boost::iterator_adaptor< epoll_event_iterator,
                                           vector<epoll::event_type>::const_iterator,
                                           const epoll::event_type&,
                                           std::forward_iterator_tag,
                                           const epoll::event_type& >
      {
         friend class boost::iterator_core_access;

      public:

         epoll_event_iterator( vector<epoll::event_type>::const_iterator it );

         int
         fd() const;

         void*
         data() const;

         bool
         ready_in() const;

         bool
         ready_out() const;

         bool
         error() const;

         bool
         hangup() const;
      };
   }
}

#endif

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

#include <boost/iterator/iterator_adaptor.hpp>
#include <sys/epoll.h>
#include "libhpc/containers/list.hh"
#include "pipe.hh"

namespace hpc {
   namespace unix {

      class epoll
         : public pipe
      {
      public:

         typedef struct epoll_event event_type;
         typedef class epoll_event_iterator iterator;

      public:

         epoll();

         epoll( const pipe& pipe );

         void
         open();

         void
         add( const pipe& pipe );

         void
         remove( const pipe& pipe );

         void
         wait( int timeout=-1 );

         iterator
         begin() const;

         iterator
         end() const;

      protected:

         unsigned _size;
         unsigned _max_events;
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

         bool
         ready() const;

         bool
         error() const;

         bool
         hangup() const;

      protected:

         
      };
   }
}

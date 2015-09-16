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

#ifndef hpc_system_epoll_hh
#define hpc_system_epoll_hh

#ifndef DARWIN

#include <vector>
#include <stdint.h>
#include <sys/epoll.h>
#include "file_descriptor.hh"

namespace hpc {
   namespace os {

      class epoll
         : public file_descriptor
      {
      public:

         struct event_type
            : public epoll_event
         {
	    int
            fd() const;

            void*
            data_ptr() const;

            uint32_t
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

      public:

         epoll();

         epoll( const file_descriptor& fd,
                uint32_t events );

         void
         open();

         void
         add( const file_descriptor& fd,
              uint32_t events,
              epoll_data_t data );

	 void
	 add( int fd,
	      uint32_t events,
	      epoll_data_t data );

         void
         add( const file_descriptor& fd,
              uint32_t events );

         void
         add( const file_descriptor& fd,
              uint32_t events,
              void* data );

         void
         add( const file_descriptor& fd,
              uint32_t events,
              uint32_t data );

         void
         add( int fd,
              uint32_t events,
              uint32_t data );

         void
         remove( const file_descriptor& fd );

         void
         remove( int fd );

         void
         modify( const file_descriptor& fd,
                 uint32_t events,
                 epoll_data_t data );

	 void
	 modify( int fd,
		 uint32_t events,
		 epoll_data_t data );

         void
         modify( const file_descriptor& fd,
                 uint32_t events );

         void
         modify( const file_descriptor& fd,
                 uint32_t events,
                 void* data );

         void
         modify( const file_descriptor& fd,
                 uint32_t events,
                 uint32_t data );

         void
         modify( int fd,
                 uint32_t events,
                 uint32_t data );

         unsigned
         wait( std::vector<event_type>& events,
               int timeout = -1 );
      };

   }
}

#endif

#endif

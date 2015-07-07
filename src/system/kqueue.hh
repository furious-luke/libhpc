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

#ifndef hpc_system_kqueue_hh
#define hpc_system_kqueue_hh

#ifdef DARWIN

#include <sys/types.h>
#include <sys/event.h>
#include "file_descriptor.hh"

namespace hpc {
   namespace os {

      class kqueue
         : public file_descriptor
      {
      public:

         struct event_type
            : public kevent64_s
         {
            int
            fd() const;

            uint32_t
            data_uint32() const;

            bool
            ready_in() const;

            bool
            ready_out() const;

            bool
            hangup() const;
         };

      public:

         kqueue();

         void
         open();

         ///
         /// @filters[in]  EVFILT_READ, EVFILT_WRITE, ...
         ///
         void
         add( file_descriptor const& fd,
              int16_t filter,
	      uint32_t fflags = 0,
              uint32_t data = 0,
              bool oneshot = false,
              bool clear = false );

         void
         add( process const& proc,
	      uint32_t fflags,
              uint32_t data = 0,
              bool oneshot = false,
              bool clear = false );

         void
         add( int fd,
              int16_t filter,
	      uint32_t fflags = 0,
              uint32_t data = ,
              bool oneshot = false,
              bool clear = false );

         void
         remove( file_descriptor const& fd,
                 int16_t filter );

         void
         remove( int fd,
                 int16_t filter );

         unsigned
         wait( std::vector<event_type>& events,
               int timeout = -1 );
      };

   }
}

#endif

#endif

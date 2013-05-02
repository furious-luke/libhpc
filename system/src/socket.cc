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

#include <sys/socket.h>
#include "libhpc/logging/logging.hh"
#include "socket.hh"

namespace hpc {
   namespace posix {

      void
      socket::open( int family,
                    int type,
                    int protocol )
      {
         LOG_ENTER();

         close();
         _fd = ::socket( family, type, protocol );
         ASSERT( _fd >= 0 );

         // Set an option to allow for rapid reuse of the port.
         int optval = 1;
         INSIST( ::setsockopt( _fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval) ), == 0 );

         LOG_EXIT();
      }

      void
      socket::listen( unsigned backlog )
      {
         INSIST( ::listen( _fd, backlog ), >= 0 );
      }
   }
}

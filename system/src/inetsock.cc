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
#include <arpa/inet.h>
#include <errno.h>
#include "libhpc/logging/logging.hh"
#include "inetsock.hh"

namespace hpc {
   namespace unix {

      inetsock::inetsock()
         : socket()
      {
      }

      inetsock::inetsock( const string& ip_addr,
                          uint16 port )
         : socket()
      {
         open();
         bind( ip_addr, port );
      }

      void
      inetsock::open()
      {
         socket::open( AF_INET, SOCK_STREAM );
      }

      void
      inetsock::bind( const string& ip_addr,
                      uint16 port )
      {
         LOG_ENTER();

         uint32 ip;
         if( !ip_addr.empty() )
         {
            ip = inet_addr( ip_addr.c_str() );
            ASSERT( ip != INADDR_NONE );
         }
         else
            ip = INADDR_ANY;

         // Prepare the address.
         struct sockaddr_in addr;
         bzero( (char*)&addr, sizeof(addr) );
         addr.sin_family = AF_INET;
         addr.sin_addr.s_addr = ip;
         addr.sin_port = htons( port );

         // Call and handle the bind function.
         int ec = ::bind( _fd, (struct sockaddr*)&addr, sizeof(addr) );
         if( ec < 0 )
         {
            HNDERR( errno == EADDRINUSE, error::address_in_use , err0 );
            SETERR( debug::error::unknown, err0 );
         }
         ERROK();

        err0:
         LOG_EXIT();
      }

      void
      inetsock::accept( inetsock& client )
      {
         LOG_ENTER();

         struct sockaddr_in addr;
         socklen_t addr_len = sizeof(addr);
         int cfd = ::accept( _fd, (struct sockaddr*)&addr, &addr_len );
         ASSERT( cfd >= 0 );
         LOGLN( "Accepted connection." );

         // Pass to the client.
         client.set_fd( cfd );

         LOG_EXIT();
      }
   }
}

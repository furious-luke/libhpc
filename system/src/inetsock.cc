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
#include <netdb.h>
#include <errno.h>
#include "libhpc/debug/error.hh"
#include "libhpc/logging/logging.hh"
#include "inetsock.hh"

namespace hpc {
   namespace posix {

      inetsock::inetsock( kind_type kind )
         : socket(),
           _kind( kind )
      {
      }

      inetsock::inetsock( const string& ip_addr,
                          uint16 port )
         : socket()
      {
         open();
         bind( port, ip_addr );
      }

      void
      inetsock::open()
      {
         socket::open( AF_INET, SOCK_STREAM );
      }

      void
      inetsock::bind( uint16 port,
                      const string& ip_addr )
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
            HNDERR( errno == EADDRINUSE, ADDRESS_IN_USE, err0 );
            SETERR( error::UNKNOWN, err0 );
         }
         ERROK();

         // Mark this socket as a server.
         _kind = SERVER;

        err0:
         LOG_EXIT();
      }

      void
      inetsock::connect( const string& ip_addr,
                         uint16 port )
      {
         LOG_ENTER();

         // Must have an IP address to connect to.
         ASSERT( !ip_addr.empty() );

         // Convert the IP address (if needed).
         struct hostent* serv = gethostbyname( ip_addr.c_str() );
         ASSERT( serv );

         // Prepare the address.
         struct sockaddr_in addr;
         bzero( (char*)&addr, sizeof(addr) );
         addr.sin_family = AF_INET;
         bcopy( (char*)serv->h_addr, (char*)&addr.sin_addr.s_addr, serv->h_length );
         addr.sin_port = htons( port );

         // Call and handle the connect function.
         int ec = ::connect( _fd, (struct sockaddr*)&addr, sizeof(addr) );
         if( ec < 0 )
         {
            SETERR( error::UNKNOWN, err0 );
         }
         ERROK();

        err0:
         LOG_EXIT();
      }

      bool
      inetsock::accept( inetsock& client )
      {
         LOG_ENTER();

         struct sockaddr_in addr;
         socklen_t addr_len = sizeof(addr);
         int cfd = ::accept( _fd, (struct sockaddr*)&addr, &addr_len );
         if( cfd < 0 && errno == EAGAIN )
         {
            LOGLN( "No pending connections." );
            LOG_EXIT();
            return false;
         }
         ASSERT( cfd >= 0 );
         LOGLN( "Accepted connection." );

         // Pass to the client.
         client.set_fd( cfd );

         LOG_EXIT();
         return true;
      }

      inetsock::kind_type
      inetsock::kind() const
      {
         return _kind;
      }

   }
}

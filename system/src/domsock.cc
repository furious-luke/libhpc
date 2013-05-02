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

#include <sys/types.h>
#include <sys/socket.h>
#include "libhpc/logging/logging.hh"
#include "domsock.hh"

namespace hpc {
   namespace posix {

      void
      domsock::open()
      {
         socket::open( PF_UNIX, SOCK_STREAM );
      }

      void
      domsock::bind( const string& path )
      {
         LOG_ENTER();

         struct sockaddr_un addr;
         ASSERT( path.length() <= sizeof(addr.sun_path) );
         bzero( (char*)&addr, sizeof(addr) );
         addr.sun_family = AF_UNIX;
         strcpy( addr.sun_path, path.c_str() );
         INSIST( ::bind( _fd, (struct sockaddr*)&addr, sizeof(addr) ), >= 0 );

         LOG_EXIT();
      }

      void
      domsock::connect( const string& path )
      {
         LOG_ENTER();

         struct sockaddr_un addr;
         ASSERT( path.length() <= sizeof(addr.sun_path) );
         bzero( (char*)&addr, sizeof(addr) );
         addr.sun_family = AF_UNIX;
         strcpy( addr.sun_path, path.c_str() );
         INSIST( ::connect( _fd, (struct sockaddr*)&addr, sizeof(addr) ), >= 0 );

         LOG_EXIT();
      }

      void
      domsock::accept( domsock& client )
      {
         LOG_ENTER();

         struct sockaddr_un addr;
         socklen_t addr_len = sizeof(addr);
         int cfd = ::accept( _fd, (struct sockaddr*)&addr, &addr_len );
         ASSERT( cfd >= 0 );
         LOGLN( "Accepted connection." );

         // Pass to the client.
         client.set_fd( cfd );

         LOG_EXIT();
      }

      void
      domsock::send_fd( int fd )
      {
         struct msghdr socket_message;
         struct iovec io_vector[1];
         struct cmsghdr* control_message = NULL;
         char message_buffer[1];

         // storage space needed for an ancillary element with a paylod of length is CMSG_SPACE(sizeof(length))
         char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
         int available_ancillary_element_buffer_space;

         // at least one vector of one byte must be sent
         message_buffer[0] = 'F';
         io_vector[0].iov_base = message_buffer;
         io_vector[0].iov_len = 1;

         // initialize socket message
         memset( &socket_message, 0, sizeof(struct msghdr) );
         socket_message.msg_iov = io_vector;
         socket_message.msg_iovlen = 1;

         // provide space for the ancillary data
         available_ancillary_element_buffer_space = CMSG_SPACE(sizeof(int));
         memset( ancillary_element_buffer, 0, available_ancillary_element_buffer_space );
         socket_message.msg_control = ancillary_element_buffer;
         socket_message.msg_controllen = available_ancillary_element_buffer_space;

         /* initialize a single ancillary data element for fd passing */
         control_message = CMSG_FIRSTHDR(&socket_message);
         control_message->cmsg_level = SOL_SOCKET;
         control_message->cmsg_type = SCM_RIGHTS;
         control_message->cmsg_len = CMSG_LEN(sizeof(int));
         *((int*)CMSG_DATA(control_message)) = fd;

         INSIST( sendmsg( _fd, &socket_message, 0 ), > 0 );
      }

      int
      domsock::recv_fd()
      {
         int fd, available_ancillary_element_buffer_space;
         struct msghdr socket_message;
         struct iovec io_vector[1];
         struct cmsghdr* control_message = NULL;
         char message_buffer[1];
         char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];

         // start clean
         memset( &socket_message, 0, sizeof(struct msghdr) );
         memset( ancillary_element_buffer, 0, CMSG_SPACE(sizeof(int)) );

         // setup a place to fill in message contents
         io_vector[0].iov_base = message_buffer;
         io_vector[0].iov_len = 1;
         socket_message.msg_iov = io_vector;
         socket_message.msg_iovlen = 1;

         // provide space for the ancillary data
         socket_message.msg_control = ancillary_element_buffer;
         socket_message.msg_controllen = CMSG_SPACE(sizeof(int));

         INSIST( recvmsg( _fd, &socket_message, MSG_CMSG_CLOEXEC ), >= 0 );
         ASSERT( message_buffer[0] == 'F' );
         ASSERT( (socket_message.msg_flags & MSG_CTRUNC) != MSG_CTRUNC );

         // iterate ancillary elements
         for( control_message = CMSG_FIRSTHDR(&socket_message);
              control_message != NULL;
              control_message = CMSG_NXTHDR(&socket_message, control_message) )
         {
            if( (control_message->cmsg_level == SOL_SOCKET) &&
                (control_message->cmsg_type == SCM_RIGHTS) )
            {
               fd = *((int*)CMSG_DATA(control_message));
               return fd;
            }
         }
         ASSERT( 0 );
      }
   }
}

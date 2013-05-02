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

#include "poll.hh"

namespace hpc {
   namespace posix {

      poll::poll( unsigned size )
      {
         reserve( size );
      }

      void
      poll::reserve( unsigned size )
      {
         _pollfds.reallocate( size );
         _pollfds.resize( 0 );
      }

      unsigned
      poll::add( const pollfd_type& pollfd )
      {
         _pollfds.push_back( pollfd );
         return _pollfds.size() - 1;
      }

      unsigned
      poll::add_read( const pipe& pipe )
      {
         _pollfds.resize( _pollfds.size() + 1 );
         _pollfds.back().fd = pipe.fd();
         _pollfds.back().events = POLLIN;
         _pollfds.back().revents = 0;
         return _pollfds.size() - 1;
      }

      unsigned
      poll::add_write( const pipe& pipe )
      {
         _pollfds.resize( _pollfds.size() + 1 );
         _pollfds.back().fd = pipe.fd();
         _pollfds.back().events = POLLOUT;
         _pollfds.back().revents = 0;
         return _pollfds.size() - 1;
      }

      void
      poll::pop()
      {
         ASSERT( _pollfds.size() );
         _pollfds.resize( _pollfds.size() - 1 );
      }

      void
      poll::wait()
      {
         int res = ::poll( _pollfds.data(), _pollfds.size(), 0 );
      }

      bool
      poll::can_read( unsigned idx ) const
      {
         return _pollfds[idx].revents & POLLIN;
      }

      bool
      poll::can_write( unsigned idx ) const
      {
         return _pollfds[idx].revents & POLLOUT;
      }

      unsigned
      poll::size() const
      {
         return _pollfds.size();
      }

      vector<poll::pollfd_type>&
      poll::pollfds()
      {
         return _pollfds;
      }
   }
}

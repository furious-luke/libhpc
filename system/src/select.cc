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

#include "select.hh"

namespace hpc {
   namespace unix {

      select::select()
         : _max( 0 )
      {
         FD_ZERO( &_read );
         FD_ZERO( &_write );
      }

      void
      select::add_read( const pipe& pipe )
      {
         ASSERT( pipe.fd() >= 0 );
         FD_SET( pipe.fd(), &_read );
         _max = std::max<int>( pipe.fd(), _max );
      }

      void
      select::add_write( const pipe& pipe )
      {
         ASSERT( pipe.fd() >= 0 );
         FD_SET( pipe.fd(), &_write );
         _max = std::max<int>( pipe.fd(), _max );
      }

      void
      select::wait()
      {
         int res = ::select( _max + 1, &_read, &_write, NULL, NULL );
      }

      bool
      select::can_read( const pipe& pipe )
      {
         return FD_ISSET( pipe.fd(), &_read );
      }

      bool
      select::can_write( const pipe& pipe )
      {
         return FD_ISSET( pipe.fd(), &_write );
      }
   }
}

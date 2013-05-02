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

#ifndef libhpc_system_timer_hh
#define libhpc_system_timer_hh

#include <ostream>
#include <time.h>
#include "gcc_4.6_fix.hh"

namespace hpc {
   namespace posix {

      typedef struct timespec time_type;

      ///
      ///
      ///
      time_type
      timer();

      unsigned long
      nsecs( const time_type& time );

      unsigned long
      usecs( const time_type& time );

      unsigned long
      msecs( const time_type& time );

      double
      seconds( const time_type& time );
   }
}

::hpc::posix::time_type
operator-( const ::hpc::posix::time_type& op0,
           const ::hpc::posix::time_type& op1 );

std::ostream&
operator<<( std::ostream& strm,
            const ::hpc::posix::time_type& time );

#endif

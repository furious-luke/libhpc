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

#include "timer.hh"

namespace hpc {
   namespace unix {

      time_type
      timer()
      {
         static time_type ts;
         clock_gettime( CLOCK_MONOTONIC, &ts );
         return ts;
      }

      unsigned long
      nsecs( const time_type& time )
      {
         return time.tv_sec*1000000000 + time.tv_nsec;
      }

      unsigned long
      usecs( const time_type& time )
      {
         return time.tv_sec*1000000 + time.tv_nsec/1000;
      }

      unsigned long
      msecs( const time_type& time )
      {
         return time.tv_sec*1000 + time.tv_nsec/1000000;
      }

      double
      seconds( const time_type& time )
      {
         return (double)time.tv_sec + ((double)time.tv_nsec)*1e-9;
      }
   }
}

::hpc::unix::time_type
operator-( const ::hpc::unix::time_type& op0,
           const ::hpc::unix::time_type& op1 )
{
   hpc::unix::time_type tmp;
   if( op0.tv_nsec < op1.tv_nsec )
   {
      tmp.tv_sec = op0.tv_sec - op1.tv_sec - 1;
      tmp.tv_nsec = 1000000000 + op0.tv_nsec - op1.tv_nsec;
   }
   else
   {
      tmp.tv_sec = op0.tv_sec - op1.tv_sec;
      tmp.tv_nsec = op0.tv_nsec - op1.tv_nsec;
   }
   return tmp;
}

std::ostream&
operator<<( std::ostream& strm,
            const ::hpc::unix::time_type& obj )
{
   strm << obj.tv_sec << "(s) + " << obj.tv_nsec << "(ns)";
   return strm;
}

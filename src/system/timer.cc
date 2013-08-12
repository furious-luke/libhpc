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

#if defined( DARWIN )

   time_type
   timer()
   {
      time_type time;
      time.time = mach_absolute_time();
      return time;
   }

   double
   seconds( const time_type& time )
   {
      mach_timebase_info_data_t info;
      mach_timebase_info( &info );
      return (double)time.time*((double)info.numer/(double)info.denom)*1e-9;
   }

#else

   time_type
   timer()
   {
      time_type ts;
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

#endif

}

#if defined( DARWIN )

::hpc::time_type
operator-( const ::hpc::time_type& op0,
           const ::hpc::time_type& op1 )
{
   return op1 - op0;
}

std::ostream&
operator<<( std::ostream& strm,
            const ::hpc::time_type& obj )
{
   strm << (double)obj.time;
   return strm;
}

#else

::hpc::time_type
operator-( const ::hpc::time_type& op0,
           const ::hpc::time_type& op1 )
{
   hpc::time_type tmp;
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
            const ::hpc::time_type& obj )
{
   strm << obj.tv_sec << "(s) + " << obj.tv_nsec << "(ns)";
   return strm;
}

#endif

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

#ifndef hpc_system_timer_handle_hh
#define hpc_system_timer_handle_hh

#include <boost/move/move.hpp>

namespace hpc {

   template< class, class >
   class timer;

   template< class TimeT,
	     class ClockT >
   class timer_handle
   {
      BOOST_MOVABLE_BUT_NOT_COPYABLE( timer_handle );

   public:

      typedef hpc::timer<TimeT,ClockT> timer_type;

      enum stop_type
      {
         NORMAL,
         TALLY
      };

   public:

      timer_handle( timer_type* timer = 0,
                    stop_type stop = NORMAL )
	 : _timer( timer ),
	   _stop( stop )
      {
      }

      inline
      timer_handle( BOOST_RV_REF( timer_handle ) src )
         : _timer( src._timer ),
           _stop( src._stop )
      {
         src._timer = 0;
      }

      ~timer_handle()
      {
	 if( _timer )
	 {
	    switch( _stop )
	    {
	       case NORMAL:
		  _timer->stop();
		  break;
	       case TALLY:
		  _timer->stop_tally();
		  break;
	    };
	 }
      }

      inline
      timer_handle&
      operator=( BOOST_RV_REF( timer_handle ) src )
      {
         _timer = src._timer;
         _stop = src._stop;
         src._timer = 0;
         return *this;
      }

   protected:

      timer_type* _timer;
      stop_type _stop;
   };

}

#endif

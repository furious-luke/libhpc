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

#ifndef libhpc_system_timer_handle_hh
#define libhpc_system_timer_handle_hh

#include <boost/move/move.hpp>

namespace hpc {

   class timer;

   class timer_handle
   {
      BOOST_MOVABLE_BUT_NOT_COPYABLE( timer_handle );

   public:

      enum stop_type
      {
         NORMAL,
         TALLY
      };

   public:

      timer_handle( hpc::timer* timer = 0,
                    stop_type stop = NORMAL );

      inline
      timer_handle( BOOST_RV_REF( timer_handle ) src )
         : _timer( src._timer ),
           _stop( src._stop )
      {
         src._timer = 0;
      }

      ~timer_handle();

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

      hpc::timer* _timer;
      stop_type _stop;
   };

}

#endif

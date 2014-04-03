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
#include "timer_handle.hh"

namespace hpc {

   timer_handle::timer_handle( hpc::timer* timer,
                               stop_type stop )
      : _timer( timer ),
        _stop( stop )
   {
   }

#ifndef __CUDA_ARCH__

   timer_handle::timer_handle( timer_handle&& src )
      : _timer( src._timer ),
        _stop( src._stop )
   {
      src._timer = 0;
   }

#endif

   timer_handle::~timer_handle()
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

}

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

#ifndef libhpc_profile_timer_handle_hh
#define libhpc_profile_timer_handle_hh

namespace hpc {
   namespace profile {

      class timer;

      class timer_handle
      {
      public:

         enum stop_type
         {
            NORMAL,
            TALLY
         };

      public:

	 timer_handle( profile::timer* timer = 0,
                       stop_type stop = NORMAL );

         timer_handle( const timer_handle& ) = delete;

         timer_handle( timer_handle&& src );

	 ~timer_handle();

      protected:

         profile::timer* _timer;
         stop_type _stop;
      };

   }
}

#endif

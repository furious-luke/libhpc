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

#include <boost/chrono.hpp>
#include "timer_handle.hh"

namespace hpc {

   ///
   ///
   ///
   class timer
   {
   public:

      typedef boost::chrono::process_cpu_clock clock_type;
      typedef boost::chrono::duration<double>  time_type;
      typedef timer_handle                     handle_type;

   public:

      timer( bool start = false );

      ~timer();

      void
      reset();

      bool
      running() const;

      handle_type
      start( handle_type::stop_type stop = handle_type::NORMAL );

      void
      start_explicit();

      void
      stop();

      void
      stop_tally();

      unsigned long
      count() const;

      time_type
      total() const;

      time_type
      mean() const;

   protected:

      clock_type::time_point _start;
      time_type _total;
      unsigned long _cnt;
      bool _run;
      unsigned _stack;
   };

}

#endif

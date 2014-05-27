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

#ifndef hpc_system_timer_hh
#define hpc_system_timer_hh

#include <boost/chrono.hpp>
#include "libhpc/debug/assert.hh"
#include "timer_handle.hh"

namespace hpc {

   ///
   ///
   ///
   template< class TimeT = boost::chrono::duration<double>,
	     class ClockT = boost::chrono::process_real_cpu_clock >
   class timer
   {
   public:

      typedef ClockT                             clock_type;
      typedef TimeT                              time_type;
      typedef timer_handle<time_type,clock_type> handle;

   public:

      timer( bool start = false )
	 : _total( 0 ),
	    _cnt( 0 ),
	    _run( false ),
	    _stack( 0 )
      {
	 if( start )
	    this->start_explicit();
      }

      void
      reset()
      {
	 _total = time_type::zero();
	 _cnt = 0;
      }

      bool
      running() const
      {
	 return _run;
      }

      handle
      start( typename handle::stop_type stop = handle::NORMAL )
      {
	 ++_stack;
	 if( !_run )
	 {
	    _run = true;
	    _start = clock_type::now();
	 }
	 return handle( this, stop );
      }

      void
      start_explicit()
      {
	 ++_stack;
	 if( !_run )
	 {
	    _run = true;
	    _start = clock_type::now();
	 }
      }

      void
      stop()
      {
	 --_stack;
	 if( !_stack )
	 {
	    _run = false;
	    _total += boost::chrono::duration_cast<time_type>( clock_type::now() - _start );
	 }
      }

      void
      stop_tally()
      {
	 stop();
	 if( !_stack )
	    ++_cnt;
      }

      unsigned long
      count() const
      {
	 return _cnt;
      }

      time_type
      total() const
      {
	 return _total;
      }

      time_type
      mean() const
      {
	 ASSERT( _cnt );
	 return _total/(double)_cnt;
      }

   protected:

      typename clock_type::time_point _start;
      time_type _total;
      unsigned long _cnt;
      bool _run;
      unsigned _stack;
   };

   typedef timer<> real_timer;
   typedef timer<boost::chrono::nanoseconds,boost::chrono::high_resolution_clock> hr_timer;

}

#endif

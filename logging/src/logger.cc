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

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include "libhpc/containers/containers.hh"
#include "logger.hh"

#ifndef NLOG

namespace hpc {
   namespace logging {

      double log_base_time;

      logger::logger()
      {
      }

      logger::~logger()
      {
      }

      void
      logger::open()
      {
      }

      void
      logger::close()
      {
      }

      void
      logger::prefix()
      {
         _buf << indent;
         _new_line = false;
      }

      void
      logger::new_line()
      {
         _new_line = true;
      }

      void
      logger::_traits<logger& ( logger& )>::impl::operator()( logger& log,
                                                              logger& (*fp)( logger& ) )
      {
         fp( log );
      }

      void
      logger::_traits<setindent_t>::impl::operator()( logger& log,
                                                      setindent_t si )
      {
         log( si, false );
      }

      ///
      ///
      ///
      logger&
      endl( logger& log )
      {
         log.new_line();
      }
   }
}

#endif

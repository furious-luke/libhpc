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

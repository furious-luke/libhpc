#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include "libhpc/containers/containers.hh"
#include "logger.hh"

#ifndef NLOG

namespace hpc {
   namespace mpi {

      double log_base_time;

      logger::logger( const std::string& filename )
         : logging::file( filename ),
           _base( filename )
      {
      }

      logger::~logger()
      {
      }

      void
      logger::open()
      {
         MPI_Comm_rank( MPI_COMM_WORLD, &_my_rank );
         std::stringstream ss;
         ss << _base << std::setfill( '0' ) << std::setw( 5 ) << _my_rank;
         _filename = ss.str();
         remove( _filename.c_str() );
         _file.open( _filename, std::fstream::out | std::fstream::app );
         _new_line = true;
      }

      void
      logger::prefix()
      {
         if( _new_line ) {
            double timestamp = MPI_Wtime() - log_base_time;
            std::streamsize old_prec = _file.precision();
            _file << std::setprecision( 9 ) << std::left << std::setw( 14 ) << timestamp << std::setprecision( old_prec );
            _file << " " << _my_rank << " : " << indent;
            _new_line = false;
         }
      }
   }
}

#endif

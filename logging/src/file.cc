#include "file.hh"

namespace hpc {
   namespace logging {

#ifndef NLOG

      file::file( const std::string& filename )
         : logger(),
           _filename( filename )
      {
      }

      file::~file()
      {
         close();
      }

      void
      file::open()
      {
         close();
         remove( _filename.c_str() );
         _file.open( _filename, std::fstream::out | std::fstream::app );
         _new_line = true;
      }

      void
      file::close()
      {
         if( _file.is_open() )
         {
            _file << _buf.str();
            _file.close();
         }
      }

      void
      file::new_line()
      {
         _file << _buf.str() << std::endl;
         _buf.str( std::string() );
         _new_line = true;
      }

#endif
   }
}

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

#ifndef debug_logger_hh
#define debug_logger_hh

#include <typeinfo>
#include <sstream>
#include <iomanip>
#include "libhpc/system/stream_indent.hh"

#ifndef NLOG

namespace hpc {
   namespace logging {

      ///
      ///
      ///
      class logger
      {
      public:

         logger();

         ~logger();

         virtual void
         open();

         virtual void
         close();

         virtual void
         new_line();

         virtual void
         prefix();

         template< class T >
         logger&
         operator<<( const T& obj )
         {
            typename _traits<T>::impl impl;
            impl( *this, obj );
            return *this;
         }

         template< class T >
         void
         operator()( const T& obj,
                     bool new_line = true )
         {
            if( new_line && _new_line )
               prefix();
            _buf << obj;
         }

      protected:

         ///
         ///
         ///
         template< class T >
         struct _traits
         {
            struct impl
            {
               void
               operator()( logger& log,
                           const T& obj )
               {
                  log( obj );
               }
            };
         };

         bool _new_line;
         std::stringstream _buf;
      };

      template<>
      struct logger::_traits< logger& ( logger& ) >
      {
         struct impl {
            void
            operator()( logger& log,
                        logger& (*fp)( logger& ) );
         };
      };

      template<>
      struct logger::_traits< setindent_t >
      {
         struct impl {
            void
            operator()( logger& log,
                        setindent_t si );
         };
      };

      ///
      ///
      ///
      logger&
      endl( logger& log );
   }
}

#endif

#endif

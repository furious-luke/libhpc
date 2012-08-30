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

#ifndef libhpc_logging_logger_hh
#define libhpc_logging_logger_hh

#include <typeinfo>
#include <sstream>
#include <iomanip>
#include <list>
#include "libhpc/system/stream_indent.hh"

#ifndef NLOG

namespace hpc {
   namespace logging {

      ///
      ///
      ///
      struct level_t
      {
         unsigned level;
      };

      ///
      ///
      ///
      class logger
      {
      public:

         logger( unsigned min_level=0 );

         virtual
         ~logger();

         virtual void
         open();

         virtual void
         close();

         virtual void
         new_line();

         virtual void
         prefix();

         void
         push_level( unsigned level );

         void
         pop_level();

         bool
         visible() const;

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
                     bool new_line=true )
         {
            // Don't log if currently below minimum level.
            if( visible() )
            {
               if( new_line && _new_line )
                  prefix();
               _buf << obj;
            }
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
         std::list<unsigned> _levels;
         unsigned _min_level;
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

      ///
      ///
      ///
      level_t
      pushlevel( unsigned level );

      ///
      ///
      ///
      logger&
      operator<<( logger& log,
                  level_t level );

      ///
      ///
      ///
      logger&
      poplevel( logger& log );
   }
}

#endif

#endif

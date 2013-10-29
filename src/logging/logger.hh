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
#include <map>
#include <set>
#include "libhpc/system/stream_indent.hh"
#include "libhpc/system/stream_output.hh"

#ifndef NLOG

namespace hpc {
   namespace logging {

      template< class T >
      std::ostream&
      operator<<( std::ostream& strm,
		  const std::vector<T>& obj )
      {
	 strm << "[";
	 if( !obj.empty() )
	 {
	    auto it = obj.cbegin();
	    strm << *it++;
	    while( it != obj.cend() )
	    {
	       strm << ", " << *it++;
	    }
	 }
	 strm << "]";
	 return strm;
      }

      template< class T >
      std::ostream&
      operator<<( std::ostream& strm,
		  const std::set<T>& obj )
      {
	 strm << "{";
	 if( !obj.empty() )
	 {
	    auto it = obj.cbegin();
	    strm << *it++;
	    while( it != obj.cend() )
	    {
	       strm << ", " << *it++;
	    }
	 }
	 strm << "}";
	 return strm;
      }

      template< class T >
      std::ostream&
      operator<<( std::ostream& strm,
		  std::list<T> const& obj )
      {
	 strm << "[";
	 if( !obj.empty() )
	 {
	    auto it = obj.cbegin();
	    strm << *it++;
	    while( it != obj.cend() )
	    {
	       strm << ", " << *it++;
	    }
	 }
	 strm << "]";
	 return strm;
      }

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

         logger( unsigned min_level = 0,
		 const std::string& tag = std::string() );

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

         virtual void
         write() = 0;

	 void
	 add_tag( const std::string& tag );

         void
         push_level( unsigned level );

         void
         pop_level();

	 void
	 push_tag( const std::string& tag );

	 void
	 pop_tag( const std::string& tag );

         bool
         visible();

         std::stringstream&
         buffer();

	 std::list<unsigned>&
	 levels();

	 std::map<std::string,int>&
	 current_tags();

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
         write_buffer( const T& obj )
         {
            std::stringstream& buf = buffer();
            buf << obj;
            write();
            buf.str( std::string() );
         }

         template< class T >
         void
         operator()( const T& obj,
                     bool allow_new_line=true )
         {
            // Don't log if currently below minimum level.
            if( visible() )
            {
               // Print new line prefix, if required.
               if( allow_new_line && _get_new_line() )
                  prefix();

               // Write the object.
               write_buffer( obj );
            }
         }

      protected:

	 bool&
	 _get_new_line();

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

	 // Using mappings from thread ID to the object in question.
	 // Need this for threaded loggers.
	 std::map<int,bool> _new_line;
         std::map<int,std::stringstream*> _buf;
	 std::map<int,std::list<unsigned> > _levels;

	 // Tags need a mapping from the tag to a count so we can
	 // push and pop them properly.
	 std::map<int,std::map<std::string,int> > _cur_tags;

         unsigned _min_level;
	 std::set<std::string> _tags;
      };

      ///
      ///
      ///
      template<>
      struct logger::_traits<logger& ( logger& )>
      {
         struct impl {
            void
            operator()( logger& log,
                        logger& (*fp)( logger& ) );
         };
      };

      ///
      ///
      ///
      template<>
      struct logger::_traits<setindent_t>
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

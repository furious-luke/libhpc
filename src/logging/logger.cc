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
#include "libhpc/system/stream_indent.hh"
#include "libhpc/containers/containers.hh"
#include "logger.hh"

#ifdef _OPENMP
#include <omp.h>
#endif

#ifndef NLOG

namespace hpc {
   namespace logging {

      double log_base_time;

      logger::logger( unsigned min_level,
		      const std::string& tag )
         : _min_level( min_level )
      {
	 if( !tag.empty() )
	    _tags.insert( tag );
      }

      logger::~logger()
      {
         for( std::map<int,std::stringstream*>::iterator it = _buf.begin();
              it != _buf.end();
              ++it )
         {
            if( it->second )
               delete it->second;
         }
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
      logger::new_line()
      {
         if( visible() )
         {
            write_buffer( "\n" );
            _get_new_line() = true;
         }
      }

      void
      logger::prefix()
      {
         write_buffer( indent );
         _get_new_line() = false;
      }

      void
      logger::add_tag( const std::string& tag )
      {
	 _tags.insert( tag );
      }

      void
      logger::push_level( unsigned level )
      {
         levels().push_front( level );
      }

      void
      logger::pop_level()
      {
         levels().pop_front();
      }

      void
      logger::push_tag( const std::string& tag )
      {
	 if( current_tags().find( tag ) == current_tags().end() )
	    current_tags().insert( std::make_pair( tag, 0 ) );
	 ++current_tags()[tag];
      }

      void
      logger::pop_tag( const string& tag )
      {
	 ASSERT( current_tags().find( tag ) != current_tags().end() );
	 int& val = current_tags()[tag];
	 ASSERT( val > 0 );
	 if( --val == 0 )
	    current_tags().erase( tag );
      }

      bool
      logger::visible()
      {
	 bool level_vis = levels().empty() || levels().front() >= _min_level;
	 bool tag_vis = _tags.empty();
	 if( !tag_vis )
	 {
            for( std::set<std::string>::const_iterator it = _tags.begin();
                 it != _tags.end();
                 ++it )
	    {
	       if( current_tags().find( *it ) != current_tags().end() )
	       {
		  tag_vis = true;
		  break;
	       }
	    }
	 }
         return level_vis && tag_vis;
      }

      std::stringstream&
      logger::buffer()
      {
#ifdef _OPENMP
         int tid = omp_get_thread_num();
#else
         int tid = 0;
#endif
#pragma omp critical( logger_buffer )
         if( _buf.find( tid ) == _buf.end() )
            _buf.insert( std::make_pair( tid, new std::stringstream ) );
         return *_buf[tid];
      }

      std::list<unsigned>&
      logger::levels()
      {
#ifdef _OPENMP
         int tid = omp_get_thread_num();
#else
         int tid = 0;
#endif
#pragma omp critical( logger_levels )
         if( _levels.find( tid ) == _levels.end() )
            _levels.insert( std::make_pair( tid, std::list<unsigned>() ) );
         return _levels[tid];
      }

      std::map<std::string,int>&
      logger::current_tags()
      {
	 int tid = OMP_TID;
#pragma omp critical( logger_tags )
         if( _cur_tags.find( tid ) == _cur_tags.end() )
            _cur_tags.insert( std::make_pair( tid, std::map<std::string,int>() ) );
         return _cur_tags[tid];
      }

      bool&
      logger::_get_new_line()
      {
#ifdef _OPENMP
         int tid = omp_get_thread_num();
#else
         int tid = 0;
#endif
#pragma omp critical( logger_get_new_line )
         if( _new_line.find( tid ) == _new_line.end() )
            _new_line.insert( std::make_pair( tid, true ) );
         return _new_line[tid];
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
         return log;
      }

      ///
      ///
      ///
      level_t
      pushlevel( unsigned level )
      {
         level_t lv;
         lv.level = level;
         return lv;
      }

      ///
      ///
      ///
      logger&
      operator<<( logger& log,
                  level_t level )
      {
         log.push_level( level.level );
         return log;
      }

      ///
      ///
      ///
      logger&
      poplevel( logger& log )
      {
         log.pop_level();
         return log;
      }
   }
}

#endif

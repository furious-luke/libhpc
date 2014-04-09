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

#include <boost/algorithm/string/join.hpp>
#include "libhpc/debug/assert.hh"
#include "multimatch.hh"

namespace hpc {

   multimatch::multimatch()
      : _ready( false )
   {
   }

   void
   multimatch::clear()
   {
      _matches.clear();
   }

   void
   multimatch::add_match( std::string const& str )
   {
      _matches.push_back( str );
      _ready = false;
   }

   void
   multimatch::compile()
   {
      if( !_ready )
      {
         std::string pattern = boost::algorithm::join( _matches, ")|(" );
         if( !pattern.empty() )
            pattern = "(" + pattern + ")";
         _re = pattern;
         _ready = true;
      }
   }

   boost::optional<size_t>
   multimatch::match( std::string const& str,
                      boost::smatch& match ) const
   {
      ASSERT( _ready, "Multimatch not compiled." );
      if( boost::regex_match( str, match, _re ) )
         return _last_capture( match );
      else
         return boost::none;
   }

   boost::optional<size_t>
   multimatch::match( std::string const& str ) const
   {
      boost::smatch match;
      return this->match( str, match );
   }

   boost::optional<size_t>
   multimatch::search( std::string const& str,
                       boost::smatch& match ) const
   {
      ASSERT( _ready, "Multimatch not compiled." );
      if( boost::regex_search( str, match, _re ) )
         return _last_capture( match );
      else
         return boost::none;
   }

   boost::optional<size_t>
   multimatch::search( std::string const& str ) const
   {
      boost::smatch match;
      return search( str, match );
   }

   size_t
   multimatch::_last_capture( boost::smatch& match ) const
   {
      size_t last = std::numeric_limits<size_t>::max();
      for( size_t ii = 1; ii < match.size(); ++ii )
      {
         if( match[ii].matched )
            last = ii - 1;
      }
      return last;
   }

};

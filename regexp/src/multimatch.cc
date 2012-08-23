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
#include "multimatch.hh"

namespace hpc {

   multimatch::multimatch()
   {
   }

   void
   multimatch::clear()
   {
      _matches.clear();
   }

   void
   multimatch::add_match( const string& str )
   {
      _matches.push_back( str );
   }

   void
   multimatch::compile()
   {
      string pattern = boost::algorithm::join( _matches, ")|(" );
      if( !pattern.empty() )
         pattern = "(" + pattern + ")";
      _re.construct( pattern );
   }

   optional<index>
   multimatch::match( const string& str ) const
   {
      re::match match;
      if( _re.match( str, match ) )
         return (index)match.last_capture();
      else
         return none;
   }

   optional<index>
   multimatch::match_start( const string& str ) const
   {
      re::match match;
      if( _re.match_start( str, match ) )
         return (index)match.last_capture();
      else
         return none;
   }

   bool
   multimatch::match_start( const string& str,
                            re::match& match ) const
   {
      return _re.match_start( str, match );
   }
};

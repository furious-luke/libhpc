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
      // _re = "";
   }

   void
   multimatch::add_match( const string& str )
   {
      _matches.push_back( str );
   }

   void
   multimatch::compile()
   {
      // _matches.sort();
      string pattern = boost::algorithm::join( _matches, ")|(" );
      if( !pattern.empty() )
         pattern = "(" + pattern + ")";
      // _re = pattern;
   }

   optional<index>
   multimatch::operator()( const string& str ) const
   {
      // boost::smatch res;
      // if( boost::regex_match( str, res, _re ) )
      // {
      //    // std::cout << "\n" << res.get_last_matched_paren() << "\n";
      //    return optional<index>( 0 );
      // }
      // else
      //    return none;
   }
};

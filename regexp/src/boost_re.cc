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

#include "boost_re.hh"

namespace hpc {
   namespace re {
      namespace impl {
         namespace boost {

            re::re()
               : ::boost::regex()
            {
            }

            re::re( const string& expression )
               : ::boost::regex( expression )
            {
            }

            void
            re::construct( const string& expression )
            {
               assign( expression );
            }

            bool
            re::match( const string& str,
                       optional<hpc::re::match&> match ) const
            {
               if( match )
                  return ::boost::regex_match( str.begin(), str.end(), *match, *this, ::boost::match_default );
               else
                  return ::boost::regex_match( str.begin(), str.end(), *this, ::boost::match_default );
            }

            bool
            re::search( const string& str,
                        optional<hpc::re::match&> match ) const
            {
               if( match )
                  return ::boost::regex_search( str.begin(), str.end(), *match, *this, ::boost::match_default | ::boost::match_continuous );
               else
                  return ::boost::regex_search( str.begin(), str.end(), *this, ::boost::match_default | ::boost::match_continuous );
            }
         }
      }
   }
}

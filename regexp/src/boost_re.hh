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

#ifndef libhpc_regexp_boost_re_hh
#define libhpc_regexp_boost_re_hh

#include <boost/regex.hpp>
#include "libhpc/containers/optional.hh"
#include "match.hh"

namespace hpc {
   namespace re {
      namespace impl {
         namespace boost {

            class re
               : ::boost::regex
            {
            public:

               re();

               re( const string& expression );

               void
               construct( const string& expression );

               bool
               match( const string& str,
                      optional<hpc::re::match&> match = optional<hpc::re::match&>() ) const;

               bool
               search( const string& str,
                       optional<hpc::re::match&> match = optional<hpc::re::match&>() ) const;
            };
         }
      }
   }
}

#endif

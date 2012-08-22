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

#ifndef libhpc_regexp_multimatch_hh
#define libhpc_regexp_multimatch_hh

#include "libhpc/system/types.hh"
#include "libhpc/containers/string.hh"
#include "libhpc/containers/optional.hh"
#include "libhpc/containers/list.hh"
#include "libhpc/containers/map.hh"
#include "re.hh"

namespace hpc {

   class multimatch
   {
   public:

      multimatch();

      void
      clear();

      void
      add_match( const string& str );

      ///
      /// Note: Expecting matches to be in descending order of
      ///       length.
      ///
      template< class Iterator >
      void
      add_matches( Iterator first,
                   Iterator last )
      {
         while( first != last )
            _matches.push_back( *first++ );
      }

      void
      compile();

      optional<index>
      operator()( const string& str ) const;

   protected:

      list<string> _matches;
      re::re _re;
   };
};

#endif

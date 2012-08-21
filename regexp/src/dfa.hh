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

#include "libhpc/system/types.hh"
#include "libhpc/containers/optional.hh"
#include "libhpc/containers/csr.hh"
#include "libhpc/containers/vector.hh"
#include "libhpc/containers/string.hh"

class dfa_suite;

namespace hpc {
   namespace re {

      class dfa
      {
         friend class ::dfa_suite;

         friend std::ostream&
         operator<<( std::ostream& strm,
                     const dfa& obj );

      public:

         void
         clear();

         void
         set_states( vector<uint16>& moves,
                     optional<csr<uint16>&> open_captures=optional<csr<uint16>&>(),
                     optional<csr<uint16>&> close_captures=optional<csr<uint16>&>() );

         uint16
         move( uint16 state,
               byte data ) const;

         const vector<uint16>::view
         open_captures( uint16 state );

         const vector<uint16>::view
         close_captures( uint16 state );

         bool
         operator()( const string& str );

      protected:

         bool
         _move( uint16& state,
                byte data,
                const char* ptr );

      protected:

         unsigned _num_states;
         vector<uint16> _moves;
         csr<uint16> _open, _close;
      };
   }
}

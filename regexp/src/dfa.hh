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

#ifndef libhpc_regexp_dfa_hh
#define libhpc_regexp_dfa_hh

#include "libhpc/system/types.hh"
#include "libhpc/containers/optional.hh"
#include "libhpc/containers/csr.hh"
#include "libhpc/containers/vector.hh"
#include "libhpc/containers/string.hh"
#include "match.hh"

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

         dfa();

         dfa( const string& expression );

         void
         construct( const string& expression );

         void
         clear();

         void
         set_states( vector<uint16>& moves,
                     optional<vector<uint16>&> meta_moves = optional<vector<uint16>&>(),
                     optional<vector<uint16>&> open_captures = optional<vector<uint16>&>(),
                     optional<vector<uint16>&> close_captures = optional<vector<uint16>&>() );

         uint16
         move( uint16 state,
               byte data ) const;

         bool
         match( const string& str,
                optional<re::match&> match = optional<re::match&>() ) const;

         bool
         match_start( const string& str,
                      optional<re::match&> match = optional<re::match&>() ) const;

      protected:

         bool
         _match_and_capture( const string& str,
                             re::match& match ) const;

         bool
         _match_start_and_capture( const string& str,
                                   re::match& match ) const;

         bool
         _match( const string& str ) const;

         bool
         _move_and_capture( uint16& state,
                            byte data,
                            const char* ptr,
                            re::match& match ) const;

         bool
         _move( uint16& state,
                byte data,
                const char* ptr ) const;

      protected:

         unsigned _num_states, _num_caps;
         vector<uint16> _moves, _meta_moves;
         vector<uint16> _open, _close;
      };
   }
}

#endif

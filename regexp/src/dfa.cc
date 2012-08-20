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

#include "libhpc/logging/logging.hh"
#include "dfa.hh"
#include "codes.hh"

namespace hpc {
   namespace re {

      void
      dfa::clear()
      {
         _num_states = 0;
         _moves.clear();
         _open.clear();
         _close.clear();
         _caps.clear();
      }

      void
      dfa::set_states( vector<uint16>& states,
                       optional<csr<uint16>&> open_captures,
                       optional<csr<uint16>&> close_captures )
      {
         clear();

         _moves.take( states );
         _num_states = _moves.size()/256;
         ASSERT( _num_states*256 == _moves.size() );

         if( open_captures )
            _open.take( *open_captures );
         else
            _open.num_rows( _num_states );

         if( close_captures )
            _close.take( *close_captures );
         else
            _close.num_rows( _num_states );
         ASSERT( _open.num_rows() == _num_states,
                 "Invalid number of capture rows." );
         ASSERT( _open.num_rows() == _close.num_rows() && _open.array().size() == _close.array().size(),
                 "Number of open and close captures must match." );

         _caps.resize( _open.array().size() );
      }

      uint16
      dfa::move( uint16 state,
                 byte data ) const
      {
         ASSERT( state < _num_states );
         return _moves[state*256 + data];
      }

      const vector<uint16>::view
      dfa::open_captures( uint16 state )
      {
         return _open[state];
      }

      const vector<uint16>::view
      dfa::close_captures( uint16 state )
      {
         return _close[state];
      }

      bool
      dfa::operator()( const string& str )
      {
         LOG_ENTER();

         const char* ptr = str.c_str();
         uint16 state = 0;
         while( *ptr != 0 )
         {
            if( !_move( state, *ptr, ptr ) )
            {
               LOG_EXIT();
               return false;
            }
            ++ptr;
         }

         // We have only matched if we end up in the match state.
         bool res = (move( state, static_cast<byte>( codes::match ) ) != std::numeric_limits<uint16>::max() );

         LOG_EXIT();
         return res;
      }

      bool
      dfa::_move( uint16& state,
                  byte data,
                  const char* ptr )
      {
         LOG_ENTER();
         LOGLN( "In state ", state, " with data ", data );

         // Find the next move.
         state = move( state, data );
         LOGLN( "Next state is ", state );

         // Check if that move is invalid.
         if( state == std::numeric_limits<uint16>::max() )
         {
            LOG_EXIT();
            return false;
         }

         // Check for opening and closing captures.
         {
            const vector<uint16>::view caps = _open[state];
            for( unsigned ii = 0; ii < caps.size(); ++ii )
               _caps[caps[ii]].first = ptr;
         }
         {
            const vector<uint16>::view caps = _close[state];
            for( unsigned ii = 0; ii < caps.size(); ++ii )
               _caps[caps[ii]].second = ptr;
         }

         LOG_EXIT();
         return true;
      }

      std::ostream&
      operator<<( std::ostream& strm,
                  const dfa& obj )
      {
         unsigned offs = 0;
         while( offs < obj._moves.size() )
         {
            vector<uint16>::view state( obj._moves, 256, offs );
            strm << indent << state << "\n";
            offs += 256;
         }
         return strm;
      }
   }
}

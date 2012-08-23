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
#include "syntax_tree.hh"

namespace hpc {
   namespace re {

      dfa::dfa()
      {
      }

      dfa::dfa( const string& expression )
      {
         construct( expression );
      }

      void
      dfa::clear()
      {
         _num_states = 0;
         _moves.clear();
         _open.clear();
         _close.clear();
      }

      void
      dfa::construct( const string& expression )
      {
         syntax_tree st;
         st.construct( expression );
         st.to_dfa( *this );
      }

      void
      dfa::set_states( vector<uint16>& moves,
                       optional<vector<uint16>&> meta_moves,
                       optional<vector<uint16>&> open_captures,
                       optional<vector<uint16>&> close_captures )
      {
         clear();

         _moves.take( moves );
         _num_states = _moves.size()/256;
         ASSERT( _num_states*256 == _moves.size() );

         if( meta_moves )
            _meta_moves.take( *meta_moves );

         if( open_captures )
            _open.take( *open_captures );
         if( close_captures )
            _close.take( *close_captures );

         _num_caps = 0;
         for( auto idx : _open )
         {
            if( idx < std::numeric_limits<uint16>::max() )
               ++_num_caps;
         }
#ifndef NDEBUG
         {
            uint16 close_caps = 0;
            for( auto idx : _close )
            {
               if( idx < std::numeric_limits<uint16>::max() )
                  ++close_caps;
            }
            ASSERT( _num_caps == close_caps );
         }
#endif
      }

      uint16
      dfa::move( uint16 state,
                 byte data ) const
      {
         ASSERT( state < _num_states );
         return _moves[state*256 + data];
      }

      bool
      dfa::match( const string& str,
                  optional<re::match&> match ) const
      {
         LOG_ENTER();

         bool res;
         if( _num_states )
         {
            if( match )
               res = _match_and_capture( str, *match );
            else
               res = _match( str );
         }
         else
            res = false;

         LOG_EXIT();
         return res;
      }

      bool
      dfa::match_start( const string& str,
                        optional<re::match&> match ) const
      {
         LOG_ENTER();

         bool res;
         if( _num_states )
         {
            if( match )
               res = _match_start_and_capture( str, *match );
            else
               ASSERT( 0 );
         }
         else
            res = false;

         LOG_EXIT();
         return res;
      }

      bool
      dfa::_match_and_capture( const string& str,
                               re::match& match ) const
      {
         LOG_ENTER();

         // Set appropriate size and clear out captures.
         match._caps.resize( _num_caps );
         for( auto& cap : match._caps )
            cap.first = cap.second = NULL;

         const char* ptr = str.c_str();
         uint16 state = 0;
         while( *ptr != 0 )
         {
            if( !_move_and_capture( state, *ptr, ptr, match ) )
            {
               LOG_EXIT();
               return false;
            }
            ++ptr;
         }

         // We have only matched if we end up in the match state.
         bool res = (move( state, static_cast<byte>( code_match ) ) != std::numeric_limits<uint16>::max() );

         LOG_EXIT();
         return res;
      }

      bool
      dfa::_match_start_and_capture( const string& str,
                                     re::match& match ) const
      {
         LOG_ENTER();

         // Set appropriate size and clear out captures.
         match._caps.resize( _num_caps );
         for( auto& cap : match._caps )
            cap.first = cap.second = NULL;

         const char* ptr = str.c_str();
         uint16 state = 0;
         while( *ptr != 0 && 
                move( state, static_cast<byte>( code_match ) ) == std::numeric_limits<uint16>::max() )
         {
            if( !_move_and_capture( state, *ptr, ptr, match ) )
            {
               LOG_EXIT();
               return false;
            }
            ++ptr;
         }

         // We have only matched if we end up in the match state.
         bool res = (move( state, static_cast<byte>( code_match ) ) != std::numeric_limits<uint16>::max());

         LOG_EXIT();
         return res;
      }

      bool
      dfa::_match( const string& str ) const
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
         bool res = (move( state, static_cast<byte>( code_match ) ) != std::numeric_limits<uint16>::max() );

         LOG_EXIT();
         return res;
      }

      bool
      dfa::_move_and_capture( uint16& state,
                              byte data,
                              const char* ptr,
                              re::match& match ) const
      {
         LOG_ENTER();
         LOGLN( "In state ", state, " with data ", data );

         // Find the next move.
         uint16 new_state = move( state, data );
         LOGLN( "Next state is ", new_state );

         // Check if that move is invalid.
         if( new_state == std::numeric_limits<uint16>::max() )
         {
            LOG_EXIT();
            return false;
         }

         // If this is a meta state, process it straight away.
         while( new_state >= _num_states )
         {
            LOGLN( "Processing meta state ", new_state );
            uint16 meta = new_state - _num_states;
            if( _open[meta] < std::numeric_limits<uint16>::max() )
            {
               match._caps[_open[meta]].first = ptr;
               LOGLN( "Open group ", _open[meta], " at ", *ptr );
            }
            if( _close[meta] < std::numeric_limits<uint16>::max() )
            {
               match._caps[_close[meta]].second = ptr + 1;
               match._last = _close[meta];
               LOGLN( "Close group ", _close[meta], " at ", *(ptr + 1) );
            }
            new_state = _meta_moves[meta];
            LOGLN( "Next state is ", new_state );
         }

         state = new_state;
         LOG_EXIT();
         return true;
      }

      bool
      dfa::_move( uint16& state,
                  byte data,
                  const char* ptr ) const
      {
         LOG_ENTER();
         LOGLN( "In state ", state, " with data ", data );

         // Find the next move.
         uint16 new_state = move( state, data );
         LOGLN( "Next state is ", new_state );

         // Check if that move is invalid.
         if( new_state == std::numeric_limits<uint16>::max() )
         {
            LOG_EXIT();
            return false;
         }

         // If this is a meta state, process it straight away.
         while( new_state >= _num_states )
         {
            LOGLN( "Processing meta state ", new_state );
            uint16 meta = new_state - _num_states;
            new_state = _meta_moves[meta];
            LOGLN( "Next state is ", new_state );
         }

         state = new_state;
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

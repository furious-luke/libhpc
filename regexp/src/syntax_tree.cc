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

#include "libhpc/debug/debug.hh"
#include "libhpc/logging/logging.hh"
#include "syntax_tree.hh"
#include "dfa.hh"
#include "codes.hh"

namespace hpc {
   namespace re {

      bool
      syntax_tree::dfa_state::compare::operator()( const scoped_ptr<dfa_state>& op0,
                                                   const scoped_ptr<dfa_state>& op1 ) const
      {
         return op0->indices < op1->indices;
                  // if( indices.size() < op->indices.size() )
                  //    return true;
                  // else if( indices.size() > op->indices.size() )
                  //    return false;
                  // else
                  // {
                  //    auto it0 = indices.cbegin(), it1 = op->indices.cbegin();
                  //    for( ; it0 != indices.cend(); ++it0, ++it1 )
                  //    {
                  //       if( *it0 < *it1 )
                  //          return true;
                  //       else if( *it0 > *it1 )
                  //          return false;
                  //    }
                  // }
      }

      syntax_tree::dfa_state::dfa_state()
      {
      }

      syntax_tree::dfa_state::dfa_state( const set<unsigned>& idxs,
                                         unsigned id )
      {
         indices.insert( idxs.begin(), idxs.end() );
         this->id = id;
      }

      syntax_tree::node::node( byte data,
                               node* left,
                               node* right )
         : data( data )
      {
         child[0] = left;
         child[1] = right;
      }

      unsigned
      syntax_tree::node::calc_indices( unsigned idx )
      {
         if( child[0] || child[1] )
         {
            if( child[0] )
               idx = child[0]->calc_indices( idx );
            if( child[1] )
               idx = child[1]->calc_indices( idx );
            index = -1;
            return idx;
         }
         else
         {
            index = idx;
            return idx + 1;
         }
      }

      void
      syntax_tree::node::calc_idx_to_node( vector<node*>& idx_to_node ) const
      {
         if( index != -1 )
            idx_to_node[index] = (node*)this;
         for( unsigned ii = 0; ii < 2; ++ii )
         {
            if( child[ii] )
               child[ii]->calc_idx_to_node( idx_to_node );
         }
      }

      bool
      syntax_tree::node::nullable() const
      {
         if( data == static_cast<byte>( codes::split ) )
         {
            ASSERT( child[0] && child[1] );
            return child[0]->nullable() || child[1]->nullable();
         }
         else if( data == static_cast<byte>( codes::concat ) )
         {
            ASSERT( child[0] && child[1] );
            return child[0]->nullable() && child[1]->nullable();
         }
         else if( data == static_cast<byte>( codes::many ) )
         {
            ASSERT( (bool)child[0] );
            return true;
         }
         else if( data == static_cast<byte>( codes::capture ) )
         {
            ASSERT( (bool)child[0] );
            return child[0]->nullable();
         }
         else
         {
            return false;
         }
      }

      void
      syntax_tree::node::calc_firstpos()
      {
         LOG_ENTER();

         firstpos.clear();
         for( unsigned ii = 0; ii < 2; ++ii )
         {
            if( child[ii] )
               child[ii]->calc_firstpos();
         }

         if( data == static_cast<byte>( codes::split ) )
         {
            for( unsigned ii = 0; ii < 2; ++ii )
            {
               ASSERT( (bool)child[ii] );
               firstpos.insert( child[ii]->firstpos.begin(), child[ii]->firstpos.end() );
            }
         }
         else if( data == static_cast<byte>( codes::concat ) )
         {
            ASSERT( (bool)child[0] );
            firstpos.insert( child[0]->firstpos.begin(), child[0]->firstpos.end() );
            if( child[0]->nullable() )
            {
               ASSERT( (bool)child[1] );
               firstpos.insert( child[1]->firstpos.begin(), child[1]->firstpos.end() );
            }
         }
         else if( data == static_cast<byte>( codes::many ) ||
                  data == static_cast<byte>( codes::capture ) )
         {
            ASSERT( (bool)child[0] );
            firstpos.insert( child[0]->firstpos.begin(), child[0]->firstpos.end() );
         }
         else
         {
            ASSERT( index != -1 );
            firstpos.insert( index );
         }

         LOGLN( "firstpos (", (int)data, "): ", firstpos );
         LOG_EXIT();
      }

      void
      syntax_tree::node::calc_lastpos()
      {
         lastpos.clear();
         for( unsigned ii = 0; ii < 2; ++ii )
         {
            if( child[ii] )
               child[ii]->calc_lastpos();
         }

         if( data == static_cast<byte>( codes::split ) )
         {
            for( unsigned ii = 0; ii < 2; ++ii )
            {
               ASSERT( (bool)child[ii] );
               lastpos.insert( child[ii]->lastpos.begin(), child[ii]->lastpos.end() );
            }
         }
         else if( data == static_cast<byte>( codes::concat ) )
         {
            ASSERT( (bool)child[1] );
            lastpos.insert( child[1]->lastpos.begin(), child[1]->lastpos.end() );
            if( child[1]->nullable() )
            {
               ASSERT( (bool)child[0] );
               lastpos.insert( child[0]->lastpos.begin(), child[0]->lastpos.end() );
            }
         }
         else if( data == static_cast<byte>( codes::many ) ||
                  data == static_cast<byte>( codes::capture ) )
         {
            ASSERT( (bool)child[0] );
            lastpos.insert( child[0]->lastpos.begin(), child[0]->lastpos.end() );
         }
         else
         {
            ASSERT( index != -1 );
            lastpos.insert( index );
         }
      }

      void
      syntax_tree::node::calc_followpos( multimap<unsigned,unsigned>& followpos )
      {
         LOG_ENTER();

         if( data == static_cast<byte>( codes::concat ) )
         {
            LOGLN( "Concatenation." );

            // Depth first.
            ASSERT( child[0] && child[1] );
            for( unsigned ii = 0; ii < 2; ++ii )
               child[ii]->calc_followpos( followpos );

            for( const auto& lp : child[0]->lastpos )
            {
               for( const auto& fp : child[1]->firstpos )
               {
                  LOGLN( "Adding ", fp, " to ", lp );
                  followpos.insert( lp, fp );
               }
            }
         }
         else if( data == static_cast<byte>( codes::many ) )
         {
            // Depth first.
            child[0]->calc_followpos( followpos );

            for( const auto& lp : lastpos )
            {
               for( const auto& fp : firstpos )
                  followpos.insert( lp, fp );
            }
         }
         else if( data == static_cast<byte>( codes::capture ) )
         {
            // Depth first.
            child[0]->calc_followpos( followpos );
         }
         else
         {
            for( unsigned ii = 0; ii < 2; ++ii )
            {
               if( child[ii] )
                  child[ii]->calc_followpos( followpos );
            }
         }

         LOG_EXIT();
      }

      // uint16
      // syntax_tree::node::calc_capture_indices( uint16 idx )
      // {
      //    LOG_ENTER();

      //    if( data == static_cast<byte>( codes::concat ) )
      //    {
      //       ASSERT( child[0] && child[1] );
      //       for( unsigned ii = 0; ii < 2; ++ii )
      //          idx = child[ii]->calc_capture_indices( idx );
      //    }
      //    else if( data == static_cast<byte>( codes::many ) )
      //    {
      //       ASSERT( child[0] );
      //       idx = child[0]->calc_capture_indices( idx );
      //    }
      //    else if( data == static_cast<byte>( codes::capture ) )
      //    {
      //       ASSERT( child[0] );
      //       child[0]->calc_capture_indices( idx );
      //    }
      //    else if( data == static_cast<byte>( codes::split ) )
      //    {
      //       ASSERT( child[0] && child[1] );
      //       uint16 left = child[0]->calc_capture_indices( idx );
      //       uint16 right = child[1]->calc_capture_indices( idx );
      //    }

      //    LOG_EXIT();
      //    return idx;
      // }

      void
      syntax_tree::clear()
      {
         LOG_ENTER();

         _root = NULL;
         _num_captures = 0;

         LOG_EXIT();
      }

      void
      syntax_tree::construct( const string& expr )
      {
         LOG_ENTER();

         // Clear out anything we may have.
         clear();

         // Only continue if we have something to work with.
         if( !expr.empty() )
         {
            const char* ptr = expr.c_str();
            _root = _construct_recurse( ptr );

            // Add final terminal state.
            _root = new node( static_cast<byte>( codes::concat ), _root.release(), new node( static_cast<byte>( codes::match ) ) );
         }

         LOG_EXIT();
      }

      void
      syntax_tree::to_dfa( dfa& dfa )
      {
         LOG_ENTER();

         // First convert to local DFA representation.
         _calc_dfa();

         // Build the moves.
         vector<uint16> moves( _states.size()*256 );
         for( auto it = _states.cbegin(); it != _states.cend(); ++it )
         {
            dfa_state* cur = it->get();
            _conv_moves( cur, moves );
         }

         // Setup dfa.
         dfa.set_states( moves );

         LOG_EXIT();
      }

      syntax_tree::node*
      syntax_tree::_construct_recurse( const char*& ptr,
                                       uint16 level )
      {
         LOG_ENTER();

         // Keep this sub-branch isolated, begin from scratch.
         node* cur_node = NULL;

         // Walk over each character.
         char ch = *ptr;
         bool force = false, concat = false;
         node* new_node;
         while( ch != 0 )
         {
            if( force )
            {
               LOGLN( "Forced: ", ch );

               // Concatenate.
               new_node = new node( ch );
               concat = true;
               force = false;
            }
            else if( ch == '\\' )
            {
               // The next character is a terminal.
               force = true;
            }
            else if( ch == '|' )
            {
               LOGLN( "Splitting." );

               // 'Or'.
               ASSERT( cur_node, "Must be expression to left of |." );
               cur_node = new node( static_cast<byte>( codes::split ), cur_node, _construct_recurse( ++ptr, level ) );
               ch = *ptr;
            }
            else if( ch == '*' )
            {
               LOGLN( "Star." );

               // Closure?
               ASSERT( cur_node, "Must be existing expression." );
               cur_node = new node( static_cast<byte>( codes::many ), cur_node );
            }
            else if( ch == '(' )
            {
               LOGLN( "Opening capture." );

               // Create new sub-branch.
               new_node = new node( static_cast<byte>( codes::capture ), _construct_recurse( ++ptr, level + 1 ) );

               // // Mark the capture.
               // LOGLN( "Marking capture at level ", level );
               // new_node->open.insert( level );
               // new_node->close.insert( level );

               // // Keep track of max levels and number of captures.
               // _num_levels = std::max<uint16>( level + 1, _num_levels );
               // ++_num_captures;

               ch = *ptr;
               concat = true;
            }
            else if( ch == ')' )
            {
               LOGLN( "Closing capture." );

               // // Mark closing capture.
               // cur_node->close.insert( std::make_pair( level - 1, cap_idx++ ) );

               // // Increment number of global captures.
               // ++_num_captures;

               LOG_EXIT();
               return cur_node;
            }
            else
            {
               // Concatenate.
               LOGLN( "Concatenate: ", ch );
               new_node = new node( ch );
               concat = true;
            }

            // Handle concatenation.
            if( concat )
            {
               if( cur_node )
                  cur_node = new node( static_cast<byte>( codes::concat ), cur_node, new_node );
               else
                  cur_node = new_node;
               concat = false;
            }

            // Advance.
            if( ch != 0 )
               ch = *++ptr;
         }

         LOG_EXIT();
         return cur_node;
      }

      void
      syntax_tree::_calc_followpos( multimap<unsigned,unsigned>& followpos )
      {
         followpos.clear();

         ASSERT( (bool)_root );
         _num_indices = _root->calc_indices( 0 );
         _calc_idx_to_node( _idx_to_node );
         _root->calc_firstpos();
         _root->calc_lastpos();
         _root->calc_followpos( followpos );
      }

      void
      syntax_tree::_calc_idx_to_node( vector<node*>& idx_to_node ) const
      {
         idx_to_node.reallocate( _num_indices );
         _root->calc_idx_to_node( idx_to_node );
      }

      void
      syntax_tree::_calc_dfa()
      {
         LOG_ENTER();

         _states.clear();
         _to_proc.clear();

         _calc_followpos( _followpos );
         unsigned cur_id = 0;
         dfa_state* s0 = new dfa_state( _root->firstpos, cur_id++ );
         _states.insert( s0 );
         _to_proc.push_back( s0 );
         while( !_to_proc.empty() )
         {
            _proc_dfa_state( *_to_proc.front(), cur_id );
            _to_proc.pop_front();
         }

         LOG_EXIT();
      }

      void
      syntax_tree::_proc_dfa_state( dfa_state& state,
                                    unsigned& cur_id )
      {
         LOG_ENTER();
         LOGLN( "Looking at state: ", state.indices );

         map<char,dfa_state*> new_states;
         bool is_accepting;
         for( auto idx : state.indices )
         {
            node* node = _idx_to_node[idx];
            char data = node->data;
            if( data == static_cast<byte>( codes::match ) )
            {
               is_accepting = true;
               continue;
            }
            ASSERT( data < static_cast<byte>( codes::terminal ) );
            LOGLN( "Index: ", idx, ", data: ", data );

            auto res = new_states.insert( data, NULL );
            if( res.second )
            {
               LOGLN( "Creating new temporary state." );
               res.first->second = new dfa_state;
            }
            dfa_state& new_state = *res.first->second;

            auto rng = _followpos.equal_range( idx );
            for( ; rng.first != rng.second; ++rng.first )
               new_state.indices.insert( rng.first->second );
         }
#ifndef NLOG
         LOGLN( "Potential new states:", setindent( 2 ) );
         for( const auto& state_pair : new_states )
            LOGLN( state_pair.second->indices );
         LOG( setindent( -2 ) );
#endif

         for( const auto& state_pair : new_states )
         {
            char data = state_pair.first;

            auto res = _states.insert( state_pair.second );
            dfa_state* new_state = res.first->get();

            // Insert the move operation.
            state.moves.insert( data, new_state );

            if( res.second )
            {
               LOGLN( "Created new state, ", (*res.first)->indices, ", with id ", cur_id );

               // Add node captures to the state.
               for( auto idx : new_state->indices )
               {
                  node& node = *_idx_to_node[idx];
                  // new_state->open.insert( node.open.begin(), node.open.end() );
                  // new_state->close.insert( node.close.begin(), node.close.end() );
               }

               new_state->id = cur_id++;
               _to_proc.push_back( new_state );
            }
         }
#ifndef NLOG
         LOGLN( "State moves:", setindent( 2 ) );
         for( const auto& move : state.moves )
            LOGLN( move.first, " -> ", move.second->indices );
         LOG( setindent( -2 ) );
#endif

         // If this state is accepting (i.e. has a permissible movement
         // from the match code) add a movement to itself.
         if( is_accepting )
            state.moves.insert( static_cast<byte>( codes::match ), &state );

         LOG_EXIT();
      }

      void
      syntax_tree::_conv_moves( const dfa_state* state,
                                vector<uint16>& moves ) const
      {
         vector<uint16>::view state_moves( moves, 256, state->id*256 );
         std::fill( state_moves.begin(), state_moves.end(), std::numeric_limits<uint16>::max() );
         for( const auto& elem : state->moves )
            state_moves[static_cast<index>( elem.first )] = elem.second->id;
      }

      std::ostream&
      operator<<( std::ostream& strm,
                  const syntax_tree& obj )
      {
         if( obj._root )
            strm << *obj._root;
         return strm;
      }

      std::ostream&
      operator<<( std::ostream& strm,
                  const syntax_tree::node& obj )
      {
         strm << (int)obj.data;
         for( unsigned ii = 0; ii < 2; ++ii )
         {
            strm << "\n" << indent << "|-";
            if( obj.child[ii] )
               strm << setindent( 1 ) << *obj.child[ii] << setindent( -1 );
         }
         return strm;
      }
   }
}

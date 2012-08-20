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

#include <iostream>
#include "libhpc/system/types.hh"
#include "libhpc/containers/scoped_ptr.hh"
#include "libhpc/containers/string.hh"
#include "libhpc/containers/multimap.hh"
#include "libhpc/containers/set.hh"
#include "libhpc/containers/vector.hh"
#include "libhpc/containers/map.hh"

class syntax_tree_suite;

namespace hpc {
   namespace re {

      class dfa;

      class syntax_tree
      {
      public:

         struct dfa_state
         {
            struct compare
            {
               bool
               operator()( const scoped_ptr<dfa_state>& op0,
                           const scoped_ptr<dfa_state>& op1 ) const;
            };

            dfa_state();

            dfa_state( const set<unsigned>& idxs,
                       unsigned id );

            unsigned id;
            set<unsigned> indices;
            map<char,dfa_state*> moves;
            set<uint16> open, close;
         };

         struct node
         {
            node( byte data,
                  node* left=NULL,
                  node* right=NULL );

            unsigned
            calc_indices( unsigned idx );

            void
            calc_idx_to_node( vector<node*>& idx_to_node ) const;

            bool
            nullable() const;

            void
            calc_firstpos();

            void
            calc_lastpos();

            void
            calc_followpos( multimap<unsigned,unsigned>& followpos );

            void
            calc_captures( multimap<unsigned,unsigned>& open,
                           multimap<unsigned,unsigned>& close );

            void
            _calc_dfa();

            byte data;
            scoped_ptr<node> child[2];
            unsigned index;
            set<unsigned> firstpos, lastpos;
            uint16 capture_index;
            set<uint16> open, close;
         };

      public:

         void
         clear();

         void
         construct( const string& expr );

         void
         to_dfa( dfa& dfa );

         friend std::ostream&
         operator<<( std::ostream& strm,
                     const syntax_tree& obj );

      protected:

         node*
         _construct_recurse( const char*& ptr,
                             uint16 level=0 );

         void
         _calc_followpos( multimap<unsigned,unsigned>& followpos );

         void
         _calc_idx_to_node( vector<node*>& data_map ) const;

         void
         _calc_dfa();

         void
         _proc_dfa_state( dfa_state& state,
                          unsigned& cur_id );

         void
         _conv_moves( const dfa_state* state,
                      vector<uint16>& moves ) const;

      protected:

         scoped_ptr<node> _root;
         unsigned _num_captures;
         uint16 _num_levels;
         unsigned _num_indices;
         vector<node*> _idx_to_node;
         multimap<unsigned,unsigned> _followpos;

         set<scoped_ptr<dfa_state>,dfa_state::compare> _states;
         list<dfa_state*> _to_proc;

         friend class ::syntax_tree_suite;
      };

      std::ostream&
      operator<<( std::ostream& strm,
                  const syntax_tree::node& obj );
   }
}

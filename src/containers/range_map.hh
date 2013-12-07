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

#ifndef hpc_containers_range_map_hh
#define hpc_containers_range_map_hh

#include <boost/iterator/iterator_facade.hpp>
#include "map.hh"
#include "set.hh"
#include "range.hh"
#include "list.hh"

namespace hpc {

   template< class T,
             class Value >
   class range_map_insert_iterator;

   ///
   ///
   ///
   template< class T,
             class Value >
   class range_map
      : public map< range<T>,
                    Value >
   {
   public:

      typedef map<range<T>,Value> super_type;
      typedef range<T> range_type;
      typedef Value mapped_type;
      typedef typename super_type::iterator iterator;
      typedef typename super_type::cosnt_iterator const_iterator;
      typedef range_map_insert_iterator<T,Value> insert_iterator;

      insert_iterator
      insert( const range_type& range,
              const Value& value )
      {
         return insert_iterator( *this, range, value );
      }
   };

   template< class T,
             class Value >
   class range_map_insert_iterator
      : public boost::iterator_facade< range_map_insert_iterator<T,Value>,
                                       std::pair<range<T>,Value&>,
				       std::forward_iterator_tag,
                                       std::pair<range<T>,Value&> >
   {
   public:

      range_map_insert_iterator( range_map<T,Value>& map,
                                 const range<T>& range,
                                 const Value& value )
         : _map( map ),
           _range( range ),
           _value( value )
      {
         _begin();
      }

      bool
      done()
      {
         return !_cur_val;
      }

   protected:

      void
      _begin()
      {
         _low = _map.lower_bound( _range );
         _upp = _map.upper_bound( _range );

         // If low returns end() then the supplied range is greater than
         // all of the current ranges.
         if( _low == _map.end() ||
             _low->first.start() >= _range.finish() )
         {
            // Call parent insert.
            typename range_map<T,Value>::const_iterator res =
               ((typename range_map<T,Value>::super_type&)_map).insert( _range, _value );
            ASSERT( res.second, "Element should not have existed" );

            // Cache values.
            _cur_rng = res.first->first;
            _cur_val = &res.first->second; // Should use reference.
         }
         else
            _overlap();

         // Try and advance the low iterator.
         if( _low != _upp )
            ++_low;
      }

      void
      _overlap()
      {
         // Block in frontal overlap?
         T start, finish;
         if( _low->first.start() < _range.start() && _low->first.finish() > _range.start() )
         {
            start = _range.start();
            finish = std::min( _low->first.finish(), _range.finish() );
            _cur_rng.set( start, finish );
            _cur_val = &_low->second;

            // Replace.
            T orig_start = _low->first.start();
            Value orig_val = _low->second;
            _map.erase( _low );
            _low = ((typename range_map<T,Value>::super_type&)_map).insert( range<T>( orig_start, start ), orig_val ).first;

            typename range_map<T,Value>::iterator res =
               ((typename range_map<T,Value>::super_type&)_map).insert( _cur_rng, _low->second );
            ASSERT( res.second, "Element should not have existed" );
         }

         // Block in perfect overlap?
         else if( _low->first.start() == _range.start() && _low->first.finish() == _range.start() )
         {
            _cur_rng = _low->first;
            _cur_val = &_low->second;
         }

         // Block in central overlap?
         else if( _low->first.start() >= _range.start() && _low->first.finish() <= _range.finish() )
         {
            _cur_rng = _low->first;
            _cur_val = &_low->second;
         }

         // Block in rear overlap?
         else if( _low->first.start() >= _range.start() && _low->first.start() < _range.finish() && _low->first.finish() > _range.finish() )
         {
            start = _low->first.start();
            finish = _range.finish();
            _cur_rng.set( start, finish );
            _cur_val = &_low->second;

            // Replace.
            T orig_finish = _low->first.finish();
            Value orig_val = _low->second;
            _map.erase( _low );
            _low = ((typename range_map<T,Value>::super_type&)_map).insert( range<T>( finish, orig_finish ), orig_val ).first;

            typename range_map<T,Value>::iterator res =
               ((typename range_map<T,Value>::super_type&)_map).insert( _cur_rng, _low->second );
            ASSERT( res.second, "Element should not have existed" );
         }

         // Block in total overlap?
         else if( _low->first.start() < _range.start() && _low->first.finish() > _range.finish() )
         {
            _cur_rng.set( _range.finish(), _low->first.finish() );

            // Replace
            T orig_start = _low->first.start();
            Value orig_val = _low->second;
            _map.erase( _low );
            _low = ((typename range_map<T,Value>::super_type&)_map).insert( range<T>( orig_start, _range.start() ), orig_val ).first;

            typename range_map<T,Value>::iterator res =
               ((typename range_map<T,Value>::super_type&)_map).insert( _cur_rng, _low->second );
            ASSERT( res.second, "Element should not have existed" );

            _cur_rng = _range;
            _cur_val = &_low->second;
            res = ((typename range_map<T,Value>::super_type&)_map).insert( _cur_rng, _low->second );
            ASSERT( res.second, "Element should not have existed" );
         }
      }

      void
      increment()
      {
         // If low now points to the end, add the remaining range.
         if( _low == _upp )
         {
            if( _cur_rng.finish() < _range.finish() )
            {
               _cur_rng.set( _cur_rng.finish(), _range.finish() );
               typename range_map<T,Value>::iterator res =
                  ((typename range_map<T,Value>::super_type&)_map).insert( _cur_rng, _value );
               ASSERT( res.second, "Element should not have existed" );
               _cur_val = &res.first->second;
            }
            else
               _cur_val = NULL;
         }

         // Where are we compared to the last visited finish point?
         else if( _cur_rng.finish() < _low->first.start() )
         {
            // Fill in the gap.
            _cur_rng.set( _cur_rng.finish(), _low->first.start() );
            typename range_map<T,Value>::iterator res =
               ((typename range_map<T,Value>::super_type&)_map).insert( _cur_rng, _value );
            ASSERT( res.second, "Element should not have existed" );
            _cur_val = &res.first->second;
         }
         else
         {
            _overlap();
            ++_low;
         }
      }

      bool
      equal( const range_map_insert_iterator& op ) const
      {
         return _cur_val == op._cur_val;
      }

      std::pair<range<T>,Value&>
      dereference() const
      {
         return std::pair<range<T>,Value&>( _cur_rng, *_cur_val );
      }

   protected:

      range_map<T,Value>& _map;
      const range<T>& _range;
      const Value& _value;
      typename range_map<T,Value>::iterator _low, _upp;
      range<T> _cur_rng;
      Value* _cur_val;

      friend class boost::iterator_core_access;
   };

   // ///
   // ///
   // ///
   // template< class T,
   //           class Value >
   // void
   // invert_mapping( const range_map<T,Value>& mapping,
   //                 map<Value,set<range<T> > >& inv )
   // {
   //    for( typename range_map<T,Value>::const_iterator rng_it = mapping.begin();
   //         rng_it != mapping.end();
   //         ++rng_it )
   //    {
   //       for( typename range_map<T,Value>:: const auto& val : rng_it->second )
   //       {
   //          // TODO: Combine sequential ranges.
   //          inv[val].insert( elem.first );
   //       }
   //    }
   // }

}

#endif

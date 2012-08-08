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

#ifndef containers_functors_hh
#define containers_functors_hh

#include <functional>
#include <boost/tuple/tuple.hpp>

namespace hpc {

   template< class Iter,
	     class Value >
   void
   iota( Iter first,
	 Iter last,
	 Value init )
   {
      for(; first != last; ++first, ++init)
	 *first = init;
   }

   template< class Sequence,
	     class Value >
   void
   iota( Sequence& seq,
	 Value init )
   {
      typename Sequence::iterator end = seq.end();
      for(typename Sequence::iterator it = seq.begin(); it != end; ++it, ++init)
	 *it = init;
   }

   template< class InputIter,
	     class OutputIter,
	     class Predicate >
   OutputIter
   copy_if( InputIter first,
	    InputIter last,
	    OutputIter result,
	    Predicate pred )
   {
      while(first != last) {
	 if(pred(*first))
	    *(result++) = *first;
	 ++first;
      }
      return result;
   }

   template< class T >
   struct counted {
      typedef std::pair<int, T> result_type;

      counted()
	 : _idx(0)
      {
      }

      std::pair<int, T>
      operator()( const T& val ) const
      {
	 return std::make_pair(((counted*)this)->_idx++, val);
      }

      hpc::index _idx;
   };

   template< class T >
   struct counted_flipped {
      typedef std::pair<T, int> result_type;

      counted_flipped()
	 : _idx(0)
      {
      }

      std::pair<T, int>
      operator()( const T& val ) const
      {
	 return std::make_pair(val, ((counted_flipped*)this)->_idx++);
      }

      hpc::index _idx;
   };

   template< class Map >
   struct map_getter {

      typedef typename Map::key_type key_type;
      typedef typename Map::mapped_type mapped_type;
      typedef typename Map::value_type value_type;

      map_getter( const Map& map )
	 : _map(&map)
      {
      }

      typename Map::mapped_type
      operator()( const typename Map::key_type& key )
      {
	 return this->_map->get(key);
      }

      const Map* _map;
   };

   template< class Map >
   map_getter<Map> map_get( const Map& map )
   {
      return map_getter<Map>(map);
   }

   template< class Map >
   struct map_derefer
      : public std::unary_function< typename Map::key_type,
				    typename Map::mapped_type >
   {
      typedef typename Map::key_type key_type;
      typedef typename Map::mapped_type mapped_type;
      typedef typename Map::value_type value_type;

      map_derefer( const Map& map )
	 : _map(&map)
      {
      }

      typename Map::mapped_type
      operator()( const typename Map::key_type& key ) const
      {
	 return (*this->_map)[key];
      }

      const Map* _map;
   };

   template< class Map >
   map_derefer<Map> map_deref( const Map& map )
   {
      return map_derefer<Map>(map);
   }

   template< class Array >
   struct element
      : public std::unary_function< const Array&,
				    typename Array::value_type >
   {
      typedef index key_type;
      typedef typename Array::value_type mapped_type;
      typedef typename Array::value_type value_type;
      typedef typename Array::value_type result_type;

      element( index idx )
	 : _idx( idx )
      {
      }

      value_type
      operator()( const Array& array ) const
      {
         return array[this->_idx];
      }

      index _idx;
   };

   template< class Set >
   struct _set_has
   {

      typedef typename Set::key_type key_type;
      typedef typename Set::mapped_type mapped_type;
      typedef typename Set::value_type value_type;

      _set_has( const Set& set )
	 : _set(&set)
      {
      }

      mapped_type
      operator()( const key_type& key )
      {
	 return this->_set->has(key);
      }

      const Set* _set;
   };

   template< class Set >
   _set_has<Set> set_has( const Set& set )
   {
      return _set_has<Set>(set);
   }

   template< class T >
   class identity
      : public std::unary_function< T, T >
   {
   public:

      inline T
      operator()( const T& arg ) const
      {
         return arg;
      }
   };

   template< class Operation1,
	     class Operation2,
	     class Operation3 >
   class binary_compose
      : public std::binary_function< typename Operation2::argument_type,
				     typename Operation3::argument_type,
				     typename Operation1::result_type >
   {
   public:
      binary_compose( const Operation1& op1,
		      const Operation2& op2,
		      const Operation3& op3 )
	 : _op1(op1),
	   _op2(op2),
	   _op3(op3)
      {
      }

      typename Operation1::result_type
      operator()( const typename Operation2::argument_type& x,
		  const typename Operation3::argument_type& y ) const
      {
	 return this->_op1(this->_op2(x), this->_op3(y));
      }

   protected:
      Operation1 _op1;
      Operation2 _op2;
      Operation3 _op3;
    };

   template< class Operation1,
	     class Operation2,
	     class Operation3 >
   inline binary_compose<Operation1, Operation2, Operation3>
   compose2( const Operation1& op1,
	     const Operation2& op2,
	     const Operation3& op3 )
   {
      return binary_compose<Operation1, Operation2, Operation3>(op1, op2, op3);
   }

   // template< class Tuple >
   // typename boost::tuples::element<1, Tuple>::type
   // tuple_select1st( Tuple& x )
   // {
   //    return boost::tuples::get<1>(x);
   // }

   // template< class Tuple >
   // const typename boost::tuples::element<1, Tuple>::type
   // tuple_select1st( const Tuple& x )
   // {
   //    return boost::tuples::get<1>(x);
   // }

   // template< class Tuple,
   // 	     int N >
   // struct tuple_select
   //    : public unary_function< Tuple,
   // 			       typename boost::tuple::element<N, Tuple>::type >
   // {
   //    typename boost::tuple::element<N, Tuple>::type&
   //    operator()( Tuple& x ) const
   //    {
   // 	 return boost::tuple::get<N>(x);
   //    }

   //    const typename boost::tuple::element<N, Tuple>::type&
   //    operator()( const Tuple& x ) const
   //    {
   // 	 return boost::tuple::get<N>(x);
   //    }
   // }

   // template< class Tuple >
   // struct tuple_select1st
   //    : public std::unary_function< Tuple,
   // 				    typename boost::tuples::element<1, Tuple>::type >
   // {
   //    typename boost::tuples::element<1, Tuple>::type&
   //    operator()( Tuple& x ) const
   //    {
   // 	 return boost::tuples::get<1>(x);
   //    }

   //    const typename boost::tuples::element<1, Tuple>::type&
   //    operator()( const Tuple& x ) const
   //    {
   // 	 return boost::tuples::get<1>(x);
   //    }
   // };
}

#endif

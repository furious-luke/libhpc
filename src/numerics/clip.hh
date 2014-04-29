#ifndef libhpc_numerics_clip_hh
#define libhpc_numerics_clip_hh

#include <vector>
#include <list>
#include <boost/array.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include "libhpc/system/math.hh"
#include "libhpc/algorithm/combination.hpp"
#include "libhpc/algorithm/inner_product.hh"
#include "libhpc/system/functional.hh"

namespace hpc {

   template< class Iterator >
   typename std::iterator_traits<Iterator>::value_type
   magnitude( Iterator first,
              const Iterator& last )
   {
      typedef typename std::iterator_traits<Iterator>::value_type real_type;
      real_type mag = 0.0;
      while( first != last )
      {
         mag += (*first)*(*first);
         ++first;
      }
      return sqrt( mag );
   }

   template< class Seq1,
             class Seq2,
             class Seq3 >
   void
   cross_product_3( const Seq1& x,
                    const Seq2& y,
                    Seq3& z )
   {
      z[0] = x[1]*y[2] - x[2]*y[1];
      z[1] = x[2]*y[0] - x[0]*y[2];
      z[2] = x[0]*y[1] - x[1]*y[0];
   }

   template< class PointIterator,
	     class HalfSpaceIterator >
   bool
   inside( PointIterator point_first,
	   const PointIterator& point_last,
	   HalfSpaceIterator half_space_first,
	   typename std::iterator_traits<PointIterator>::value_type epsilon = 1e-8 )
   {
      typedef typename std::iterator_traits<PointIterator>::value_type real_type;
      real_type sum = 0.0;
      while( point_first != point_last )
	 sum += (*half_space_first++)*(*point_first++);
      return sum >= *half_space_first + epsilon;
   }

   template< class PointIterator,
	     class HalfSpaceIterator >
   typename std::iterator_traits<PointIterator>::value_type
   half_space_eval( PointIterator point_first,
		    const PointIterator& point_last,
		    HalfSpaceIterator half_space_first )
   {
      typedef typename std::iterator_traits<PointIterator>::value_type real_type;
      real_type sum = 0.0;
      while( point_first != point_last )
	 sum += (*half_space_first++)*(*point_first++);
      return sum - *half_space_first;
   }

   template< class Point1Iterator,
	     class Point2Iterator,
	     class HalfSpaceIterator,
	     class ResultIterator >
   void
   line_half_space_intersection( Point1Iterator point1_first,
				 const Point1Iterator& point1_last,
				 Point2Iterator point2_first,
				 const Point2Iterator& point2_last,
				 HalfSpaceIterator half_space_first,
				 ResultIterator result )
   {
      typedef typename std::iterator_traits<Point1Iterator>::value_type real1_type;
      typedef typename std::iterator_traits<Point2Iterator>::value_type real2_type;
      auto func = expand2(
	 compose2(
	    std::minus<real1_type>(),
	    element_tuple<boost::tuple<real2_type,real1_type>,0>(),
	    element_tuple<boost::tuple<real2_type,real1_type>,1>()
	    )
	 );
	 
      real1_type denom = inner_product(
      	 boost::make_transform_iterator(
      	    boost::make_zip_iterator( boost::make_tuple( point2_first, point1_first ) ),
      	    func
      	    ),
      	 boost::make_transform_iterator(
      	    boost::make_zip_iterator( boost::make_tuple( point2_last, point1_last ) ),
      	    func
      	    ),
      	 half_space_first
      	 );
      denom = 1.0/denom;
      real1_type enumr = half_space_eval( point1_first, point1_last, half_space_first );
      while( point1_first != point1_last )
      {
      	 *result++ = *point1_first - (*point2_first - *point1_first)*enumr*denom;
	 ++point1_first;
	 ++point2_first;
      }
   }

   template< class MinIterator,
	     class MaxIterator,
	     class HalfSpaceIterator >
   bool
   box_half_space_overlap( MinIterator min_first,
			   const MinIterator& min_last,
			   MaxIterator max_first,
			   HalfSpaceIterator half_space_first,
			   typename std::iterator_traits<MinIterator>::value_type epsilon = 1e-8 )
   {
      typedef typename std::iterator_traits<MinIterator>::value_type real1_type;
      typedef typename std::iterator_traits<MaxIterator>::value_type real2_type;
      unsigned size = min_last - min_first;
      std::vector<real1_type> perms( 2*size );
      for( unsigned ii = 0; ii < size; ++ii )
      {
	 perms[ii] = 0;
	 perms[ii + size] = 1;
      }
      boost::array<real1_type,3> point;
      do
      {
	 MinIterator min_it = min_first;
	 MaxIterator max_it = max_first;
	 auto perms_it = perms.cbegin();
	 auto point_it = point.begin();
	 {
	    while( min_it != min_last )
	    {
	       *point_it = (*perms_it) ? *max_it : *min_it;
	       ++point_it;
	       ++perms_it;
	       ++min_it;
	       ++max_it;
	    }
	 }
	 if( inside( point.begin(), point.end(), half_space_first, epsilon ) )
	    return true;
      }
      while( boost::next_partial_permutation( perms.begin(), perms.begin() + 3, perms.end() ) );
      return false;
   }

   template< class MinIterator,
	     class MaxIterator,
	     class HalfSpaceIterator >
   bool
   box_half_space_intersection( MinIterator min_first,
				const MinIterator& min_last,
				MaxIterator max_first,
				HalfSpaceIterator half_space_first )
   {
      typedef typename std::iterator_traits<MinIterator>::value_type real1_type;
      typedef typename std::iterator_traits<MaxIterator>::value_type real2_type;
      unsigned size = min_last - min_first;
      std::vector<real1_type> perms( 2*size );
      for( unsigned ii = 0; ii < size; ++ii )
      {
	 perms[ii] = 0;
	 perms[ii + size] = 1;
      }
      boost::array<real1_type,3> point;
      bool in = false, out = false;
      do
      {
	 MinIterator min_it = min_first;
	 MaxIterator max_it = max_first;
	 auto perms_it = perms.cbegin();
	 auto point_it = point.begin();
	 {
	    while( min_it != min_last )
	    {
	       *point_it = (*perms_it) ? *max_it : *min_it;
	       ++point_it;
	       ++perms_it;
	       ++min_it;
	       ++max_it;
	    }
	 }
	 if( inside( point.begin(), point.end(), half_space_first ) )
	 {
	    if( out )
	       return true;
	    else
	       in = true;
	 }
	 else
	 {
	    if( in )
	       return true;
	    else
	       out = true;
	 }
      }
      while( boost::next_partial_permutation( perms.begin(), perms.begin() + 3, perms.end() ) );
      return false;
   }

   template< class MinIterator,
	     class MaxIterator,
	     class MinBndIterator,
	     class MaxBndIterator,
	     class ResultMinIterator,
	     class ResultMaxIterator >
   void
   box_box_clip( MinIterator min_first,
		 const MinIterator& min_last,
		 MaxIterator max_first,
		 MinBndIterator bnd_min_first,
		 MaxBndIterator bnd_max_first,
		 ResultMinIterator min_result,
		 ResultMaxIterator max_result )
   {
      while( min_first != min_last )
      {
	 if( *min_first < *bnd_min_first )
	    *min_result = *bnd_min_first;
	 else if( *min_first > *bnd_max_first )
	    *min_result = *bnd_max_first;
	 else
	    *min_result = *min_first;
	 if( *max_first < *bnd_min_first )
	    *max_result = *bnd_min_first;
	 else if( *max_first > *bnd_max_first )
	    *max_result = *bnd_max_first;
	 else
	    *max_result = *max_first;
	 ++min_first;
	 ++max_first;
	 ++bnd_min_first;
	 ++bnd_max_first;
	 ++min_result;
	 ++max_result;
      }
   }

   template< class MinIterator,
	     class MaxIterator >
   typename std::iterator_traits<MinIterator>::value_type
   box_volume( MinIterator min_first,
	       const MinIterator& min_last,
	       MaxIterator max_first )
   {
      typename std::iterator_traits<MinIterator>::value_type vol = 1.0;
      while( min_first != min_last )
	 vol *= *max_first++ - *min_first++;
      return vol;
   }

   template< class PointIterator >
   typename PointIterator::value_type::value_type
   polygon_area( PointIterator point_first,
		 const PointIterator& point_last )
   {
      typename PointIterator::value_type::value_type sum = 0.0;
      if( point_first != point_last )
      {
	 auto it_a = point_first++;
	 auto first = it_a;
	 while( point_first != point_last )
	 {
	    const auto& pnt_a = *it_a;
	    const auto& pnt_b = *point_first;
	    sum += pnt_a[0]*pnt_b[1] - pnt_b[0]*pnt_a[1];
	    it_a = point_first++;
	 }
	 const auto& pnt_a = *it_a;
	 const auto& pnt_b = *first;
	 sum += pnt_a[0]*pnt_b[1] - pnt_b[0]*pnt_a[1];
      }
      return 0.5*abs( sum );
   }

   template< class HalfSpaceIterator,
	     class PointIterator,
	     class ResultIterator >
   void
   clip_edge( const HalfSpaceIterator& half_space,
	      PointIterator first,
	      const PointIterator& last,
	      ResultIterator result )
   {
      typedef typename std::iterator_traits<HalfSpaceIterator>::value_type real_type;

      // If we have nothing, return nothing.
      if( first == last )
	 return;

      // Need to keep track of two points per edge.
      PointIterator pnts[2], first_point = first;
      unsigned edge[2] = { 0, 1 };
      bool intersect[2], first_intersect;

      // Set the first point and check if it's interior.
      pnts[0] = first++;
      intersect[0] = inside( pnts[0]->begin(), pnts[0]->end(), half_space );
      if( intersect[0] )
      {
	 *result++ = *pnts[0];
	 first_intersect = true;
      }
      else
	 first_intersect = false;

      // Now process remaining edges/points.
      while( first != last )
      {
	 pnts[edge[1]] = first++;
	 intersect[edge[1]] = inside( pnts[edge[1]]->begin(), pnts[edge[1]]->end(), half_space );

	 // Case 1, edge goes from outside window to outside.
	 if( !intersect[edge[0]] && !intersect[edge[1]] )
	 {
	    // Do nothing.
	 }

	 // Case 2, edge goes from outside window to inside.
	 else if( !intersect[edge[0]] && intersect[edge[1]] )
	 {
	    // Save both intersection and inside vertex.
	    boost::array<real_type,2> tmp;
	    line_half_space_intersection(
	       pnts[edge[0]]->begin(), pnts[edge[0]]->end(),
	       pnts[edge[1]]->begin(), pnts[edge[1]]->end(),
	       half_space,
	       tmp.begin()
	       );
	    *result++ = tmp;
	    *result++ = *pnts[edge[1]];
	 }

	 // Case 3, edge goes from inside window to outside.
	 else if( intersect[edge[0]] && !intersect[edge[1]] )
	 {
	    // Save intersection.
	    boost::array<real_type,2> tmp;
	    line_half_space_intersection(
	       pnts[edge[0]]->begin(), pnts[edge[0]]->end(),
	       pnts[edge[1]]->begin(), pnts[edge[1]]->end(),
	       half_space,
	       tmp.begin()
	       );
	    *result++ = tmp;
	 }

	 // Case 4, edge goes from inside window to inside.
	 else
	 {
	    // Save inside vertex.
	    *result++ = *pnts[edge[1]];
	 }

	 // Swap edge ends.
	 std::swap( edge[0], edge[1] );
      }

      // Are the first and last points on opposite sides? We use edge[0]
      // because of the swap.
      if( first_intersect ^ intersect[edge[0]] )
      {
	 boost::array<real_type,2> tmp;
	 line_half_space_intersection(
	    pnts[edge[0]]->begin(), pnts[edge[0]]->end(),
	    first_point->begin(), first_point->end(),
	    half_space,
	    tmp.begin()
	    );
	 *result++ = tmp;
      }
   }

   template< class PointIterator,
	     class ResultIterator >
   void
   clip_polygon( double box_size,
		 PointIterator poly_start,
		 const PointIterator& poly_finish,
		 ResultIterator result )
   {
      typedef typename std::iterator_traits<PointIterator>::value_type point_type;
      typedef typename point_type::value_type real_type;

      // We will need temporary storage.
      std::list<point_type> tmp[2];

      // Need to know the number of dimensions to clip in.
      unsigned dim = poly_start->size();

      // Prepare the plane equation.
      std::vector<real_type> hsp( dim + 1 );

      // First clip the X axis.
      std::fill( hsp.begin(), hsp.end(), 0 );
      hsp[0] = 1;
      clip_edge( hsp.begin(), poly_start, poly_finish,
		 std::insert_iterator<std::list<point_type> >( tmp[0], tmp[0].begin() ) );
      hsp[0] = -1;
      hsp[dim] = -box_size;
      if( dim == 1 )
      {
	 clip_edge( hsp.begin(), tmp[0].begin(), tmp[0].end(), result );
      }
      else
      {
	 clip_edge( hsp.begin(), tmp[0].begin(), tmp[0].end(),
		    std::insert_iterator<std::list<point_type> >( tmp[1], tmp[1].begin() ) );
      }

      // Now clip middle dimensions.
      for( unsigned ii = 1; ii < dim - 1; ++ii )
      {
	 std::fill( hsp.begin(), hsp.end(), 0 );
	 hsp[ii] = 1;
	 tmp[0].clear();
	 clip_edge( hsp.begin(), tmp[1].begin(), tmp[1].end(),
		    std::insert_iterator<std::list<point_type> >( tmp[0], tmp[0].begin() ) );
	 hsp[ii] = -1;
	 hsp[dim] = -box_size;
	 tmp[1].clear();
	 clip_edge( hsp.begin(), tmp[0].begin(), tmp[0].end(),
		    std::insert_iterator<std::list<point_type> >( tmp[1], tmp[1].begin() ) );
      }

      // Now the final dimension.
      std::fill( hsp.begin(), hsp.end(), 0 );
      hsp[dim - 1] = 1;
      tmp[0].clear();
      clip_edge( hsp.begin(), tmp[1].begin(), tmp[1].end(),
		 std::insert_iterator<std::list<point_type> >( tmp[0], tmp[0].begin() ) );
      hsp[dim - 1] = -1;
      hsp[dim] = -box_size;
      tmp[1].clear();
      clip_edge( hsp.begin(), tmp[0].begin(), tmp[0].end(),
		 std::insert_iterator<std::list<point_type> >( tmp[1], tmp[1].begin() ) );

      // Finally, check if the area is zero (or close to). If so,
      // discard the new polygon.
      if( !approx( polygon_area( tmp[1].begin(), tmp[1].end() ), 0.0, 1e-8 ) )
	 std::copy( tmp[1].begin(), tmp[1].end(), result );
   }
}

#endif

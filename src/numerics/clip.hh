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

#ifndef hpc_numerics_clip_hh
#define hpc_numerics_clip_hh

#include <vector>
#include <list>
#include <boost/array.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include "libhpc/system/math.hh"
#include "libhpc/system/matrix.hh"
#include "libhpc/algorithm/combination.hpp"
#include "libhpc/algorithm/inner_product.hh"
#include "libhpc/system/functional.hh"
#include "libhpc/logging.hh"
#include "coords.hh"

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

   template< class EndPointT,
             class PointT >
   typename PointT::value_type
   distance_to_line_2( EndPointT const& ep0,
                       EndPointT const& ep1,
                       PointT const pnt )
   {
      typedef typename PointT::value_type real_type;
      real_type numer = (pnt[0] - ep0[0])*(ep1[0] - ep0[0]) + (pnt[1] - ep0[1])*(ep1[1] - ep0[1]);
      real_type denom = (ep1[0] - ep0[0])*(ep1[0] - ep0[0]) + (ep1[1] - ep0[1])*(ep1[1] - ep0[1]);
      real_type r = numer/denom;
      if( r >= 0.0 && r <= 1.0 )
      {
         PointT perp;
         perp[0] = ep1[1] - ep0[1];
         perp[1] = -(ep1[0] - ep0[0]);
         real_type inv_mag = 1.0/sqrt( perp[1]*perp[1] + perp[0]*perp[0] );
         real_type dist = fabs( -perp[1]*(ep0[1] - pnt[1]) - (ep0[0] - pnt[0])*perp[0] )*inv_mag;
         return dist;
         // real_type s = ((ep0[1] - pnt[1])*(ep1[0] - ep0[0]) - (ep0[0] - pnt[0])*(ep1[1] - ep0[1]))/denom;
         // return fabs( s )/sqrt( denom );
      }
      else
      {
         return std::min(
            sqrt( (ep0[0] - pnt[0])*(ep0[0] - pnt[0]) + (ep0[1] - pnt[1])*(ep0[1] - pnt[1]) ),
            sqrt( (ep1[0] - pnt[0])*(ep1[0] - pnt[0]) + (ep1[1] - pnt[1])*(ep1[1] - pnt[1]) )
            );
      }
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
      typedef typename std::iterator_traits<PointIterator>::value_type point_type;
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
         // Interior, store point.
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
	    point_type tmp;
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
            point_type tmp;
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
         point_type tmp;
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

   template< class BoxMinIterT,
             class BoxMaxIterT,
             class SphIterT >
   bool
   sphere_box_collision( BoxMinIterT box_min_begin,
                         BoxMinIterT const& box_min_end,
                         BoxMaxIterT box_max_begin,
                         SphIterT sph_begin )
   {
      typedef typename std::iterator_traits<BoxMinIterT>::value_type value_type;
      value_type min_dist = 0;
      while( box_min_begin != box_min_end )
      {
         value_type sph = *sph_begin++;
         if( sph < *box_min_begin )
            min_dist += (sph - *box_min_begin)*(sph - *box_min_begin);
         else if( sph > *box_max_begin )
            min_dist += (sph - *box_max_begin)*(sph - *box_max_begin);
         ++box_min_begin;
         ++box_max_begin;
      }
      LOGTLN( "Sphere-box collision, min_dist: ", min_dist, ", radius: ", (*sph_begin)*(*sph_begin) );
      return min_dist < (*sph_begin)*(*sph_begin);
   }

   template< class EcsSeqT,
	     class BoxSeqT >
   bool
   ecs_box_collision( EcsSeqT const& ecs_min,
		      EcsSeqT const& ecs_max,
		      BoxSeqT const& box_min,
		      BoxSeqT const& box_max )
   {
      typedef typename BoxSeqT::value_type real_type;

      hpc::matrix<real_type> crds( 8, 3 ), ecs( 8, 3 );
      crds( 0, 0 ) = box_min[0]; crds( 0, 1 ) = box_min[1]; crds( 0, 2 ) = box_min[2];
      crds( 1, 0 ) = box_max[0]; crds( 1, 1 ) = box_min[1]; crds( 1, 2 ) = box_min[2];
      crds( 2, 0 ) = box_max[0]; crds( 2, 1 ) = box_max[1]; crds( 2, 2 ) = box_min[2];
      crds( 3, 0 ) = box_min[0]; crds( 3, 1 ) = box_max[1]; crds( 3, 2 ) = box_min[2];
      crds( 4, 0 ) = box_min[0]; crds( 4, 1 ) = box_min[1]; crds( 4, 2 ) = box_max[2];
      crds( 5, 0 ) = box_max[0]; crds( 5, 1 ) = box_min[1]; crds( 5, 2 ) = box_max[2];
      crds( 6, 0 ) = box_max[0]; crds( 6, 1 ) = box_max[1]; crds( 6, 2 ) = box_max[2];
      crds( 7, 0 ) = box_min[0]; crds( 7, 1 ) = box_max[1]; crds( 7, 2 ) = box_max[2];
      for( int ii = 0; ii < 8; ++ii )
      {
	 hpc::num::cartesian_to_ecs<double>( crds( ii, 0 ), crds( ii, 1 ), crds( ii, 2 ),
					     ecs( ii, 0 ), ecs( ii, 1 ), ecs( ii, 2 ) );
      }

      // To discard based on max distance we need to use a
      // collision routine.
      {
         boost::array<real_type,4> sph{ 0.0, 0.0, 0.0, ecs_max[2] };
         if( !hpc::sphere_box_collision( box_min.begin(), box_min.end(), box_max.begin(), sph.begin() ) )
         {
            LOGTLN( "Box outside outer radius." );
            return false;
         }
      }

      // Alternatively, all of the corners may be below the minimum
      // radial distance.
      bool discard = true;
      for( unsigned ii = 0; ii < 8; ++ii )
      {
         if( ecs( ii, 2 ) >= ecs_min[2] )
         {
            LOGTLN( "Corner ", ii, " with radius ", ecs( ii, 2 ), " greater than ", ecs_min[2], "." );
            discard = false;
            break;
         }
      }
      if( discard )
      {
         LOGTLN( "Box within inner radius." );
         return false;
      }

      // Now we handle RA and DEC. We need to split the operation
      // into to halves.
      for( int half_ii = 0; half_ii < 2; ++half_ii )
      {
         LOGTLN( "Checking half: ", half_ii );

         // Which ecs coordinates should I use for the two planes?
         real_type min_ra, max_ra;
         if( half_ii == 0 )
         {
            if( ecs_min[0] < M_PI )
            {
               min_ra = ecs_min[0];
               if( ecs_max[0] <= M_PI )
                  max_ra = ecs_max[0];
               else
                  max_ra = M_PI;
            }
            else
               continue;
         }
         else
         {
            if( ecs_max[0] > M_PI )
            {
               max_ra = ecs_max[0];
               if( ecs_min[0] >= M_PI )
                  min_ra = ecs_min[0];
               else
                  min_ra = M_PI;
            }
            else
               continue;
         }
         LOGTLN( "Using RA range: ", min_ra, " to ", max_ra );

         // Clip the appropriate surface.
         std::list<boost::array<real_type,3> > surf[2];
         for( int ii = 0; ii < 4; ++ii )
            surf[1].push_back( boost::array<real_type,3>{ crds( ii, 0 ), crds( ii, 1 ), 0.0 } );
         if( hpc::approx( ecs_min[0], ecs_max[0], 1e-8 ) )
         {
            // Do nothing, as we want an empty list.
            surf[1].clear();
         }
         else if( hpc::approx( ecs_min[0], 0.0, 1e-8 ) && hpc::approx( ecs_max[0], 2.0*M_PI, 1e-8 ) )
         {
            // Fill the list with the standard.
         }
         else
         {
            boost::array<real_type,4> ra_planes[2];
            boost::array<real_type,3> tmp[2];
            num::ecs_to_cartesian<real_type>( min_ra, -0.25*M_PI, tmp[0][0], tmp[0][1], tmp[0][2] );
            num::ecs_to_cartesian<real_type>( min_ra,  0.25*M_PI, tmp[1][0], tmp[1][1], tmp[1][2] );
            cross_product_3( tmp[1], tmp[0], ra_planes[0] );
            ra_planes[0][3] = 0.0;
            ASSERT( hpc::approx( ra_planes[0][2], 0.0, 1e-8 ) );
            num::ecs_to_cartesian<real_type>( max_ra, -0.25*M_PI, tmp[0][0], tmp[0][1], tmp[0][2] );
            num::ecs_to_cartesian<real_type>( max_ra,  0.25*M_PI, tmp[1][0], tmp[1][1], tmp[1][2] );
            cross_product_3( tmp[0], tmp[1], ra_planes[1] );
            ra_planes[1][3] = 0.0;
            ASSERT( hpc::approx( ra_planes[1][2], 0.0, 1e-8 ) );
            LOGTLN( "RA planes: ", ra_planes[0], " and ", ra_planes[1] );
            clip_edge( ra_planes[0].begin(), surf[1].begin(), surf[1].end(),
                       std::insert_iterator<std::list<boost::array<real_type,3> > >( surf[0], surf[0].begin() ) );
            LOGTLN( "Intermediate surface: ", surf[0] );
            surf[1].clear();
            clip_edge( ra_planes[1].begin(), surf[0].begin(), surf[0].end(),
                       std::insert_iterator<std::list<boost::array<real_type,3> > >( surf[1], surf[1].begin() ) );
            LOGTLN( "Final surface: ", surf[1] );
         }

         // If there is no surface left then we can discard this half.
         if( surf[1].empty() )
            continue;

         // Calculate the nearest and furthest of the points.
         real_type min_dist = std::numeric_limits<real_type>::max();
         real_type max_dist = std::numeric_limits<real_type>::min();
         unsigned ii = 0;
         for( typename std::list<boost::array<real_type,3> >::const_iterator it = surf[1].begin();
              it != surf[1].end(); 
              ++it, ++ii )
         {
            real_type dist;
            boost::array<real_type,2> zero{ 0.0, 0.0 };
            boost::array<real_type,2> ep0 = { (*it)[0], (*it)[1] };
            if( ii < surf[1].size() - 1 )
            {
               typename std::list<boost::array<real_type,3> >::const_iterator it2 = it;
               ++it2;
               boost::array<real_type,2> ep1 = { (*it2)[0], (*it2)[1]};
               dist = distance_to_line_2( ep0, ep1, zero );
            }
            else
            {
               boost::array<real_type,2> ep1 = { surf[1].front()[0], surf[1].front()[1] };
               dist = distance_to_line_2( ep0, ep1, zero );
            }
            if( dist < min_dist )
               min_dist = dist;

            // The furthest distance will always be a point.
            dist = sqrt( ep0[0]*ep0[0] + ep0[1]*ep0[1] );
            if( dist > max_dist )
               max_dist = dist;
         }

         // Use the calculated maximum distance to check if all points of
         // the clipped box are now within the minimum radius.
         if( sqrt( max_dist*max_dist + box_min[2]*box_min[2] ) < ecs_min[2] &&
             sqrt( max_dist*max_dist + box_max[2]*box_max[2] ) < ecs_min[2] )
         {
            LOGTLN( "All clipped points within inner radius." );
            continue;
         }

         // If the furthest point of the bottom plane is inside the
         // maximum DEC cone, we can discard.
         // LOGTLN( "Checking max DEC: dec=", ecs_max[1], ", max_dist=", max_dist,
         //         ", box_min[2]=", box_min[2], ", cone_rad=", box_min[2]/tan( ecs_max[1] ) );
         if( ecs_max[1] > 0.0 )
         {
            if( !hpc::approx( ecs_max[1], 0.5*M_PI, 1e-8 ) &&
                box_min[2] > 0.0 && max_dist <= fabs( box_min[2]/tan( ecs_max[1] ) ) )
            {
               LOGTLN( "Lower plane inside upper cone." );
               continue;
            }
         }
         else if( ecs_max[1] < 0.0 )
         {
            if( hpc::approx( ecs_max[1], -0.5*M_PI, 1e-8 ) ||
                (box_min[2] > 0.0 || min_dist >= fabs( box_min[2]/tan( ecs_max[1] ) )) )
            {
               LOGTLN( "Lower plane inside upper cone." );
               continue;
            }
         }
         else if( box_min[2] >= 0.0 )
         {
            LOGTLN( "Lower plane above maximum dec (0.0)." );
            continue;
         }

         // If the nearest point of the top plane is outside the
         // minimum DEC cone, we can discard.
         // LOGT( "Checking min DEC: dec=", ecs_min[1], ", max_dist=", max_dist, ", min_dist=", min_dist );
         // LOGTLN(", box_max[2]=", box_max[2], ", cone_rad=", fabs( box_max[2]*tan( ecs_min[1] ) ) );
         if( ecs_min[1] < 0.0 )
         {
            if( !hpc::approx( ecs_min[1], -0.5*M_PI, 1e-8 ) &&
                box_max[2] < 0.0 && max_dist <= fabs( box_max[2]/tan( ecs_min[1] ) ) )
            {
               LOGTLN( "Upper plane inside lower cone." );
               continue;
            }
         }
         else if( ecs_min[1] > 0.0 )
         {
            if( hpc::approx( ecs_min[1], 0.5*M_PI, 1e-8 ) ||
                (box_max[2] < 0.0 || min_dist >= fabs( box_max[2]/tan( ecs_min[1] ) )) )
            {
               LOGTLN( "Upper plane inside lower cone." );
               continue;
            }
         }
         else if( box_max[2] <= 0.0 )
         {
            LOGTLN( "Upper plane below minimum dec (0.0)." );
            continue;
         }

         // Need to handle cases where there is a clipped column. This is
         // basically checking the cylinder created by the dec circles...
         // wow that's confusing.
         {
            real_type upp_rad = cos( ecs_max[1] )*ecs_min[2];
            real_type upp_z = sin( ecs_max[1] )*ecs_min[2];
            real_type low_rad = cos( ecs_min[1] )*ecs_min[2];
            real_type low_z = sin( ecs_min[1] )*ecs_min[2];
            if( box_max[2] >= upp_z && max_dist <= upp_rad )
            {
               if( box_min[2] >= 0.0 )
                  continue;
               else if( box_min[2] >= low_z )
               {
                  if( max_dist <= sqrt( ecs_min[2]*ecs_min[2] - box_min[2]*box_min[2] ) )
                     continue;
               }
               else
               {
                  if( max_dist <= low_rad )
                     continue;
               }
            }
            if( box_min[2] <= low_z && max_dist <= low_rad )
            {
               if( box_max[2] <= 0.0 )
                  continue;
               else if( box_max[2] <= upp_z )
               {
                  if( max_dist <= sqrt( ecs_min[2]*ecs_min[2] - box_max[2]*box_max[2] ) )
                     continue;
               }
               else
               {
                  if( max_dist <= upp_rad )
                     continue;
               }
            }
         }

         // If I make it here, then this half touches the
         // ECS geometry.
         LOGTLN( "In contact." );
         return true;
      }

      // If I make it here then neither half touches the ECS geom.
      return false;
   }

}

#endif

#ifndef hpc_algorithm_xdmf_writer_hh
#define hpc_algorithm_xdmf_writer_hh

#include <fstream>
#include <sstream>
#include <boost/mpl/map.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/at.hpp>
#include "libhpc/mpi.hh"
#include "libhpc/h5.hh"
#include "kdtree.hh"

namespace mpl = boost::mpl;

namespace hpc {

   void
   write_xdmf_xml_begin( std::ofstream& xml_file );

   void
   write_xdmf_close_grid_xml( std::ofstream& xml_file );

   void
   write_xdmf_xml_end( std::ofstream& xml_file );

   void
   write_xdmf_xml_domain_begin( std::ofstream& xml_file );

   void
   write_xdmf_xml_domain_end( std::ofstream& xml_file );

   void
   write_xdmf_xml_grid_begin( std::ofstream& xml_file,
                              const std::string& name );

   void
   write_xdmf_xml_topology( std::ofstream& xml_file,
                            const std::string& h5_filename,
			    const std::string& grid,
                            int dim,
                            unsigned num_global_cells,
                            unsigned num_cell_points );

   void
   write_xdmf_xml_geometry( std::ofstream& xml_file,
                            const std::string& h5_filename,
			    const std::string& grid,
                            int dim,
                            unsigned num_global_cells,
                            unsigned num_cell_points );

   template< class IterT >
   void
   write_xdmf_xml_field( std::ofstream& xml_file,
                         const std::string& h5_filename,
                         const std::string& grid,
                         const std::string& name,
                         IterT const& begin,
                         IterT const& end,
                         mpi::comm const& comm = mpi::comm::world );

   template< class IterT >
   void
   write_xdmf_h5_field( h5::file& h5_file,
                        const std::string& name,
                        IterT const& begin,
                        IterT const& end,
                        mpi::comm const& comm = mpi::comm::world );

   void
   write_xdmf_open_grid_xml( std::ofstream& xml_file,
			     const std::string& h5_filename,
			     const std::string& name,
			     const kdtree<>& kdt );

   template< class IterT >
   void
   write_xdmf_xml_open_particles( std::ofstream& xml_file,
                                  const std::string& h5_filename,
                                  const std::string& name,
                                  IterT const& begin,
                                  IterT const& end,
                                  mpi::comm const& comm )
   {
      unsigned n_dims = begin->end() - begin->begin();
      unsigned n_parts = end - begin;
      write_xdmf_xml_grid_begin( xml_file, name );
      write_xdmf_xml_topology( xml_file, h5_filename, name, n_dims, n_parts, 1 );
      write_xdmf_xml_geometry( xml_file, h5_filename, name, n_dims, n_parts, 1 );
   }

   template< class IterT >
   void
   write_xdmf_h5_particles( h5::file& h5_file,
                            const std::string& grid,
                            IterT const& begin,
                            IterT const& end,
                            mpi::comm const& comm )
   {
      unsigned dim = begin->end() - begin->begin();
      unsigned num_local_cells = end - begin;
      unsigned num_global_cells = comm.all_reduce( num_local_cells );
      unsigned num_cell_points = 1;
      {
         std::vector<hsize_t> space_dims( 2 );
         space_dims[0] = num_global_cells;
         space_dims[1] = num_cell_points;
         h5::dataspace file_space( space_dims );
         h5::dataset topology_set;
         topology_set.create( h5_file, grid + "/topology", h5::datatype::native_ulong, file_space );
         space_dims[0] = 1;
         h5::dataspace mem_space( space_dims );
         mem_space.select_all();
         std::vector<unsigned> buffer(num_cell_points);
         std::vector<hsize_t> start( 2 ), count( 2 );
         count[0] = 1;
         count[1] = num_cell_points;
         start[1] = 0;
         unsigned global_offs = comm.scan( num_local_cells, MPI_SUM, true );
         for( unsigned ii = global_offs; ii < global_offs + num_local_cells; ++ii ) {
            std::iota( buffer.begin(), buffer.end(), ii*num_cell_points );
            start[0] = ii;
            file_space.select_hyperslab<std::vector<hsize_t> >( H5S_SELECT_SET, count, start );
            topology_set.write( buffer.data(), h5::datatype::native_uint, mem_space, file_space, comm );
         }
      }
      {
         std::vector<hsize_t> space_dims( 2 );
         space_dims[0] = num_global_cells*num_cell_points;
         space_dims[1] = dim;
         h5::dataspace file_space( space_dims );
         h5::dataset geometry_set;
         geometry_set.create( h5_file, grid + "/geometry", h5::datatype::native_double, file_space );
         space_dims[0] = num_cell_points;
         h5::dataspace mem_space( space_dims );
         mem_space.select_all();
         std::vector<double> buffer( num_cell_points*dim );
         std::vector<hsize_t> start( 2 ), count( 2 );
         count[0] = num_cell_points;
         count[1] = dim;
         start[1] = 0;
         unsigned global_offs = comm.scan( num_local_cells );
         auto it = begin;
         for( unsigned ii = global_offs; ii < global_offs + num_local_cells; ++ii, ++it )
         {
            std::copy( it->begin(), it->end(), buffer.begin() );
            start[0] = ii*num_cell_points;
            file_space.select_hyperslab<std::vector<hsize_t> >( H5S_SELECT_SET, count, start );
            geometry_set.write( buffer.data(), h5::datatype::native_double, mem_space, file_space, comm );
         }
      }
      // {
      //    std::vector<hsize_t> space_dims( 1 );
      //    space_dims[0] = num_global_cells;
      //    h5::dataspace file_space( space_dims );
      //    h5::dataset topology_set;
      //    topology_set.create( h5_file, "ownership", h5::datatype::native_int, file_space );
      //    space_dims[0] = 1;
      //    h5::dataspace mem_space( space_dims );
      //    mem_space.select_all();
      //    std::vector<hsize_t> start( 1 ), count( 1 );
      //    count[0] = 1;
      //    unsigned global_offs = comm.scan( num_local_cells, MPI_SUM, true );
      //    int buffer = comm.rank();
      //    for( unsigned ii = global_offs; ii < global_offs + num_local_cells; ++ii ) {
      //       start[0] = ii;
      //       file_space.select_hyperslab<std::vector<hsize_t> >( H5S_SELECT_SET, count, start );
      //       topology_set.write( &buffer, h5::datatype::native_int, mem_space, file_space, comm );
      //    }
      // }
   }

   class xdmf_writer
   {
   public:

      xdmf_writer();

      xdmf_writer( const std::string& base_filename,
		   mpi::comm const& comm = mpi::comm::world );

      ~xdmf_writer();

      void
      open( const std::string& base_filename,
	    mpi::comm const& comm = mpi::comm::world );

      void
      open_grid( const std::string& name,
		 const kdtree<>& kdt );

      template< class IterT >
      void
      open_grid( std::string const& name,
		 IterT const& begin,
		 IterT const& end,
		 mpi::comm const& comm = mpi::comm::world )
      {
	 _grid = name;
         write_xdmf_xml_open_particles<IterT>( _xml_file, _h5_filename, name, begin, end, comm );
         write_xdmf_h5_particles<IterT>( _h5_file, _grid, begin, end, comm );
      }

      template< class IterT >
      void
      write_field( const std::string& name,
                   IterT const& begin,
                   IterT const& end )
      {
         write_xdmf_xml_field( this->_xml_file, this->_h5_filename, _grid, name, begin, end );
         write_xdmf_h5_field( this->_h5_file, _grid + "/" + name, begin, end );
      }

      void
      close_grid();

      void
      close();

   protected:

      std::string _h5_filename;
      h5::file _h5_file;
      std::ofstream _xml_file;
      std::string _grid;
   };

   void
   write_xdmf( const std::string& base_filename,
               const std::string& name,
               const kdtree<>& kdt );

   void
   write_xdmf_open_grid_h5( h5::file& h5_file,
			    const std::string& grid,
			    const kdtree<>& kdt );

   template< class IterT >
   void
   write_xdmf_h5_field( h5::file& h5_file,
                        const std::string& name,
                        IterT const& begin,
                        IterT const& end,
                        mpi::comm const& comm )
   {
      typedef typename IterT::value_type::value_type value_type;

      unsigned dim = begin->end() - begin->begin();
      unsigned pv_dim = (dim == 2) ? 3 : dim;
      unsigned num_locals = end - begin;
      unsigned num_globals = comm.all_reduce( num_locals );

      std::vector<hsize_t> space_dims( 1 );
      space_dims[0] = num_globals*pv_dim;
      h5::dataspace file_space( space_dims );

      h5::dataset field_set;
      h5::datatype type( mpl::at<h5::datatype::type_map,value_type>::type::value );
      field_set.create( h5_file, name, type, file_space );

      space_dims[0] = pv_dim;
      h5::dataspace mem_space( space_dims );
      mem_space.select_all();

      std::vector<hsize_t> start( 1 ), count( 1 );
      count[0] = pv_dim;
      unsigned global_offs = comm.scan( num_locals*pv_dim );
      start[0] = global_offs;
      std::vector<value_type> buffer( pv_dim );
      for( IterT it = begin; it != end; ++it )
      {
         std::copy( it->begin(), it->end(), buffer.begin() );
         if( dim == 2 )
            buffer[2] = 0.0;
         file_space.select_hyperslab<std::vector<hsize_t> >( H5S_SELECT_SET, count, start );
         field_set.write( buffer.data(), type, mem_space, file_space, comm );
         start[0] += pv_dim;
      }
   }

   void
   write_xdmf_open_xml( std::ofstream& xml_file,
                        const std::string& h5_filename );

   void
   write_xdmf_xml_close( std::ofstream& xml_file );

   void
   write_xdmf_xml_attribute( std::ofstream& xml_file,
                             const std::string& name,
                             const std::string& h5_filename,
                             const std::string& h5_path,
                             const std::string& type,
                             unsigned dim,
                             unsigned num_global_cells,
                             bool cell_centered = false );

   template< class IterT >
   void
   write_xdmf_xml_field( std::ofstream& xml_file,
                         const std::string& h5_filename,
			 const std::string& grid,
                         const std::string& name,
                         IterT const& begin,
                         IterT const& end,
                         mpi::comm const& comm )
   {
      typedef typename IterT::value_type::value_type value_type;
      typedef mpl::map< mpl::pair<int, mpl::int_<0> >,
                        mpl::pair<unsigned, mpl::int_<1> >,
                        mpl::pair<long, mpl::int_<2> >,
                        mpl::pair<unsigned long, mpl::int_<3> >,
                        mpl::pair<float, mpl::int_<4> >,
                        mpl::pair<double, mpl::int_<5> > > type_map;

      const char* type;
      switch( mpl::at<type_map,value_type>::type::value ) {
         case 0:
         case 1:
         case 2:
         case 3:
            type = "Int";
            break;
         case 4:
         case 5:
            type = "Float";
            break;
      }

      std::string path = "/";
      path += grid + "/" + name;
      unsigned dim = begin->end() - begin->begin();
      unsigned n_globals = comm.all_reduce( end - begin );
      write_xdmf_xml_attribute( xml_file, name, h5_filename, path, type, dim, n_globals, true );
   }

}

#endif

#include "libhpc/system/stream.hh"
#include "xdmf_writer.hh"

namespace hpc {

   xdmf_writer::xdmf_writer()
   {
   }

   xdmf_writer::xdmf_writer( const std::string& base_filename,
                             const std::string& name,
                             const kdtree<>& kdt )
   {
      this->open( base_filename, name, kdt );
   }

   xdmf_writer::~xdmf_writer()
   {
      close();
   }

   void
   xdmf_writer::open( const std::string& base_filename,
                      const std::string& name,
                      const kdtree<>& kdt )
   {
      std::string xml_filename = base_filename + ".xmf";
      this->_h5_filename = base_filename + ".h5";

      this->_xml_file.open( xml_filename, std::ios::out );
      write_xdmf_xml_open( this->_xml_file, this->_h5_filename, name, kdt );

      const mpi::comm& comm = kdt.comm();
      this->_h5_file.open( this->_h5_filename, H5F_ACC_TRUNC, comm );
      write_xdmf_h5( this->_h5_file, name, kdt );
   }

   void
   xdmf_writer::close()
   {
      write_xdmf_xml_close( this->_xml_file );
      this->_xml_file.close();
      this->_h5_file.close();
   }

   void
   write_xdmf( const std::string& base_filename,
               const std::string& name,
               const kdtree<>& kdt )
   {
      std::string xml_filename = base_filename + ".xmf";
      std::string h5_filename = base_filename + ".h5";
      write_xdmf_xml( xml_filename, h5_filename, name, kdt );
      write_xdmf_h5( h5_filename, name, kdt );
   }

   void
   write_xdmf_h5( const std::string& h5_filename,
                  const std::string& name,
                  const kdtree<>& kdt )
   {
      const mpi::comm& comm = kdt.comm();
      h5::file h5_file( h5_filename, H5F_ACC_TRUNC, comm );
      write_xdmf_h5( h5_file, name, kdt );
   }

   void
   write_xdmf_h5( h5::file& h5_file,
                  const std::string& name,
                  const kdtree<>& kdt )
   {
      const mpi::comm& comm = kdt.comm();
      unsigned dim = kdt.n_dims();
      unsigned num_local_cells = kdt.n_leafs();
      unsigned num_global_cells = comm.all_reduce( num_local_cells );
      unsigned num_cell_points = 1 << dim;

      {
         std::vector<hsize_t> space_dims( 2 );
         space_dims[0] = num_global_cells;
         space_dims[1] = num_cell_points;
         h5::dataspace file_space( space_dims );
         h5::dataset topology_set;
         topology_set.create( h5_file, "topology", h5::datatype::native_ulong, file_space );
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
         geometry_set.create( h5_file, "geometry", h5::datatype::native_double, file_space );
         space_dims[0] = num_cell_points;
         h5::dataspace mem_space( space_dims );
         mem_space.select_all();
         std::vector<double> buffer( num_cell_points*dim );
         std::vector<hsize_t> start( 2 ), count( 2 );
         count[0] = num_cell_points;
         count[1] = dim;
         start[1] = 0;
         unsigned global_offs = comm.scan( num_local_cells );
         std::vector<double> bnds( 2*dim );
         for( unsigned ii = global_offs; ii < global_offs + num_local_cells; ++ii )
         {
            kdt.leaf_geometry( ii - global_offs, bnds.begin() );
            int cur_buf_pos = 0;
            buffer[cur_buf_pos++] = bnds[0]; // lo[0];
            buffer[cur_buf_pos++] = bnds[2]; // lo[1];
            if( dim == 3 )
               buffer[cur_buf_pos++] = bnds[4]; // lo[2];
            buffer[cur_buf_pos++] = bnds[1]; // hi[0];
            buffer[cur_buf_pos++] = bnds[2]; // lo[1];
            if( dim == 3 )
               buffer[cur_buf_pos++] = bnds[4]; // lo[2];
            buffer[cur_buf_pos++] = bnds[1]; // hi[0];
            buffer[cur_buf_pos++] = bnds[3]; // hi[1];
            if( dim == 3 )
               buffer[cur_buf_pos++] = bnds[4]; // lo[2];
            buffer[cur_buf_pos++] = bnds[0]; // lo[0];
            buffer[cur_buf_pos++] = bnds[3]; // hi[1];
            if( dim == 3 )
               buffer[cur_buf_pos++] = bnds[4]; // lo[2];
            if( dim == 3 ) {
               buffer[cur_buf_pos++] = bnds[0]; // lo[0];
               buffer[cur_buf_pos++] = bnds[2]; // lo[1];
               buffer[cur_buf_pos++] = bnds[5]; // hi[2];
               buffer[cur_buf_pos++] = bnds[1]; // hi[0];
               buffer[cur_buf_pos++] = bnds[2]; // lo[1];
               buffer[cur_buf_pos++] = bnds[5]; // hi[2];
               buffer[cur_buf_pos++] = bnds[1]; // hi[0];
               buffer[cur_buf_pos++] = bnds[3]; // hi[1];
               buffer[cur_buf_pos++] = bnds[5]; // hi[2];
               buffer[cur_buf_pos++] = bnds[0]; // lo[0];
               buffer[cur_buf_pos++] = bnds[3]; // hi[1];
               buffer[cur_buf_pos++] = bnds[5]; // hi[2];
            }
            start[0] = ii*num_cell_points;
            file_space.select_hyperslab<std::vector<hsize_t> >( H5S_SELECT_SET, count, start );
            geometry_set.write( buffer.data(), h5::datatype::native_double, mem_space, file_space, comm );
         }
      }
      {
         std::vector<hsize_t> space_dims( 1 );
         space_dims[0] = num_global_cells;
         h5::dataspace file_space( space_dims );
         h5::dataset topology_set;
         topology_set.create( h5_file, "ownership", h5::datatype::native_int, file_space );
         space_dims[0] = 1;
         h5::dataspace mem_space( space_dims );
         mem_space.select_all();
         std::vector<hsize_t> start( 1 ), count( 1 );
         count[0] = 1;
         unsigned global_offs = comm.scan( num_local_cells, MPI_SUM, true );
         int buffer = comm.rank();
         for( unsigned ii = global_offs; ii < global_offs + num_local_cells; ++ii ) {
            start[0] = ii;
            file_space.select_hyperslab<std::vector<hsize_t> >( H5S_SELECT_SET, count, start );
            topology_set.write( &buffer, h5::datatype::native_int, mem_space, file_space, comm );
         }
      }
   }

   void
   write_xdmf_xml( const std::string& xml_filename,
                   const std::string& h5_filename,
                   const std::string& name,
                   const kdtree<>& kdt )
   {
      std::ofstream xml_file( xml_filename, std::ios::out );
      write_xdmf_xml_open( xml_file, h5_filename, name, kdt );
      write_xdmf_xml_close( xml_file );
   }

   void
   write_xdmf_xml_open( std::ofstream& xml_file,
                        const std::string& h5_filename,
                        const std::string& name,
                        const kdtree<>& kdt )
   {
      unsigned dim = kdt.n_dims();
      unsigned num_global_cells = kdt.comm().all_reduce( kdt.n_leafs() );
      unsigned num_cell_points = 1 << dim;
      write_xdmf_xml_begin( xml_file );
      write_xdmf_xml_domain_begin( xml_file );
      write_xdmf_xml_grid_begin( xml_file, name );
      write_xdmf_xml_topology( xml_file, h5_filename, dim, num_global_cells, num_cell_points );
      write_xdmf_xml_geometry( xml_file, h5_filename, dim, num_global_cells, num_cell_points );
      write_xdmf_xml_attribute( xml_file, "ownership", h5_filename, "/ownership", "Int", 1, num_global_cells, true );
   }

   void
   write_xdmf_xml_open_particles( std::ofstream& xml_file,
                                  const std::string& h5_filename,
                                  const std::string& name,
                                  unsigned n_parts,
                                  unsigned n_dims,
                                  mpi::comm const& comm )
   {
      write_xdmf_xml_begin( xml_file );
      write_xdmf_xml_domain_begin( xml_file );
      write_xdmf_xml_grid_begin( xml_file, name );
      write_xdmf_xml_topology( xml_file, h5_filename, n_dims, n_parts, 1 );
      write_xdmf_xml_geometry( xml_file, h5_filename, n_dims, n_parts, 1 );
   }

   void
   write_xdmf_xml_close( std::ofstream& xml_file )
   {
      write_xdmf_xml_grid_end( xml_file );
      write_xdmf_xml_domain_end( xml_file );
      write_xdmf_xml_end( xml_file );
   }

   void
   write_xdmf_xml_begin( std::ofstream& xml_file )
   {
      xml_file << indent << "<Xdmf Version=\"2.0\">\n" << setindent( 2 );
   }

   void
   write_xdmf_xml_end( std::ofstream& xml_file )
   {
      xml_file << setindent( -2 ) << indent << "</Xdmf>\n";
   }

   void
   write_xdmf_xml_domain_begin( std::ofstream& xml_file )
   {
      xml_file << indent << "<Domain>\n" << setindent( 2 );
   }

   void
   write_xdmf_xml_domain_end( std::ofstream& xml_file )
   {
      xml_file << setindent( -2 ) << indent << "</Domain>\n";
   }

   void
   write_xdmf_xml_grid_begin( std::ofstream& xml_file,
                              const std::string& name )
   {
      xml_file << indent << "<Grid Name=\"" << name << "\" GridType=\"Uniform\">\n" << setindent( 2 );
   }

   void
   write_xdmf_xml_grid_end( std::ofstream& xml_file )
   {
      xml_file << setindent( -2 ) << indent << "</Grid>\n";
   }

   void
   write_xdmf_xml_topology( std::ofstream& xml_file,
                            const std::string& h5_filename,
                            int dim,
                            unsigned num_global_cells,
                            unsigned num_cell_points )
   {
      ASSERT( dim > 0 && dim < 4 );
      xml_file << indent << "<Topology TopologyType=\"";
      if( num_cell_points == 1 )
         xml_file << "PolyVertex";
      else if( num_cell_points == 2 && dim == 1 )
         xml_file << "PolyLine";
      else if( num_cell_points == 4 && dim == 2 )
         xml_file << "Quadrilateral";
      else if( num_cell_points == 8 && dim == 3 )
         xml_file << "Hexahedron";
      else
      {
         EXCEPT( "Failed to deduce topology." );
      }
      xml_file << "\" NumberOfElements=\"" << num_global_cells << "\">\n" << setindent( 2 );

      xml_file << indent << "<DataItem Format=\"HDF\" NumberType=\"UINT\" ";
      xml_file << "Dimensions=\"" << num_global_cells << " " << num_cell_points << "\">\n" << setindent( 2 );
      xml_file << indent << h5_filename << ":/topology\n";
      xml_file << setindent( -2 ) << indent << "</DataItem>\n";

      xml_file << setindent( -2 ) << indent << "</Topology>\n";
   }

   void
   write_xdmf_xml_geometry( std::ofstream& xml_file,
                            const std::string& h5_filename,
                            int dim,
                            unsigned num_global_cells,
                            unsigned num_cell_points )
   {
      static const char* geometry_map[4] = {
         "",
         "",
         "XY",
         "XYZ"
      };

      ASSERT( dim > 1 && dim < 4 );
      xml_file << indent << "<Geometry GeometryType=\"" << geometry_map[dim] << "\">\n" << setindent( 2 );

      xml_file << indent << "<DataItem Format=\"HDF\" NumberType=\"Float\" ";
      xml_file << "Dimensions=\"" << num_global_cells*num_cell_points << " " << dim << "\">\n" << setindent( 2 );
      xml_file << indent << h5_filename << ":/geometry\n";
      xml_file << setindent( -2 ) << indent << "</DataItem>\n";

      xml_file << setindent( -2 ) << indent << "</Geometry>\n";
   }

   void
   write_xdmf_xml_attribute( std::ofstream& xml_file,
                             const std::string& name,
                             const std::string& h5_filename,
                             const std::string& h5_path,
                             const std::string& type,
                             unsigned dim,
                             unsigned num_global_cells,
                             bool cell_centered )
   {
      unsigned pv_dim = (dim == 2) ? 3 : dim;

      const char* center = cell_centered ? "Cell" : "Node";
      const char* attr_type = (dim == 1) ? "Scalar" : "Vector";
      xml_file << indent << "<Attribute Name=\"" << name << "\" AttributeType=\"" << attr_type << "\" ";
      xml_file << "Center=\"" << center << "\">\n" << setindent( 2 );

      xml_file << indent << "<DataItem Format=\"HDF\" NumberType=\"" + type + "\" ";
      xml_file << "Dimensions=\"" << num_global_cells << " " << pv_dim << "\">\n" << setindent( 2 );
      xml_file << indent << h5_filename << ":" << h5_path << "\n";
      xml_file << setindent( -2 ) << indent << "</DataItem>\n";

      xml_file << setindent( -2 ) << indent << "</Attribute>\n";
   }

}

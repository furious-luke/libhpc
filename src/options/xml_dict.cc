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

#ifdef HAVE_PUGIXML

#include <fstream>
#include "libhpc/debug/except.hh"
#include "xml_dict.hh"
#include "bad_option.hh"

using namespace pugi;

namespace hpc {
   namespace options {

      xml_dict::xml_dict()
         : _sep( ":" ),
           _root( NULL )
      {
      }

      xml_dict::xml_dict( xml_node root )
         : _sep( ":" ),
           _root( root )
      {
      }

      xml_dict::xml_dict( std::istream& strm,
                          const hpc::string& xpath_root )
         : _sep( ":" ),
           _root( 0 )
      {
         read( strm, xpath_root );
      }

      xml_dict::xml_dict( const xml_dict& src )
         : _sep( src._sep ),
           _root( 0 )
      {
         _doc.reset( src._doc );
         if( src._root == src._doc )
            _root = _doc;
         else if( src._root )
            _root = _find_root( _doc, src._root.path() );
      }

      xml_dict::~xml_dict()
      {
      }

      void
      xml_dict::read( const hpc::string& filename,
                      const hpc::string& path )
      {
         std::ifstream file( filename.c_str(), std::fstream::in );
         read( file, path, filename );
      }

      void
      xml_dict::read( std::istream& stream,
                      const hpc::string& xpath_root,
                      const hpc::string& filename )
      {
         if( !_root )
         {
            xml_parse_result result = _doc.load( stream );
            EXCEPTAS( result, bad_xml, "Error: Couldn't read XML from file: ", filename );
            _root = _find_root( _doc, xpath_root );
         }
         else
            _merge( stream, xpath_root, filename );
      }

      bool
      xml_dict::has( const hpc::string& path ) const
      {
         return _get_node( path, false );
      }

      xpath_node_set
      xml_dict::get_nodes( const hpc::string& xpath ) const
      {
         return _root.select_nodes( xpath.c_str() );
      }

      xpath_node
      xml_dict::get_node( const hpc::string& xpath ) const
      {
         return _root.select_single_node( xpath.c_str() );
      }

      xml_node
      xml_dict::_find_root( xml_node& node,
                            const hpc::string& xpath_root ) const
      {
         if( !xpath_root.empty() )
         {
            xpath_node root = node.select_single_node( xpath_root.c_str() );
            ASSERT( root, "XPath root does not exist." );
            return root.node();
         }
         else
            return node;
      }

      void
      xml_dict::_merge( std::istream& stream,
                        const hpc::string& path,
                        const hpc::string& filename )
      {
         xml_document doc;
         xml_parse_result result = doc.load( stream );
         EXCEPTAS( result, bad_xml, "Error: Couldn't read XML from file: ", filename );
         xml_node root = _find_root( doc, path );
         for( xml_node_iterator it = root.begin(); it != root.end(); ++it )
            _merge_node( _root, *it );
      }

      void
      xml_dict::_merge_node( xml_node merge_into,
                             xml_node merge_from )
      {
         if( merge_from.type() == node_element )
         {
            xml_node child = merge_into.child( merge_from.name() );
            if( child )
            {
               for( xml_node_iterator it = merge_from.begin(); it != merge_from.end(); ++it )
                  _merge_node( child, *it );
            }
            else
               merge_into.append_copy( merge_from );
         }
         else if( merge_from.type() == node_pcdata )
         {
            // Search for the first pcdata element.
            for( xml_node_iterator it = merge_into.begin(); it != merge_into.end(); ++it )
            {
               if( it->type() == node_pcdata )
               {
                  it->set_value( merge_from.value() );
                  return;
               }
            }
         }
      }

      xml_node
      xml_dict::_get_node( const hpc::string& path,
                           bool except ) const
      {
         hpc::string xpath = _xform_path( path );
         xpath_node node = _root.select_single_node( xpath.c_str() );
         if( !node )
         {
            EXCEPTAS( !except, bad_option, "Error: Couldn't find option in XML dictionary: ", path );
            return (xml_node)0;
         }
         return node.node();
      }

      hpc::string
      xml_dict::_xform_path( const hpc::string& path ) const
      {
         hpc::string new_path = hpc::string( "./" ) + path;
         for( unsigned ii = 0; ii < new_path.size(); ++ii )
         {
            if( new_path[ii] == ':' )
               new_path[ii] = '/';
         }
         return new_path;
      }

      template<>
      hpc::string
      xml_dict::_coerce( const hpc::string& value ) const
      {
         return value;
      }

   }
}

#endif

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

#include <fstream>
#include "libhpc/debug/assert.hh"
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

      xml_dict::~xml_dict()
      {
      }

      void
      xml_dict::read( const hpc::string& filename,
                      const hpc::string& path )
      {
         std::ifstream file( filename, std::fstream::in );
         read( file, path );
      }

      void
      xml_dict::read( std::istream& stream,
                      const hpc::string& path )
      {
         if( !_root )
         {
            xml_parse_result result = _doc.load( stream );
            ASSERT( result, "Error reading XML." );
            _root = _find_root( _doc, path );
         }
         else
            _merge( stream, path );
      }

      xml_node
      xml_dict::_find_root( xml_node& node,
                            const string& path ) const
      {
         if( !path.empty() )
         {
            xpath_node root = node.select_single_node( path.c_str() );
            ASSERT( root );
            return root.node();
         }
         else
            return node;
      }

      void
      xml_dict::_merge( std::istream& stream,
                        const string& path )
      {
         xml_document doc;
         xml_parse_result result = doc.load( stream );
         ASSERT( result, "Error reading XML." );
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
      xml_dict::_get_node( const string& path ) const
      {
         string xpath = _xform_path( path );
         xpath_node node = _root.select_single_node( xpath.c_str() );
         if( !node )
            throw bad_option( path );
         return node.node();
      }

      string
      xml_dict::_xform_path( const string& path ) const
      {
         string new_path = string( "/" ) + path;
         for( unsigned ii = 0; ii < new_path.size(); ++ii )
         {
            if( new_path[ii] == ':' )
               new_path[ii] = '/';
         }
         return new_path;
      }

   }
}

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

#include "xml.hh"

using namespace pugi;

namespace hpc {
   namespace options {

      xml::xml()
         : _sep( ":" )
      {
      }

      xml::~xml()
      {
      }

      void
      xml::read( const hpc::string& filename,
                 dictionary& dict,
                 const hpc::string& path )
      {
         std::ifstream file( filename, std::fstream::in );
         read( file, dict, path );
      }

      void
      xml::read( std::istream& strm,
                 dictionary& dict,
                 const hpc::string& path )
      {
         // Parse the document.
         xml_document doc;
         xml_parse_result result = doc.load( strm );
         ASSERT( result );

         // If we were given a path, locate it.
         if( !path.empty() )
         {
            xpath_node_set nodes = doc.select_nodes( path.c_str() );
            for( const xpath_node* it = nodes.begin(); it != nodes.end(); ++it )
            {
               xml_node cur = it->node();
               _iter_node( cur, "", dict );
            }
         }

         // Process each root node.
         else
         {
            for( xml_node_iterator it = doc.begin(); it != doc.end(); ++it )
               _iter_node( *it, "", dict );
         }
      }

      void
      xml::write( const hpc::string& filename,
                  const dictionary& dict )
      {
         std::ofstream file( filename, std::fstream::out );
         write( file, dict );
      }

      void
      xml::write( std::ostream& strm,
                  const dictionary& dict )
      {
         // Iterate over dictionary.
         xml_document doc;
         _iter_dict( doc, dict );

         // Dump document to stream.
         doc.save( strm );
      }

      void
      xml::_iter_node( xml_node& node,
                       hpc::string name,
                       dictionary& dict )
      {
         // If this node is PCDATA node, then try and store the value.
         if( node.type() == node_pcdata )
         {
            if( dict.has_option( name ) )
               dict[name] = node.value();
         }

         // If not, keep walking.
         else
         {
            // Process the name.
            hpc::string sub_name;
            if( !name.empty() )
               sub_name = name + _sep;
            sub_name += _node_name( node );

            // Iterate over each child.
            for( xml_node_iterator it = node.begin(); it != node.end(); ++it )
               _iter_node( *it, sub_name, dict );
         }
      }

      void
      xml::_iter_dict( xml_node& node,
                       const dictionary& dict )
      {
         for( auto it = dict.options_begin(); it != dict.options_end(); ++it )
         {
            // Don't store if it's using the default value.
            if( !(*it)->has_value() )
               continue;

            xml_node new_node = node.append_child( (*it)->name().c_str() );
            hpc::string val_str = (*it)->store();
            new_node.append_child( node_pcdata ).set_value( val_str.c_str() );
         }

         for( auto it = dict.dicts_begin(); it != dict.dicts_end(); ++it )
         {
            xml_node new_node = node.append_child( (*it)->prefix().c_str() );
            _iter_dict( new_node, **it );
         }
      }

      hpc::string
      xml::_node_name( xml_node& node ) const
      {
         // Check if there is an attribute called "name".
         for( xml_attribute_iterator it = node.attributes_begin(); it != node.attributes_end(); ++it )
         {
            hpc::string name = it->name();
            std::transform( name.begin(), name.end(), name.begin(), ::tolower );
            if( name == "name" )
               return it->value();
         }

         // If not, use the node name.
         return node.name();
      }
   }
}

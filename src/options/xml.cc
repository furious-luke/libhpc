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

      xml::xml( bool use_name_attr )
         : format(),
           _sep( ":" ),
	   _use_name_attr( use_name_attr )
      {
      }

      xml::~xml()
      {
      }

      void
      xml::parse( dictionary& dict,
		  const hpc::string& filename )
      {
	 read( filename, dict );
      }

      void
      xml::read( const hpc::string& filename,
                 dictionary& dict,
                 const hpc::string& path )
      {
         std::ifstream file( filename.c_str(), std::fstream::in );
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
                  const dictionary& dict,
		  bool all )
      {
         std::ofstream file( filename.c_str(), std::fstream::out );
         write( file, dict, all );
      }

      void
      xml::write( std::ostream& strm,
                  const dictionary& dict,
		  bool all )
      {
         // Iterate over dictionary.
         xml_document doc;
         _iter_dict( doc, dict, all );

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

            // Is the name we're looking at a list option?
            if( dict.has_option( sub_name ) && dict[sub_name].is_list() )
            {
               // Process each individual child, but no deeper.
               for( xml_node_iterator it = node.begin(); it != node.end(); ++it )
               {
                  if( it->first_child() && it->first_child().type() == node_pcdata )
                     dict[sub_name].parse( it->first_child().value() );
               }
            }
            else
            {
               // Iterate over each child.
               for( xml_node_iterator it = node.begin(); it != node.end(); ++it )
                  _iter_node( *it, sub_name, dict );
            }
         }
      }

      void
      xml::_iter_dict( xml_node& node,
                       const dictionary& dict,
		       bool all )
      {
         for( vector<shared_ptr<option_base> >::const_iterator it = dict.options_cbegin();
              it != dict.options_cend();
              ++it )
         {
            // Don't store if it's using the default value, unless the
	    // user has requested everything be dumped.
            if( !all && !(*it)->has_value() )
               continue;

	    // If there is a description add that first.
	    if( !(*it)->description().empty() )
	       node.append_child( node_comment ).set_value( (*it)->description().c_str() );

            // Create the new node.
            xml_node new_node = node.append_child( (*it)->name().c_str() );

            // Is this a list we're looking at?
            if( (*it)->is_list() )
            {
               _cur_node = &new_node;
               ((shared_ptr<option_base>&)(*it))->store_visit( *this );
            }

            // If not just store as usual.
            else
            {
	       // Check if there is an actual value.
	       hpc::string val_str;
	       if( (*it)->has_value_or_default() )
		  val_str = (*it)->store();

	       // Write the node.
               new_node.append_child( node_pcdata ).set_value( val_str.c_str() );
            }
         }

         for( vector<shared_ptr<dictionary> >::const_iterator it = dict.dicts_begin();
              it != dict.dicts_end();
              ++it )
         {
            xml_node new_node = node.append_child( (*it)->prefix().c_str() );
            _iter_dict( new_node, **it, all );
         }
      }

      hpc::string
      xml::_node_name( xml_node& node ) const
      {
	 // Have we enabled using attribute "name"?
	 if( _use_name_attr )
	 {
	    // Check if there is an attribute called "name".
	    for( xml_attribute_iterator it = node.attributes_begin(); it != node.attributes_end(); ++it )
	    {
	       hpc::string name = it->name();
	       std::transform( name.begin(), name.end(), name.begin(), ::tolower );
	       if( name == "name" )
		  return it->value();
	    }
	 }

         // If not, use the node name.
         return node.name();
      }

      void
      xml::start_list( const hpc::string& name )
      {
      }

      void
      xml::add_list_item( const hpc::string& value )
      {
         _cur_node->append_child( "item" ).append_child( node_pcdata ).set_value( value.c_str() );
      }

      void
      xml::end_list()
      {
      }
   }
}

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

#include "cmd_line.hh"

namespace hpc {
   namespace options {

      const char* cmd_line::default_equality_symbol = "=";

      cmd_line::cmd_line()
         : format(),
           _sep( default_separator ),
	   _eq_sym( default_equality_symbol ),
	   _short_pre( "-" ),
	   _long_pre( "--" )
      {
      }

      void
      cmd_line::parse( dictionary& dict,
		       int argc,
		       const char* argv[] )
      {
	 // Reset positionals.
	 dict.reset_positionals();

	 // Traverse the argument list in order.
	 unsigned ii = 1;
	 while( ii < argc )
	 {
	    // Is this a short option?
	    const char* tok;
	    if( (tok = _is_short( argv[ii] )) )
	    {
	       // Make sure we have this option before anything
	       // else.
	       if( dict.has_short_option( tok ) )
	       {
		  // If this is a boolean option flag it as true without
		  // consuming anything else.
		  if( dict[dict.short_to_long( tok )].is_boolean() )
		     dict[dict.short_to_long( tok )] = "true";
		  else
		  {
		     // Must have another argument.
		     ++ii;
		     if( ii < argc )
			dict[dict.short_to_long( tok )] = argv[ii];
		     else
		     {
			// TODO: Add an error.
		     }
		  }
	       }
	       else
	       {
		  // TODO: Add an error.
	       }
	    }

	    // Or a long option?
	    else if( (tok = _is_long( argv[ii] )) )
	    {
	       // Split the long option on the equality symbol.
	       const char* val = strstr( tok, _eq_sym.c_str() );
	       if( val )
	       {
		  // Split the argument into name and value.
		  hpc::string name( tok, val++ );

		  // Save.
		  if( dict.has_option( name ) )
		     dict[name] = val;
		  else
		  {
		     // TODO: Add an error.
		  }
	       }

	       // If there's no value, check if it's a boolean option.
	       else if( dict.has_option( tok ) && dict[tok].is_boolean() )
		     dict[tok] = "true";
	       else
	       {
		  // TODO: Add an error.
	       }
	    }

	    // Or a positional.
	    else
	    {
	       dict.set_next_positional( argv[ii] );
	    }

	    ++ii;
	 }
      }

      void
      cmd_line::start_list( const hpc::string& name )
      {
      }

      void
      cmd_line::add_list_item( const hpc::string& value )
      {
      }

      void
      cmd_line::end_list()
      {
      }

      const char*
      cmd_line::_is_short( const char* opt )
      {
	 unsigned ii = 0, jj = 0;
	 while( ii < _short_pre.size() && opt[jj] != 0 )
	 {
	    if( _short_pre[ii++] != opt[jj++] )
	       return NULL;
	 }
	 if( ii == _short_pre.size() )
	 {
	    if( ii >= _long_pre.size() )
	       return opt + _short_pre.size();
	    while( ii < _long_pre.size() && opt[jj] != 0 )
	    {
	       if( _long_pre[ii++] != opt[jj++] )
		  return opt + _short_pre.size();
	    }
	 }
	 return NULL;
      }

      const char*
      cmd_line::_is_long( const char* opt )
      {
	 unsigned ii = 0, jj = 0;
	 while( ii < _long_pre.size() && opt[jj] != 0 )
	 {
	    if( _long_pre[ii++] != opt[jj++] )
	       return NULL;
	 }
	 if( ii == _long_pre.size() )
	 {
	    if( ii >= _short_pre.size() )
	       return opt + _long_pre.size();
	    while( ii < _short_pre.size() && opt[jj] != 0 )
	    {
	       if( _short_pre[ii++] != opt[jj++] )
		  return opt + _long_pre.size();
	    }
	 }
	 return NULL;
      }

   }
}

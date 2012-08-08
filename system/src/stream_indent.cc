#include "libhpc/debug/debug.hh"
#include "libhpc/memory/memory.hh"
#include "stream_indent.hh"
#include <map>

namespace hpc {
   namespace impl {
      std::map<std::ostream*,int> curindent;
   }

   setindent_t
   setindent( int indent )
   {
      setindent_t ind;
      ind.indent = indent;
      return ind;
   }

   std::ostream&
   operator<<( std::ostream& strm,
               setindent_t si )
   {
      int& val = impl::curindent[&strm];
      val += si.indent;
      ASSERT( val >= 0 );
      if( val == 0 )
         impl::curindent.erase( &strm );
      return strm;
   }

   std::ostream&
   indent( std::ostream& strm )
   {
      std::map<std::ostream*,int>::const_iterator it = impl::curindent.find( &strm );
      int val = 0;
      if( it != impl::curindent.end() )
         val = it->second;
      for( int ii = 0; ii < val; ++ii )
         strm << " ";
      return strm;
   }
}

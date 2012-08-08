#ifndef debug_tracer_hh
#define debug_tracer_hh

#include <sstream>

namespace debug {

   class tracer {
   public:

      static const int max_size = 16384;

      tracer();

      ~tracer();

      void
      clear();

      void
      push( const char* id );

      void
      pop();

      int
      buffer_size( int indent=0 ) const;

      int
      print( int indent=0 ) const;

      int
      sprint( char* buf,
	      int indent=0 ) const;

      friend std::ostream&
      operator<<( std::ostream& strm,
		  const tracer& obj );

   private:
      int _num_entries;
      int _pos;
      char _stack[max_size + 1];
   };
}

#endif

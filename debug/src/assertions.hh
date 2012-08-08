#ifndef debug_assertions_hh
#define debug_assertions_hh

#ifndef NDEBUG

#include <iostream>
#include <string>
#include <exception>

namespace debug {

   class assertion
      : public std::exception
   {
   public:

      assertion( const char* msg=NULL ) throw();

      assertion( const assertion& ass );

      virtual ~assertion() throw();

      void
      details( const char* file,
	       int line,
	       const char* expr ) throw();

      virtual const char*
      what() const throw();

   protected:

      void write_buffer() throw();

   private:
      const char* _file;
      int _line;
      const char* _expr;
      char _msg[1024];
      char _buf[1024];
   };
}

#endif

#endif

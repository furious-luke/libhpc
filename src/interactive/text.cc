#ifdef HAVE_GLUT

#include "text.hh"

namespace hpc {
   namespace gl {

      void
      draw_text( const std::string& text,
                 GLfloat x,
                 GLfloat y,
                 void* font )
      {
         glRasterPos2f( x, y );
         for( char c : text )
            glutBitmapCharacter( font, c );
      }

   }
}

#endif

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
         for( std::string::const_iterator it = text.begin(); it != text.end(); ++it )
            glutBitmapCharacter( font, *it );
      }

   }
}

#endif

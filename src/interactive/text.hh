#ifndef libhpc_interactive_text_hh
#define libhpc_interactive_text_hh

#ifdef HAVE_GLUT

#include <string>
#include <GL/glut.h>

namespace hpc {
   namespace gl {

      void
      draw_text( const std::string& text,
                 GLfloat x,
                 GLfloat y,
                 void* font = GLUT_BITMAP_9_BY_15 );

      template< class Iterator >
      Iterator
      draw_text_fit( Iterator start,
                     const Iterator& finish,
                     GLfloat x,
                     GLfloat y,
                     GLfloat width,
                     void* font = GLUT_BITMAP_9_BY_15 )
      {
         glRasterPos2f( x, y );
         GLfloat pos = 0;
         while( start != finish )
         {
            char c = *start;
            pos += glutBitmapWidth( font, c );
            if( pos > width )
               break;
            glutBitmapCharacter( font, c );
            ++start;
         }
         return start;
      }

      template< class String >
      void
      draw_text_cont( const String& str,
                      GLfloat x,
                      GLfloat& y,
                      GLfloat width,
                      void* font = GLUT_BITMAP_9_BY_15,
                      GLfloat line_height = 15 )
      {
         std::string::const_iterator pos = str.begin();
         while( pos != str.end() && y >= 0 )
         {
            pos = draw_text_fit( pos, str.end(), x, y, width );
            y -= line_height;
         }
      }

   }
}

#endif

#endif

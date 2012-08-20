#ifndef libhpc_sound_pcm_hh
#define libhpc_sound_pcm_hh

#include <alsa/asoundlib.h>
#include "libhpc/system/pipe.hh"

namespace hpc {
   namespace sound {

      class pcm
      {
      public:

         pcm();

         ~pcm();

         void
         open();

         void
         close();

         unsigned
         sample_rate() const;

         unsigned long
         period_size() const;

         long
         available_frames();

         const vector<unix::pipe>&
         poll_pipes() const;

      protected:

         void
         _set_hw_params();

         void
         _set_sw_params();

         void
         _get_poll_fds();

      protected:

         snd_pcm_t* _handle;
         unsigned _rate;
         snd_pcm_uframes_t _frames;
         vector<unix::pipe> _fds;
      };
   }
}

#endif

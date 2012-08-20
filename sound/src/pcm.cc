#include "libhpc/debug/debug.hh"
#include "pcm.hh"

namespace hpc {
   namespace sound {

      pcm::pcm()
      {
         open();
      }

      pcm::~pcm()
      {
         close();
      }

      void
      pcm::open()
      {
         INSIST( snd_pcm_open( &_handle, "default", SND_PCM_STREAM_PLAYBACK, 0 ), >= 0 );
         _set_hw_params();
         _set_sw_params();
         INSIST( snd_pcm_prepare( _handle ), >= 0 );
         _get_poll_fds();
      }

      void
      pcm::close()
      {
         INSIST( snd_pcm_drain( _handle ), >= 0 );
         INSIST( snd_pcm_close( _handle ), >= 0 );
      }

      unsigned
      pcm::sample_rate() const
      {
         return _rate;
      }

      unsigned long
      pcm::period_size() const
      {
         return _frames;
      }

      long
      pcm::available_frames()
      {
         snd_pcm_sframes_t res = snd_pcm_avail_update( _handle );
         ASSERT( res >= 0 || res == -EPIPE );
         return res;
      }

      const vector<unix::pipe>&
      pcm::poll_pipes() const
      {
         return _fds;
      }

      void
      pcm::_set_hw_params()
      {
         // Allocate hardware parameters and fill with defaults.
         snd_pcm_hw_params_t* params;
         INSIST( snd_pcm_hw_params_malloc( &params ), >= 0 );
         INSIST( snd_pcm_hw_params_any( _handle, params ), >= 0);

         // Set hardware values.
         INSIST( snd_pcm_hw_params_set_access( _handle, params, SND_PCM_ACCESS_RW_INTERLEAVED ), >= 0 );
         INSIST( snd_pcm_hw_params_set_format( _handle, params, SND_PCM_FORMAT_S16_LE ), >= 0 );
         INSIST( snd_pcm_hw_params_set_channels( _handle, params, 2 ), >= 0 );
         _rate = 44100;
         int dir;
         INSIST( snd_pcm_hw_params_set_rate_near( _handle, params, &_rate, &dir ), >= 0 );
         _frames = 1024;
         INSIST( snd_pcm_hw_params_set_period_size_near( _handle, params, &_frames, &dir ), >= 0 );
         INSIST( snd_pcm_hw_params( _handle, params ), >= 0 );

         // Calculate a buffer large enough to hold one period.
         INSIST( snd_pcm_hw_params_get_period_size( params, &_frames, &dir ), >= 0 );
         INSIST( snd_pcm_hw_params_get_rate( params, &_rate, &dir ), >= 0 );
         // size_t size = frames*4; /* 2 bytes/sample, 2 channels */
         // vector<char> buffer( size );

         // Free the parameters structure now.
         snd_pcm_hw_params_free( params );
      }

      void
      pcm::_set_sw_params()
      {
         snd_pcm_sw_params_t* params;
         INSIST( snd_pcm_sw_params_malloc( &params ), >= 0 );
         INSIST( snd_pcm_sw_params_current( _handle, params ), >= 0 );
         INSIST( snd_pcm_sw_params_set_avail_min( _handle, params, 4096 ), >= 0 );
         INSIST( snd_pcm_sw_params_set_start_threshold( _handle, params, 0U ), >= 0 );
         INSIST( snd_pcm_sw_params( _handle, params ), >= 0 );
         snd_pcm_sw_params_free( params );
      }

      void
      pcm::_get_poll_fds()
      {
         vector<struct pollfd> pollfds;
         pollfds.reallocate( snd_pcm_poll_descriptors_count( _handle ) );
         INSIST( snd_pcm_poll_descriptors( _handle, pollfds.data(), pollfds.size() ), == pollfds.size() );

         _fds.reallocate( pollfds.size() );
         for( unsigned ii = 0; ii < _fds.size(); ++ii )
            _fds[ii].set_fd( pollfds[ii].fd, false );
      }
   }
}

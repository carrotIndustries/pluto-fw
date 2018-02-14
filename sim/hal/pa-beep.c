#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <pulse/pulseaudio.h>

//based on https://www.freedesktop.org/wiki/Software/PulseAudio/Documentation/Developer/Clients/Samples/AsyncPlayback/

#define N_SAMPLES 65536

static int latency = 50000; // start latency in micro seconds
static short sampledata[N_SAMPLES];
static pa_buffer_attr bufattr;
static int underflows = 0;
static pa_sample_spec ss;

// This callback gets called when our context changes state.  We really only
// care about when it's ready or if it has failed
static void pa_state_cb(pa_context *c, void *userdata) {
  pa_context_state_t state;
  int *pa_ready = userdata;
  state = pa_context_get_state(c);
  switch  (state) {
    // These are just here for reference
  case PA_CONTEXT_UNCONNECTED:
  case PA_CONTEXT_CONNECTING:
  case PA_CONTEXT_AUTHORIZING:
  case PA_CONTEXT_SETTING_NAME:
  default:
    break;
  case PA_CONTEXT_FAILED:
  case PA_CONTEXT_TERMINATED:
    *pa_ready = 2;
    break;
  case PA_CONTEXT_READY:
    *pa_ready = 1;
    break;
  }
}

static volatile unsigned short freq = 0;

void beep_set_freq(unsigned short f) {
	freq = f;
}


static void stream_request_cb(pa_stream *s, size_t length, void *userdata) {
  pa_usec_t usec;
  int neg;
  pa_stream_get_latency(s,&usec,&neg);
  //printf("  latency %8d us %d\n",(int)usec, length);

  signed short *buf;
  size_t nb = length;
  pa_stream_begin_write(s, (void**)&buf, &nb);
  //printf("w %d %d\n", length, nb);

  static unsigned short phase = 0;
  int i = 0;
  while(i < nb/2) {
	  if(freq) {
		buf[i] = sampledata[phase];
		phase += freq*(65536./48000.);
	}
	else {
		buf[i] = 0;
		phase = 0;
	}

	  //printf("%d\n", buf[i]);
	  i++;
  }

  pa_stream_write(s, buf, nb, NULL, 0LL, PA_SEEK_RELATIVE);
}

static void stream_underflow_cb(pa_stream *s, void *userdata) {
  // We increase the latency by 50% if we get 6 underflows and latency is under 2s
  // This is very useful for over the network playback that can't handle low latencies
  printf("underflow\n");
  underflows++;
  if (underflows >= 6 && latency < 2000000) {
    latency = (latency*3)/2;
    bufattr.maxlength = pa_usec_to_bytes(latency,&ss);
    bufattr.tlength = pa_usec_to_bytes(latency,&ss);
    pa_stream_set_buffer_attr(s, &bufattr, NULL, NULL);
    underflows = 0;
    printf("latency increased to %d\n", latency);
  }
}

#define N_HARMONIC 10

void beep_startup() {
  pa_threaded_mainloop *pa_ml;
  pa_mainloop_api *pa_mlapi;
  pa_context *pa_ctx;
  pa_stream *playstream;
  int r;
  int pa_ready = 0;
  unsigned int a;
  double amp;

  // Create some data to play
  for (a=0; a<N_SAMPLES; a++) {
	amp = 0;
	for(unsigned int h = 1; h<(N_HARMONIC+1); h++) {
		amp += sin(h*M_PI*2*a*(1.0/N_SAMPLES));
	}
    sampledata[a] = amp * 32000.0/N_HARMONIC;
    //printf("%d \n", sampledata[a]);
  }
  // Create a mainloop API and connection to the default server
  pa_ml = pa_threaded_mainloop_new();
  pa_mlapi = pa_threaded_mainloop_get_api(pa_ml);
  pa_ctx = pa_context_new(pa_mlapi, "Pluto emulator");
  pa_context_connect(pa_ctx, NULL, 0, NULL);

  // This function defines a callback so the server will tell us it's state.
  // Our callback will wait for the state to be ready.  The callback will
  // modify the variable to 1 so we know when we have a connection and it's
  // ready.
  // If there's an error, the callback will set pa_ready to 2
  pa_context_set_state_callback(pa_ctx, pa_state_cb, &pa_ready);
  pa_threaded_mainloop_start(pa_ml);
  // We can't do anything until PA is ready, so just iterate the mainloop
  // and continue
  while (pa_ready == 0) {
    usleep(1000);
  }
  if (pa_ready == 2) {
    exit(-1);
  }

  ss.rate = 48000;
  ss.channels = 1;
  ss.format = PA_SAMPLE_S16NE;
  playstream = pa_stream_new(pa_ctx, "Playback", &ss, NULL);
  if (!playstream) {
    printf("pa_stream_new failed\n");
  }
  pa_stream_set_write_callback(playstream, stream_request_cb, NULL);
  pa_stream_set_underflow_callback(playstream, stream_underflow_cb, NULL);
  bufattr.fragsize = (uint32_t)-1;
  bufattr.maxlength = pa_usec_to_bytes(latency,&ss);
  bufattr.minreq = pa_usec_to_bytes(0,&ss);
  bufattr.prebuf = (uint32_t)-1;
  bufattr.tlength = pa_usec_to_bytes(latency,&ss);
  r = pa_stream_connect_playback(playstream, NULL, &bufattr,
                                 PA_STREAM_INTERPOLATE_TIMING
                                 |PA_STREAM_ADJUST_LATENCY
                                 |PA_STREAM_AUTO_TIMING_UPDATE, NULL, NULL);
  if (r < 0) {
    // Old pulse audio servers don't like the ADJUST_LATENCY flag, so retry without that
    r = pa_stream_connect_playback(playstream, NULL, &bufattr,
                                   PA_STREAM_INTERPOLATE_TIMING|
                                   PA_STREAM_AUTO_TIMING_UPDATE, NULL, NULL);
  }
  if (r < 0) {
    printf("pa_stream_connect_playback failed\n");
    exit(-1);
  }
}

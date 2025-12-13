export module audio;
import hai;

export namespace audio {
  void init();
  void play(const hai::array<float> & samples);

  extern bool enabled;
  void interrupt();
}

#ifndef LECO_TARGET_WASM
#pragma leco add_impl audio_impl
#else
namespace audio {
  void init() {}
  void play(const hai::array<float> & samples) {}
  bool enabled = false;
  void interrupt() {}
}
#endif

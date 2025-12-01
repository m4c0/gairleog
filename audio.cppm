export module audio;
import file;
import hai;
import siaudio;
import silog;

export namespace audio {
  void play(const hai::array<float> & samples);

  extern bool enabled;
  void interrupt();
}

module : private;

bool audio::enabled = true;
void audio::interrupt() {}

static hai::array<float> g_playing {};
static volatile unsigned g_ptr = 0;
void audio::play(const hai::array<float> & samples) {
  g_ptr = ~0;
  g_playing.set_capacity(samples.size());
  for (auto i = 0; i < samples.size(); i++) g_playing[i] = samples[i];
  g_ptr = 0;
}

static constexpr const auto rate = 44100;
static void fill_buffer(float * data, unsigned samples) {
  float volume = audio::enabled ? 1 : 0;
  for (auto i = 0; i < samples; i++) {
    if (g_ptr > g_playing.size()) {
      *data++ = 0;
      continue;
    }
    *data++ = volume * g_playing[g_ptr];
    g_ptr = g_ptr + 1;
  }
}
const int i = [] {
  silog::info("Starting audio");
  siaudio::filler(fill_buffer);
  siaudio::rate(rate);
  return 0;
}();

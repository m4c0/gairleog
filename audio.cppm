export module audio;
import file;
import hai;
import mtx;
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

static mtx::mutex g_mtx {};
static hai::array<float> g_playing {};
static unsigned g_ptr = 0;
void audio::play(const hai::array<float> & samples) {
  mtx::lock l { &g_mtx };

  g_playing.set_capacity(samples.size());
  for (auto i = 0; i < samples.size(); i++) g_playing[i] = samples[i];
  g_ptr = 0;
}

static constexpr const auto rate = 44100;
static void fill_buffer(float * data, unsigned samples) {
  mtx::lock l { &g_mtx };

  float volume = audio::enabled ? 1 : 0;
  for (auto i = 0; i < samples; i++) {
    *data++ = g_ptr > g_playing.size() ? 0 : volume * g_playing[g_ptr++];
  }
}
const int i = [] {
  silog::info("Starting audio");
  siaudio::filler(fill_buffer);
  siaudio::rate(rate);
  return 0;
}();

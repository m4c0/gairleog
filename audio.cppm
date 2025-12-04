export module audio;
import file;
import hai;
import mtx;
import siaudio;
import silog;

export namespace audio {
  void init();
  void play(const hai::array<float> & samples);

  extern bool enabled;
  void interrupt();
}

module : private;

bool audio::enabled = true;
void audio::interrupt() {}

static mtx::mutex g_mtx {};
static float * g_sample_it {};
static float * g_sample_end {};
void audio::play(const hai::array<float> & samples) {
  mtx::lock l { &g_mtx };
  g_sample_it = samples.begin();
  g_sample_end = samples.end();
}

static constexpr const auto rate = 44100;
static void fill_buffer(float * data, unsigned samples) {
  mtx::lock l { &g_mtx };

  float volume = audio::enabled ? 1 : 0;
  for (auto i = 0; i < samples; i++) {
    *data++ = g_sample_it >= g_sample_end ? 0 : volume * *g_sample_it++;
  }
}
void audio::init() {
  silog::info("Starting audio");
  siaudio::filler(fill_buffer);
  siaudio::rate(rate);
}

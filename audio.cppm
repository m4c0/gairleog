export module audio;
import file;
import siaudio;
import silog;

export namespace audio {
  enum class source {
    eat,
    hit,
    miss,
    pick,
    walk,
  };
  void play(source);

  extern bool enabled;
  void interrupt();
}

module : private;

bool audio::enabled = true;
void audio::interrupt() {}

static constexpr const auto rate = 44100;
static void fill_buffer(float * data, unsigned samples) {
  static unsigned xxx = 0;
  float volume = audio::enabled ? 1 : 0;
  for (auto i = 0; i < samples; i++, xxx++) {
    float sample = 0.3 * (xxx % 1000) / 1000.0f;
    data[i] = volume * sample;
  }
}
const int i = [] {
  silog::info("Starting audio");
  siaudio::filler(fill_buffer);
  siaudio::rate(rate);
  return 0;
}();

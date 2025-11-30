export module audio;
import file;
import siaudio;

export namespace audio {
  enum class source {
    eat,
    hit,
    miss,
    pick,
    walk,
  };
  void play(source);

  bool enabled();
  void enabled(bool);

  void read(file::reader * r);
  void write(file::writer * w);
}

module : private;

bool g_enabled = true;

bool audio::enabled() { return g_enabled; }
void audio::enabled(bool e) { g_enabled = e; }

void audio::read(file::reader * r) {
  enabled(r->read<bool>());
}
void audio::write(file::writer * w) {
  w->write<bool>(enabled());
}

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

  extern bool enabled;
  void interrupt();

  void read(file::reader * r);
  void write(file::writer * w);
}

module : private;

bool audio::enabled = true;
void audio::interrupt() {}

void audio::read(file::reader * r) {
  enabled = r->read<bool>();
}
void audio::write(file::writer * w) {
  w->write<bool>(enabled);
}

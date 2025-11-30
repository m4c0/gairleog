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

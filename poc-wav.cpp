#pragma leco test

import jojo;
import jute;
import print;
import sv;

struct error {
  jute::heap msg;
};

[[noreturn]] static void erred(sv msg) {
  throw error { .msg { msg } };
}

class reader {
  const void * m_ptr;
  const void * m_end;

public:
  explicit reader(sv data) :
    m_ptr { data.begin() }
  , m_end { data.end() }
  {}

  template<typename T> [[nodiscard]] T read() {
    auto s = static_cast<const T *>(m_ptr);
    auto e = static_cast<const T *>(m_end);
    auto n = s + 1;
    if (n > e) erred("Premature EOF");
    m_ptr = n;
    return *s;
  }
  template<typename T> void skip() {
    auto _ = read<T>();
  }
};

static void run(sv data) {
  reader r { data };
  if (r.read<unsigned>() != 'FFIR') erred("Invalid WAV file");
  if (r.read<unsigned>() != data.size() - 8) erred("WAV file size does not match");
  if (r.read<unsigned>() != 'EVAW') erred("Unsupported RIFF file");

  if (r.read<unsigned>() != ' tmf') erred("WAV files should have a FMT chunk");
  if (r.read<unsigned>() != 16) erred("Unsupported WAV FMT size");
  if (r.read<short>() != 1) erred("Only PCM WAV is supported");
  auto channels = r.read<short>();
  if (channels != 1 && channels != 2) erred("Invalid number of channels in WAV");
  if (r.read<unsigned>() != 44100) erred("Expecting WAV with sample rate of 44100");
  r.skip<unsigned>(); // dwAvgBytesPerSec
  r.skip<short>(); // wBlockAlign
}

int main() try {
  auto data = jojo::slurp("sfx/01_human_atk_sword_1.wav");
  run(data);
} catch (const error & e) {
  errln(e.msg);
  return 1;
}

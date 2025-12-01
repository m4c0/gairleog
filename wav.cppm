export module wav;
import jute;
import sv;

namespace wav {
  export struct error {
    jute::heap msg;
  };
}

[[noreturn]] static void erred(sv msg) {
  throw wav::error { .msg { msg } };
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
  void skip(unsigned size) {
    auto s = static_cast<const char *>(m_ptr);
    auto e = static_cast<const char *>(m_end);
    auto n = s + size;
    if (n > e) erred("Premature EOF");
    m_ptr = n;
  }

  constexpr auto ptr() const { return static_cast<const char *>(m_ptr); }
};

static sv run(sv data) {
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
  if (r.read<short>() != 24) erred("Only 24 bits per sample is supported");

  while (r.read<unsigned>() != 'atad') {
    r.skip(r.read<unsigned>());
  }

  auto sz = r.read<unsigned>();
  auto p = r.ptr();
  if (p + sz > data.end()) erred("Data in WAV overflows");
  if (sz % (3 * 2)) erred("Data in WAV is misaligned");
  return { p, sz };
}

namespace wav {
  export sv run(sv data) { return ::run(data); }
}


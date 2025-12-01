export module wav;
import jute;
import hai;
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

struct sample {
  unsigned char left[3];
  unsigned char right[3];
};
static_assert(sizeof(sample) == 6);

// Ref on changing frequencies and stereo-to-mono conversion:
// https://dsp.stackexchange.com/questions/2484/converting-from-stereo-to-mono-by-averaging
static hai::array<float> convert(sv data) {
  auto p = reinterpret_cast<const sample *>(data.begin());
  hai::array<float> res { static_cast<unsigned>(data.size() / (3 * 2)) };
  for (auto & s : res) {
    auto [l, r] = *p++;
    signed ll = (l[2] << 24) | (l[1] << 16) | (l[0] << 8);
    signed rr = (r[2] << 24) | (r[1] << 16) | (r[0] << 8);
    float lf = ll / static_cast<float>(0xFFFFFF00);
    float rf = rr / static_cast<float>(0xFFFFFF00);
    s = (lf + rf) / 2.0f;
  }
  return res;
}

namespace wav {
  export hai::array<float> load(sv wave_file) {
    return convert(run(wave_file));
  }
}


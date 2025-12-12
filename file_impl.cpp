module;
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

module file;
import buoy;
import hai;
import hay;
import jute;

static_assert(sizeof(unsigned) == 4);
static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__);

namespace file {
  static const auto filename = buoy::path("gairleog", "save.dat");

  static inline auto open(const char * mode) {
    return fopen(filename.begin(), mode);
  }
  static inline auto close(FILE * f) {
    if (f) fclose(f);
  }

  class t {
    hay<FILE *, open, close> m_f;

  public:
    explicit t(const char * mode) : m_f { mode } {
      if (!static_cast<FILE *>(m_f)) throw error {};
    }

    operator FILE *() const { return m_f; }
  };

  reader::reader() : m_f { new t { "rb" } } {}
  reader::~reader() = default;
  void reader::read(void * data, unsigned size) {
    if (size == 0) return;
    if (fread(data, size, 1, *m_f) != 1) throw error {};
  }

  reader::operator bool() const { return *m_f; }

  writer::writer() : m_f { new t { "wb" } } {}
  writer::~writer() = default;

  void writer::write(const void * data, unsigned size) {
    if (size == 0) return;
    if (fwrite(data, size, 1, *m_f) != 1) throw error {};
  }
}

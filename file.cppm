module;
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

export module file;
import buoy;
import hay;

static_assert(sizeof(unsigned) == 4);
static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__);

namespace file {
  static const auto filename = buoy::path("gairleog", "save.dat");

  static constexpr const unsigned id = 'GAIR';
  static constexpr const unsigned version = 1001;

  static inline auto open(const char * mode) {
    return fopen(filename.begin(), mode);
  }
  static inline auto close(FILE * f) {
    if (f) fclose(f);
  }

  export struct error {};

  class t {
  protected:
    hay<FILE *, open, close> m_f;

  public:
    explicit t(const char * mode) : m_f { mode } {
      if (!static_cast<FILE *>(m_f)) throw error {};
    }
  };

  export class reader : public t {
  public:
    reader() : t { "rb" } {
    }
  };

  export class writer : public t {
    void write(const void * data, unsigned size) {
      if (fwrite(data, size, 1, m_f) != 1) throw error {};
    }

  public:
    writer() : t { "wb" } {
      write(id, &version, sizeof(version));
    }

    void write(unsigned id, const void * data, unsigned size) {
      write(&id, sizeof(id));
      write(&size, sizeof(size));
      write(data, size);
    }
  };
}

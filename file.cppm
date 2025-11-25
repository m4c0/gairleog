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

  export class read : public t {
  public:
    read() : t { "rb" } {}
  };

  export class write : public t {
    void store(const void * data, unsigned size) {
      if (fwrite(data, size, 1, m_f) != 1) throw error {};
    }

  public:
    write() : t { "wb" } {
      constexpr const unsigned id = 'GAIR';
      store(&id, sizeof(id));
    }

    void store(unsigned id, const void * data, unsigned size) {
      store(&id, sizeof(id));
      store(&size, sizeof(size));
      store(data, size);
    }
  };
}

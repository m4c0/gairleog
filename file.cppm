module;
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

export module file;
import buoy;
import hay;

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
  public:
    write() : t { "wb" } {}

    void store(const void * data, unsigned size) {
      if (fwrite(data, size, 1, m_f) != 1) throw error {};
    }
  };
}

module;
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

export module file;
import buoy;
import hay;

namespace file {
  static const auto filename = buoy::path("gairleog", "save.dat");

  static inline auto open() {
    return fopen(filename.begin(), "rb");
  }
  static inline auto close(FILE * f) {
    if (f) fclose(f);
  }

  export class read {
    hay<FILE *, open, close> m_f {};

  public:
    [[nodiscard]] explicit constexpr operator bool() const {
      return static_cast<FILE *>(m_f);
    }
  };
}

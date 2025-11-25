module;
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

export module file;
import buoy;
import hay;

namespace file {
  static const auto filename = buoy::path("gairleog", "save.dat");

  // Avoiding annoying Windows warning when instantiating hay
  constexpr const auto open = fopen;
  constexpr const auto close = fclose;

  export class read {
    hay<FILE *, open, close> m_f { filename.begin(), "rb" };

  public:
    [[nodiscard]] explicit constexpr operator bool() const {
      return static_cast<FILE *>(m_f);
    }
  };
}

module;
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

export module file;
import buoy;
import hai;
import hay;
import jute;

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

  template<typename T>
  concept pointer = requires (T t) { *t; };
  template<typename T>
  concept not_a_pointer = !pointer<T>;

  export class reader : public t {
  public:
    reader() : t { "rb" } {
      if (read<unsigned>() != id) throw error {};
      if (read<unsigned>() != version) throw error {};
    }

    template<typename T>
    void read(T * t) {
      if (fread(t, sizeof(T), 1, m_f) != 1) throw error {};
    }

    template<typename T> T read();
    template<not_a_pointer T>
    T read() {
      T val {};
      read(&val);
      return val;
    }
    template<>
    jute::heap read<jute::heap>() {
      unsigned len = read<unsigned>();
      if (len == 0) return {};
      hai::cstr str { len };
      return jute::heap { str };
    }
  };

  export class writer : public t {
  public:
    writer() : t { "wb" } {
      write(id);
      write(version);
    }

    void write(const void * data, unsigned size) {
      if (size == 0) return;
      if (fwrite(data, size, 1, m_f) != 1) throw error {};
    }

    template<not_a_pointer T>
    void write(T val) {
      write(&val, sizeof(T));
    }

    void write(jute::view str) {
      write<unsigned>(str.size());
      write(str.data(), str.size());
    }
  };
}

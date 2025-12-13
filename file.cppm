#ifdef LECO_TARGET_WASM
#pragma leco add_impl file_wasm
#else
#pragma leco add_impl file_impl
#endif

export module file;
import hai;
import jute;

namespace file {
  export struct error {};

  class t;

  template<typename T>
  concept pointer = requires (T t) { *t; };
  template<typename T>
  concept not_a_pointer = !pointer<T>;

  template<typename T>
  concept serializable = not_a_pointer<T> && __is_standard_layout(T);

  export class reader {
    hai::uptr<t> m_f;

  public:
    reader();
    ~reader();

    [[nodiscard]] explicit operator bool() const;

    void read(void * data, unsigned size);

    template<serializable T>
    void read(T * t) {
      read(t, sizeof(T));
    }

    template<typename T> T read() {
      static_assert(false, "Type is not serialisable");
    }
    template<serializable T>
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
      read(str.data(), len);
      return jute::heap { str };
    }
  };

  export class writer {
    hai::uptr<t> m_f;

  public:
    writer();
    ~writer();

    void write(const void * data, unsigned size);

    template<serializable T>
    void write(T val) {
      write(&val, sizeof(T));
    }

    void write(jute::view str) {
      write<unsigned>(str.size());
      write(str.data(), str.size());
    }
  };
}

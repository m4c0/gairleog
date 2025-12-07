#pragma leco tool
export module fmt;
import jute;
import sv;

struct lit {
  const char * str;
  unsigned len;

  template<unsigned N>
  consteval lit(const char (&str)[N]) : str { str }, len { N - 1 } {}
};

template<typename T> consteval lit needle();
template<> consteval lit needle<double   >() { return "f"; }
template<> consteval lit needle<long long>() { return "d"; }
template<> consteval lit needle<int      >() { return "d"; }
template<> consteval lit needle<sv       >() { return "s"; }

static consteval unsigned p_idx(lit haystack, lit needle) {
  for (auto i = 0; i < haystack.len - needle.len; i++) {
    if (haystack.str[i] != '%') continue;

    i++;
    if (haystack.str[i] == '%') continue; // Skip %%

    for (auto j = 0; j < needle.len; j++) {
      if (haystack.str[i + j] == needle.str[j]) continue;
      throw "invalid mask in format string";
    }
    return i - 1;
  }
  // TODO: merge mask
  throw "missing mask in format string";
}
static_assert(p_idx("ok%lldok", "lld") == 2);

template<typename T>
struct mask {
  const char * str;
  unsigned idx;
  unsigned len;

  template<unsigned N>
  consteval mask(const char (&str)[N]) :
    str { str }
  , idx { p_idx(str, needle<T>()) }
  , len { N - 1 }
  {}
};
template<typename T> static constexpr jute::heap fmt_impl(mask<T> mask, T n) {
  auto [ str, idx, len ] = mask;
  sv pre { str, idx };
  auto val = jute::to_s(n);
  sv post { str + idx + 2, len - idx - 2 };
  return (pre + val + post).heap();
}

export constexpr jute::heap fmt(mask<double   > mask, double    n) { return fmt_impl(mask, n); }
export constexpr jute::heap fmt(mask<long long> mask, int       n) { return fmt_impl(mask, static_cast<long long>(n)); }
export constexpr jute::heap fmt(mask<long long> mask, long long n) { return fmt_impl(mask, n); }
export constexpr jute::heap fmt(mask<sv>        mask, sv        n) { return fmt_impl(mask, n); }

static_assert(fmt("%d", 123) == "123");
static_assert(fmt("val = %d...", 123) == "val = 123...");
static_assert(fmt("val = %f...", 2.3) == "val = 2.300...");
static_assert(fmt("val = %s...", "ok") == "val = ok...");
// static_assert(fmt("val = %e...", 123) == ""); // compile-time error
// static_assert(fmt("val", 123) == ""); // compile-time error

int main() {}

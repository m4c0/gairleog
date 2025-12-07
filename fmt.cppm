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

static constexpr jute::heap to_s(long long val) {
  if (val == 0) return "0";

  auto negative = val < 0;
  unsigned long long n = negative ? -val : val;

  static constexpr const auto sz = 32;
  char buf[sz] {};
  auto p = buf + sz - 1;
  while (n) {
    *p-- = (n % 10) + '0';
    n /= 10;
  }
  if (negative) *p-- = '-';
  auto l = static_cast<unsigned>(buf + sz - 1 - p);
  return jute::heap { jute::view { p + 1, l } };
}
static_assert(to_s(0) == "0");
static_assert(to_s(1) == "1");
static_assert(to_s(123) == "123");
static_assert(to_s(-98) == "-98");

static constexpr jute::heap to_s(double val) {
  auto is = to_s(static_cast<long long>(val));

  auto tmp = to_s(static_cast<long long>(val * 1000.0 + 1000.0));
  auto fs = (*tmp).subview(tmp.size() - 3).after;

  return (is + "." + fs).heap();
}
static_assert(to_s(0.0) == "0.000");
static_assert(to_s(1.0) == "1.000");
static_assert(to_s(123.0) == "123.000");
static_assert(to_s(-98.0) == "-98.000");
static_assert(to_s(12.3) == "12.300"); // 12.300001
static_assert(to_s(2.3) == "2.300"); // 2.299999
static_assert(to_s(0.001) == "0.001");

static constexpr jute::view to_s(sv val) { return val; }

template<typename T> consteval lit needle();
template<> consteval lit needle<double   >() { return "f"; }
template<> consteval lit needle<long long>() { return "d"; }
template<> consteval lit needle<sv       >() { return "s"; }

static consteval unsigned p_idx(lit haystack, lit needle) {
  for (auto i = 0; i < haystack.len - needle.len; i++) {
    if (haystack.str[i] != '%') continue;

    i++;
    if (haystack.str[i] == '%') continue; // Skip %%

    bool ok = true;
    for (auto j = 0; j < needle.len; j++) {
      if (haystack.str[i + j] == needle.str[j]) continue;
      ok = false;
      break;
    }
    if (ok) return i - 1;
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
  auto val = to_s(n);
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
// static_assert(fmt("val = %e...", 123) == "");
// static_assert(fmt("val", 123) == "");

int main() {}

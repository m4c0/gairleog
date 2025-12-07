#pragma leco tool
export module fmt;
import jute;
import sv;
// TODO: implement a constexpr "fmt(mask, vals...)"

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

static constexpr jute::view to_s(sv val) { return val; }

template<unsigned N>
static consteval unsigned ll_idx(const char (&str)[N]) {
  for (auto i = 0; i < N - 2; i++) {
    if (str[i] != '%') continue;

    i++;
    if (str[i] == '%') continue; // Skip %%

    if (str[i] == 'd') return i - 1;
  }
  throw "missing %d in format string";
}
static_assert(ll_idx("ok%dok") == 2);

template<unsigned N>
static consteval unsigned sv_idx(const char (&str)[N]) {
  for (auto i = 0; i < N - 2; i++) {
    if (str[i] != '%') continue;

    i++;
    if (str[i] == '%') continue; // Skip %%

    if (str[i] == 's') return i - 1;
  }
  throw "missing %s in format string";
}

struct ll_mask {
  const char * str;
  unsigned idx;
  unsigned len;

  template<unsigned N>
  consteval ll_mask(const char (&str)[N]) :
    str { str }
  , idx { ll_idx(str) }
  , len { N }
  {}
};
struct sv_mask {
  const char * str;
  unsigned idx;
  unsigned len;

  template<unsigned N>
  consteval sv_mask(const char (&str)[N]) :
    str { str }
  , idx { sv_idx(str) }
  , len { N }
  {}
};

export constexpr jute::heap fmt(ll_mask mask, long long n) {
  auto [ str, idx, len ] = mask;
  sv pre { str, idx };
  auto val = to_s(n);
  sv post { str + idx + 2, len - idx - 3 };
  return (pre + val + post).heap();
}
export constexpr jute::heap fmt(sv_mask mask, sv n) {
  auto [ str, idx, len ] = mask;
  sv pre { str, idx };
  auto val = to_s(n);
  sv post { str + idx + 2, len - idx - 3 };
  return (pre + val + post).heap();
}

static_assert(fmt("%d", 123) == "123");
static_assert(fmt("val = %d...", 123) == "val = 123...");
static_assert(fmt("val = %s...", "ok") == "val = ok...");
// static_assert(fmt("val = %e...", 123) == "");
// static_assert(fmt("val", 123) == "");

int main() {}

#pragma leco tool
export module fmt;
import jute;
import sv;
import traits;

struct lit {
  const char * str;
  unsigned len;

  template<unsigned N>
  consteval lit(const char (&str)[N]) : str { str }, len { N - 1 } {}
};
template<traits::size_t N>
struct lit2 {
  char str[N];
  unsigned len = N - 1;

  consteval lit2(const char (&s)[N]) {
    for (auto i = 0; i < N; i++) str[i] = s[i];
  }
};

template<typename T> consteval lit needle();
template<> consteval lit needle<double   >() { return "f"; }
template<> consteval lit needle<long long>() { return "d"; }
template<> consteval lit needle<int      >() { return "d"; }
template<> consteval lit needle<sv       >() { return "s"; }

static consteval unsigned p_idx(const char * hs, unsigned hl, lit needle) {
  for (auto i = 0; i < hl - needle.len; i++) {
    if (hs[i] != '%') continue;

    i++;
    if (hs[i] == '%') continue; // Skip %%

    for (auto j = 0; j < needle.len; j++) {
      if (hs[i + j] == needle.str[j]) continue;
      throw "invalid mask in format string";
    }
    return i - 1;
  }
  // TODO: merge mask
  throw "missing mask in format string";
}
static_assert(p_idx("ok%lldok", 8, "lld") == 2);

template<lit2 S, typename T>
struct mask {
  static constexpr jute::heap fmt(T && n) {
    constexpr unsigned idx = p_idx(S.str, S.len, needle<T>());
    auto [ str, len ] = S;
    sv pre { str, idx };
    auto val = jute::to_s(traits::move(n));
    sv post { str + idx + 2, len - idx - 2 };
    return (pre + val + post).heap();
  }
};

export
template<lit2 Msk, typename T>
constexpr jute::heap fmt(T && n) {
  return mask<Msk, T>::fmt(traits::fwd<T>(n));
}

static_assert(fmt<"%d">(123) == "123");
static_assert(fmt<"val = %d...">(123) == "val = 123...");
static_assert(fmt<"val = %f...">(2.3) == "val = 2.300...");
static_assert(fmt<"val = %s...">("ok"_sv) == "val = ok...");
// static_assert(fmt<"val = %e...">(123) == ""); // compile-time error
// static_assert(fmt<"val">(123) == ""); // compile-time error

// static_assert(fmt2("we got %d from %s at %f rate...", 23, "there", 6.7) == "");
// static_assert(fmt2("we got %d from %s at %f rate...", 23, "there", 6.7) == "we got 23 from there at 6.7 rate...");

int main() {}

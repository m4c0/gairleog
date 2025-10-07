export module tiledefs;
import jute;

namespace tiledefs {
  struct flags {
    bool enemy  : 1 = false;
    bool food   : 1 = false;
    bool player : 1 = false;
    bool pot    : 1 = false;
    bool solid  : 1 = false;
    bool toad   : 1 = false;
  };
  union u32flags {
    unsigned u32 = 0;
    flags f;
  };

  export struct t {
    float light {};
    unsigned sprite {};
    flags flags {};
  };

  static inline unsigned bit_of(flags f) {
    u32flags u {};
    u.f = f;
    return u.u32;
  }
  export unsigned bit_of(jute::view atom) {
    if (atom == "enemy")  return bit_of({ .enemy  = true });
    if (atom == "food")   return bit_of({ .food   = true });
    if (atom == "player") return bit_of({ .player = true });
    if (atom == "pot")    return bit_of({ .pot    = true });
    if (atom == "solid")  return bit_of({ .solid  = true });
    if (atom == "toad")   return bit_of({ .toad   = true });
    return 0U; // Should only happen whilst parsing
  }
}

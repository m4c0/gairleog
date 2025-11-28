#pragma leco add_impl entdefs_impl
export module entdefs;
import jute;
import sprdef;

namespace entdefs {
  export struct flags {
    bool ceramic : 1 = false;
    bool enemy   : 1 = false;
    bool exit    : 1 = false;
    bool food    : 1 = false;
    bool player  : 1 = false;
    bool solid   : 1 = false;
    bool toad    : 1 = false;
    bool wall    : 1 = false;
  };
  static_assert(sizeof(flags) == 1);

  union u32flags {
    unsigned u32 = 0;
    flags f;
  };

  export struct t {
    jute::heap loot {};

    float        light         {};
    unsigned     life          {};
    unsigned     max_life      {};
    unsigned     sprite        {};
    sprdef::pair attack_sprite {};
    unsigned     strength      {};
    unsigned     defense       {};
    flags        flags         {};
  };

  export inline unsigned bit_of(flags f) {
    u32flags u {};
    u.f = f;
    return u.u32;
  }
  export unsigned bit_of(jute::view atom) {
    if (atom == "ceramic") return bit_of({ .ceramic = true });
    if (atom == "enemy")   return bit_of({ .enemy   = true });
    if (atom == "exit")    return bit_of({ .exit    = true });
    if (atom == "food")    return bit_of({ .food    = true });
    if (atom == "player")  return bit_of({ .player  = true });
    if (atom == "solid")   return bit_of({ .solid   = true });
    if (atom == "toad")    return bit_of({ .toad    = true });
    return 0U; // Should only happen whilst parsing
  }

  export bool has(jute::view name);
  export const t & get(jute::view name);

  export void run(jute::view src);
}

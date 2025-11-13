export module tiledefs;
import jute;
import lispy;
import sprdef;

namespace tiledefs {
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
    unsigned     sprite        {};
    sprdef::pair attack_sprite {};
    unsigned     strength      {};
    unsigned     defense       {};

    flags    flags  {};
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

  template<typename Node, flags Flags>
  static constexpr const auto mem_set = lispy::experimental::mem_set<&Node::attr, [](auto * self, auto * n) {
    u32flags u {};
    u.u32 = bit_of(Flags) | bit_of(self->flags);
    self->flags = u.f;
  }>;

  export template<typename Node>
  void lispy(auto & ctx) {
    ctx.fns["ceramic"] = mem_set<Node, { .ceramic = true }>;
    ctx.fns["enemy"]   = mem_set<Node, { .enemy   = true }>;
    ctx.fns["exit"]    = mem_set<Node, { .exit    = true }>;
    ctx.fns["food"]    = mem_set<Node, { .food    = true }>;
    ctx.fns["player"]  = mem_set<Node, { .player  = true }>;
    ctx.fns["solid"]   = mem_set<Node, { .solid   = true }>;
    ctx.fns["toad"]    = mem_set<Node, { .toad    = true }>;
  }
}

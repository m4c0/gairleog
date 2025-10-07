export module tiledefs;
import jute;
import lispy;

namespace tiledefs {
  export struct flags {
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

  export inline unsigned bit_of(flags f) {
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

  template<typename Node, flags Flags>
  static constexpr const auto mem_set = lispy::experimental::mem_set<&Node::attr, [](auto * self, auto * n) {
    u32flags u {};
    u.f = self->flags;
    u.u32 |= bit_of(Flags);
    self->flags = u.f;
  }>;

  export template<typename Node>
  void lispy(auto & ctx) {
    ctx.fns["enemy"]  = mem_set<Node, { .enemy  = true }>;
    ctx.fns["food"]   = mem_set<Node, { .food   = true }>;
    ctx.fns["player"] = mem_set<Node, { .player = true }>;
    ctx.fns["pot"]    = mem_set<Node, { .pot    = true }>;
    ctx.fns["solid"]  = mem_set<Node, { .solid  = true }>;
    ctx.fns["toad"]   = mem_set<Node, { .toad   = true }>;
  }
}

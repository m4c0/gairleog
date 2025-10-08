export module ents;
import dotz;
import hai;
import tiledefs;

namespace ents {
  struct t : tiledefs::t {
    dotz::vec2 pos;
  };
  hai::varray<t> ents { 128 };

  export void foreach(auto && fn) {
    for (auto & e : ents) fn(e);
  }

  export void reset() {
    ents.truncate(0);
  }

  export void lift(dotz::vec2 pos, tiledefs::t & tdef) {
    const auto entities = tiledefs::bit_of({
      .ceramic = true,
      .enemy   = true,
      .food    = true,
      .toad    = true,
    });
    const auto tbits = tiledefs::bit_of(tdef.flags);
    if ((entities & tbits) == 0) return;

    t ent = { tdef };
    ent.pos = pos;
    ents.push_back_doubling(ent);

    tdef = {};
  }
}

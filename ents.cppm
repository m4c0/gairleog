export module ents;
import dotz;
import tiledefs;

namespace ents {
  export void lift(dotz::vec2 pos, tiledefs::t & tdef) {
    const auto entities = tiledefs::bit_of({
      .ceramic = true,
      .enemy   = true,
      .food    = true,
      .toad    = true,
    });
    const auto tbits = tiledefs::bit_of(tdef.flags);
    if ((entities & tbits) == 0) return;

    // TODO: lift
  }
}

export module ents;
import dotz;
import hai;
import hitdefs;
import tiledefs;

namespace ents {
  export struct t : tiledefs::t {
    dotz::ivec2 pos {};
    unsigned life {};
  };
  hai::varray<t> ents { 1024 };

  export void foreach(auto && fn) {
    for (auto & e : ents) if (e.life) fn(e);
  }

  export void reset() {
    ents.truncate(0);
  }

  export void add(dotz::ivec2 pos, tiledefs::t tdef) {
    t ent = { tdef };
    ent.pos = pos;
    ent.life = tdef.life ? tdef.life : 1;
    ents.push_back_doubling(ent);
  }

  export enum class move_outcome {
    none,
    exit,
  };
  export move_outcome move(t * ent, dotz::ivec2 by) {
    move_outcome res {};
    auto p_pos = ent->pos + by;
    ents::foreach([&](auto & d) {
      if (d.pos != p_pos) return;
      for (auto act : hitdefs::check(ent->flags, d.flags)) {
        switch (act) {
          using enum hitdefs::action;
          case block:
            p_pos = ent->pos;
            break;
          case exit:
            res = move_outcome::exit;
            break;
          case hit:
            p_pos = ent->pos;
            d.life--;
            if (!d.life) d = {};
            break;
          case miss:
            p_pos = ent->pos;
            break;
          case pick:
            d = {}; 
            break;
          case poison:
            p_pos = ent->pos;
            break;
        }
      }
    });
    ent->pos = p_pos;
    return res;
  }
}

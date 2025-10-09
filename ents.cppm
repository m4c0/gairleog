export module ents;
import dotz;
import hai;
import hitdefs;
import tiledefs;

namespace ents {
  export struct t : tiledefs::t {
    dotz::ivec2 pos {};
    bool alive = false;
  };
  hai::varray<t> ents { 1024 };

  export void foreach(auto && fn) {
    for (auto & e : ents) if (e.alive) fn(e);
  }

  export void reset() {
    ents.truncate(0);
  }

  export void add(dotz::ivec2 pos, tiledefs::t tdef) {
    t ent = { tdef };
    ent.pos = pos;
    ent.alive = true;
    ents.push_back_doubling(ent);
  }

  export void take_hit(t * ent, hitdefs::action act) {
    switch (act) {
      using enum hitdefs::action;
      case block:
      case exit:
      case miss:
      case poison:
      case pick:
        break;
      case hit:
        *ent = {}; 
        break;
    }
  }
}

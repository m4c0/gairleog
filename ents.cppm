export module ents;
import dotz;
import entdefs;
import fx;
import hai;
import hitdefs;
import inv;

namespace ents {
  export using flags = entdefs::flags;

  export struct t : entdefs::t {
    dotz::ivec2 pos {};
    dotz::vec2 size { 1 };
    unsigned life {};
    unsigned poison {};
  };
  hai::varray<t> ents { 1024 };

  export void foreach(auto && fn) {
    for (auto & e : ents) if (e.life) fn(e);
  }
  export void foreach(flags flags, auto && fn) {
    auto flg = entdefs::bit_of(flags);
    for (auto & e : ents) {
      if (!e.life) continue;
      if ((entdefs::bit_of(e.flags) & flg) != flg) continue;
      fn(e);
    }
  }

  export void reset() {
    ents.truncate(0);
  }

  static t create(dotz::ivec2 pos, entdefs::t tdef) {
    t ent = { tdef };
    ent.pos = pos;
    ent.life = tdef.life ? tdef.life : 1;
    return ent;
  }
  export void add(dotz::ivec2 pos, entdefs::t tdef) {
    ents.push_back_doubling(create(pos, tdef));
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
            fx::add(d.pos, ent->attack_sprite);
            p_pos = ent->pos;
            d.life--;
            if (!d.life) {
              if (d.loot == "") {
                d = {};
              } else {
                d = create(d.pos, entdefs::get(d.loot));
              }
            }
            break;
          case miss:
            // TODO: attack anim
            p_pos = ent->pos;
            break;
          case pick:
            inv::add(d);
            d = {}; 
            break;
          case poison:
            p_pos = ent->pos;
            d.poison++;
            break;
        }
      }
    });
    if (by.x != 0) ent->size.x = by.x;
    ent->pos = p_pos;
    return res;
  }
}

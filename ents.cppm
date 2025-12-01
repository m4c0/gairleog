export module ents;
import dotz;
import entdefs;
import file;
import fx;
import hai;
import hitdefs;
import inv;
import jute;
import sfxdefs;
import silog;

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

  export bool take_hit(t * ent) {
    if (ent->life) ent->life--;
    if (ent->life) return true;
    if (ent->loot == "") {
      *ent = {};
    } else {
      *ent = create(ent->pos, entdefs::get(ent->loot));
    }
    return false;
  }

  export enum class move_outcome {
    none,
    exit,
  };
  export move_outcome move(t * ent, dotz::ivec2 by) {
    if (ent->poison) {
      ent->poison--;
      if (!take_hit(ent)) return move_outcome::none;
    }

    move_outcome res {};
    auto p_pos = ent->pos + by;
    ents::foreach([&](auto & d) {
      if (d.pos != p_pos) return;
      for (auto act : hitdefs::check(ent->flags, d.flags)) {
        switch (act) {
          using enum hitdefs::action;
          case block:
            sfxdefs::play(ent->sfx.block);
            p_pos = ent->pos;
            break;
          case exit:
            res = move_outcome::exit;
            break;
          case hit:
            fx::add(d.pos, ent->attack_sprite);
            sfxdefs::play(ent->sfx.attack);
            p_pos = ent->pos;
            take_hit(&d);
            break;
          case miss:
            // TODO: attack anim
            p_pos = ent->pos;
            sfxdefs::play(ent->sfx.miss);
            break;
          case pick:
            inv::add(d);
            sfxdefs::play(ent->sfx.pick);
            d = {}; 
            break;
          case poison:
            sfxdefs::play(ent->sfx.poison);
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

  export void read(file::reader * r) {
    ents.truncate(0);

    auto len = r->read<unsigned>();
    silog::infof("Reading %d entities", len);
    for (auto i = 0; i < len; i++) {
      auto loot = r->read<jute::heap>();
      entdefs::sfx sfx {
        .attack = r->read<jute::heap>(),
        .block  = r->read<jute::heap>(),
        .miss   = r->read<jute::heap>(),
        .pick   = r->read<jute::heap>(),
        .walk   = r->read<jute::heap>(),
      };
      auto val = r->read<t>();
      val.loot = loot;
      val.sfx = sfx;
      ents.push_back_doubling(val);
    }
  }
  export void write(file::writer * w) {
    silog::infof("Storing %d entities", ents.size());
    w->write(ents.size());
    for (auto d : ents) {
      w->write(d.loot);
      w->write(d.sfx.attack);
      w->write(d.sfx.block);
      w->write(d.sfx.miss);
      w->write(d.sfx.pick);
      w->write(d.sfx.walk);

      d.loot = {};
      d.sfx = {};
      w->write(d);
    }
  }
}

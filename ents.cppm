export module ents;
import dotz;
import entdefs;
import file;
import fx;
import hai;
import hitdefs;
import inv;
import jute;
import rng;
import sfxdefs;
import silog;
import splats;
import strings;

namespace ents {
  export using flags = entdefs::flags;

  struct data {
    dotz::ivec2 pos {};
    dotz::vec2 size { 1 };
    unsigned life {};
    unsigned poison {};
  };
  export struct t : entdefs::t, data {};
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
    ent.life = tdef.max_life ? tdef.max_life : 1;
    return ent;
  }
  export void add(dotz::ivec2 pos, entdefs::t tdef) {
    ents.push_back_doubling(create(pos, tdef));
  }

  export bool take_hit(t * ent, unsigned dmg) {
    if (ent->splat_sprite.id) {
      splats::add({
        .pos = ent->pos,
        .sprite = ent->splat_sprite.id,
      });
    }
    if (ent->life) ent->life -= dotz::min(ent->life, dmg);;
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
  static bool process_poison(t * ent) {
    if (!ent->poison) return true;

    auto [decay, dok] = jute::to_u32(strings::get("poison-decay"));
    if (!dok) silog::die("poison-decay must yield integers");
    ent->poison -= dotz::min(ent->poison, decay);

    auto [hit, hok] = jute::to_u32(strings::get("poison-hit"));
    if (!hok) silog::die("poison-hit must yield integers");
    return take_hit(ent, hit);
  }
  export move_outcome move(t * ent, dotz::ivec2 by) {
    if (!process_poison(ent)) return move_outcome::none;

    move_outcome res {};
    auto p_pos = ent->pos + by;
    ents::foreach([&](auto & d) {
      if (d.pos != p_pos) return;
      for (auto act : hitdefs::check(ent->flags, d.flags)) {
        switch (act) {
          using enum hitdefs::action;
          case block:
            sfxdefs::play(ent->name, "block");
            p_pos = ent->pos;
            break;
          case exit:
            res = move_outcome::exit;
            break;
          case hit: {
            fx::add(d.pos, ent->attack_sprite);
            p_pos = ent->pos;

            int atk = rng::rand(ent->strength * 6);
            int def = rng::rand(d.defense * 6);
            if (atk > def) {
              sfxdefs::play(ent->name, "attack");
              take_hit(&d, rng::rand(atk - def));
            } else {
              sfxdefs::play(ent->name, "miss");
            }
            break;
          }
          case miss:
            // TODO: attack anim
            p_pos = ent->pos;
            sfxdefs::play(ent->name, "miss");
            break;
          case pick:
            inv::add(d);
            sfxdefs::play(ent->name, "pick");
            d = {}; 
            break;
          case poison:
            sfxdefs::play(ent->name, "poison");
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
      t val {};
      static_cast<data &>(val) = r->read<data>();
      static_cast<entdefs::data &>(val) = r->read<entdefs::data>();
      val.loot = r->read<jute::heap>();
      val.name = r->read<jute::heap>();
      ents.push_back_doubling(val);
    }
  }
  export void write(file::writer * w) {
    silog::infof("Storing %d entities", ents.size());
    w->write(ents.size());
    for (auto d : ents) {
      w->write(static_cast<data>(d));
      w->write(static_cast<entdefs::data>(d));
      w->write(d.name);
      w->write(d.loot);
    }
  }
}

export module pathing;
import dotz;
import ents;
import map;

namespace pathing {
  struct node {
    dotz::ivec2 step {};
    unsigned distance {};
    bool testing {};
    bool solid {};
  };
  export class t {
    node m_data[map::h][map::w] {};

    void check(dotz::ivec2 p, dotz::ivec2 step, unsigned dist) {
      if (dist > 5) return;

      auto pp = p - step;
      auto & n = m_data[pp.y][pp.x];
      if (n.solid) return;

      if (n.testing) {
        if (n.distance <= dist) return;

        n.step = step;
        n.distance = dist;
        return;
      }

      n.testing = true;
      n.step = step;
      n.distance = dist;
      check(pp, { -1, 0 }, dist + 1);
      check(pp, { +1, 0 }, dist + 1);
      check(pp, { 0, -1 }, dist + 1);
      check(pp, { 0, +1 }, dist + 1);
    }

  public:
    t() {
      dotz::ivec2 pl {};
      ents::foreach({ .solid = true }, [&](auto & p) {
        if (p.flags.enemy) return;
        else if (p.flags.player) pl = p.pos;
        else m_data[p.pos.y][p.pos.x].solid = true;
      });

      m_data[pl.y][pl.x].testing = true;
      check(pl, { -1, 0 }, 1);
      check(pl, { +1, 0 }, 1);
      check(pl, { 0, -1 }, 1);
      check(pl, { 0, +1 }, 1);
    }

    [[nodiscard]] dotz::ivec2 operator()(dotz::ivec2 start) const {
      return m_data[start.y][start.x].step;
    }
  };
}

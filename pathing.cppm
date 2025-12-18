export module pathing;
import dotz;
import ents;
import map;

namespace pathing {
  struct node {
    dotz::ivec2 delta {};
    bool solid {};
  };
  export class t {
    dotz::ivec2 m_player {};
    node m_data[map::h][map::w] {};

  public:
    t() {
      ents::foreach({ .solid = true }, [&](auto & p) {
        if (p.flags.player) m_player = p.pos;
        m_data[p.pos.y][p.pos.x].solid = true;
      });
    }

    // TODO: A*
    [[nodiscard]] dotz::ivec2 operator()(dotz::ivec2 start) const {
      return {};
    }
  };
}

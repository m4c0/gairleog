export module splats;
import dotz;
import hai;
import sprdef;

namespace splats {
  struct t {
    dotz::vec2 pos;
    unsigned sprite;
  };
  hai::varray<t> data { 128 };

  export void reset() {
    data.truncate(0);
  }

  export void add(dotz::ivec2 p) {
    data.push_back(t {
      .pos = p,
      .sprite = sprdef::get("environment/props/splatter_red").id,
    });
  }

  export void for_each(auto && fn) {
    for (const auto & d : data) fn(d);
  }
}

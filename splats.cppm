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

  export void add(t t) {
    data.push_back_doubling(t);
  }

  export void for_each(auto && fn) {
    for (const auto & d : data) fn(d);
  }
}

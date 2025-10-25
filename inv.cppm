export module inv; 
import hai;
import tiledefs;

namespace inv {
  export struct t : tiledefs::t {
  };

  hai::varray<t> data { 128 };

  export void reset() {
    data.truncate(0);
  }

  export void add(tiledefs::t tdef) {
    data.push_back_doubling(t { tdef });
  }

  static t invalid {};
  export const auto & at(int idx) {
    if (idx < 0 || idx >= data.size()) return invalid;
    return data[idx];
  }
}

export module inv; 
import hai;

import entdefs;

namespace inv {
  export struct t : entdefs::t {
  };

  hai::varray<t> data { 128 };

  export void reset() {
    data.truncate(0);
    data.push_back(t { entdefs::get("apple") });
    data.push_back(t { entdefs::get("potato") });
    data.push_back(t { entdefs::get("berry") });
    data.push_back(t { entdefs::get("chicken") });
    data.push_back(t { entdefs::get("turnip") });
    data.push_back(t { entdefs::get("potato") });
    data.push_back(t { entdefs::get("berry") });
    data.push_back(t { entdefs::get("chicken") });
    data.push_back(t { entdefs::get("turnip") });
    data.push_back(t { entdefs::get("apple") });
    data.push_back(t { entdefs::get("potato") });
    data.push_back(t { entdefs::get("apple") });
    data.push_back(t { entdefs::get("potato") });
    data.push_back(t { entdefs::get("apple") });
    data.push_back(t { entdefs::get("potato") });
    data.push_back(t { entdefs::get("apple") });
    data.push_back(t { entdefs::get("apple") });
  }

  export void add(entdefs::t tdef) {
    data.push_back_doubling(t { tdef });
  }

  static t invalid {};
  export const auto & at(int idx) {
    if (idx < 0 || idx >= data.size()) return invalid;
    return data[idx];
  }

  export auto size() { return data.size(); }

  export void consume(int sel) {
    if (sel < 0 || sel >= data.size()) return;
    for (auto i = sel; i < data.size() - 1; i++) data[i] = data[i + 1];
    data.pop_back();
  }
}

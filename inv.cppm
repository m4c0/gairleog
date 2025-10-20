export module inv; 
import hai;
import tiledefs;

namespace inv {
  export struct t : tiledefs::t {
  };

  hai::chain<t> data { 128 };

  export void add(tiledefs::t tdef) {
    data.push_back({ tdef });
  }
}

#pragma leco add_impl entdefs_impl
export module entdefs;
import jute;
import hashley;
import tiledefs;

namespace entdefs {
  hashley::fin<tiledefs::t> defs { 127 };

  export bool has(jute::view name) {
    return defs.has(name);
  }
  export const auto & get(jute::view name) {
    return defs[name];
  }

  export void run(jute::view src);
}

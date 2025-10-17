export module sprdef;
import hai;
import hashley;
import jute;
import lispy;
import sires;
import traits;

namespace sprdef {
  export struct error : lispy::parser_error {};

  auto & map() {
    static hashley::niamh i { 1023 };
    return i;
  }
  export bool has(jute::view key) { return map().has(key); }
  export const auto & get(jute::view key) { return map()[key]; }

  struct custom_node : lispy::node {
    int spr_id;
    bool valid;
  };
  static auto sprdef(const lispy::node * n, const custom_node * def, const custom_node * val) {
    if (!lispy::is_atom(def)) lispy::err(def, "def name must be an atom");
    map()[def->atom] = lispy::to_i(val);
    return n;
  }
  export void run(jute::view src) try {
    lispy::context ctx {
      .allocator = lispy::allocator<custom_node>(),
    };
    ctx.fns["sprdef"] = lispy::experimental::wrap<custom_node, sprdef>;
    lispy::run<custom_node>(src, &ctx);
  } catch (const lispy::parser_error & e) {
    throw error { e };
  }

  export unsigned to_spr(const lispy::node * name) {
    if (!lispy::is_atom(name)) lispy::err(name, "spr expects atom as name");
    if (!has(name->atom)) lispy::err(name, "invalid sprite name");
    return get(name->atom);
  }
}

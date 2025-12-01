#pragma leco add_resource_dir sfx
#pragma leco add_resource "sfxdefs.lsp"
export module sfxdefs;
import errs;
import hashley;
import hai;
import lispy;
import sires;
import sv;

namespace sfxdefs {
  hashley::niamh ids { 127 };

  static void run(sv src) try {
    using namespace lispy;
    using namespace lispy::experimental;

    temp_arena<node> a {};
    context ctx {};
    ctx.fns["sfxdef"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 2) err(n, "sfxdef requires name and an index");
      if (!is_atom(aa[0])) err(n, "expecting an atom as the name");
      auto id = to_i(aa[1]);
      ids[aa[0]->atom] = id;
      return n;
    };
    run<node>(src, &ctx);
  } catch (const lispy::parser_error & e) {
    throw lispy::to_file_err("sfxdefs.lsp", e);
  }

  export void load(hai::fn<void> && callback) {
    sires::read("sfxdefs.lsp", &errs::jid, [=](auto, hai::cstr & src) mutable {
      run(src);
      callback();
    });
  }
}

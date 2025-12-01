#pragma leco add_resource_dir sfx
#pragma leco add_resource "sfxdefs.lsp"
export module sfxdefs;
import errs;
import hashley;
import hai;
import lispy;
import sires;
import sv;
import wav;

namespace sfxdefs {
  struct t {
    int id {};
    hai::array<float> samples {};
  };
  hashley::fin<t> ids { 127 };

  static void load_wav(void * ptr, hai::cstr & buf) try {
    static_cast<t *>(ptr)->samples = wav::load(buf);
  } catch (const wav::error & e) {
    silog::error(e.msg);
  }

  static auto g_arena = lispy::arena<lispy::node>::make();
  static lispy::context g_ctx {};
  static void run(sv src) try {
    using namespace lispy;
    using namespace lispy::experimental;

    g_ctx.fns["sfxdef"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 2) erred(n, "sfxdef requires name and an index");
      if (!is_atom(aa[0])) erred(n, "expecting an atom as the name");
      auto name = aa[0]->atom;
      auto id = to_i(aa[1]);
      auto & tt = ids[name];
      tt.id = id;
      sires::read(aa[0]->atom, &tt, load_wav);
      return n;
    };

    auto a = g_arena->use();
    run<node>(src, &g_ctx);
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

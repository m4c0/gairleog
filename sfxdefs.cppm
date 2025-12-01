#pragma leco add_resource_dir sfx
#pragma leco add_resource "sfxdefs.lsp"
export module sfxdefs;
import audio;
import errs;
import hashley;
import hai;
import lispy;
import silog;
import sires;
import sv;
import wav;

namespace sfxdefs {
  struct t {
    hai::array<float> samples {};
  };
  hashley::fin<t> cache { 127 };

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
      if (as != 1) erred(n, "sfxdef requires a single atom with the file name");
      if (!is_atom(aa[0])) erred(n, "expecting an atom as the file name");
      auto name = aa[0]->atom;
      auto & tt = cache[name];
      sires::read(aa[0]->atom, &tt, load_wav);
      n->ctx->defs[aa[0]->atom] = aa[0];
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

  static const t dummy {};
  export const t & get(sv name) {
    auto n = g_ctx.defs[name];
    if (!n) return dummy;

    using namespace lispy;

    temp_arena<node> a {};
    context ctx { .parent = &g_ctx };
    auto nn = eval<node>(&ctx, n);
    if (!is_atom(nn)) return dummy;
    return cache[nn->atom];
  }

  export void play(sv name) {
    audio::play(get(name).samples);
  }
}

#pragma leco add_resource_dir sfx
#pragma leco add_resource "sfxdefs.lsp"
export module sfxdefs;
import audio;
import errs;
import glispy;
import hashley;
import hai;
import jute;
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

  auto g_arena = lispy::arena<lispy::node>::make();
  auto g_ctx = lispy::frame::make();
  static void run(sv src) {
    using namespace lispy;

    g_ctx->fns["sfxdef"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 1) erred(n, "sfxdef requires a single atom with the file name");
      if (!is_atom(aa[0])) erred(n, "expecting an atom as the file name");
      auto name = aa[0]->atom;
      auto & tt = cache[name];
      sires::read(aa[0]->atom, &tt, load_wav);
      lispy::context()->def(aa[0]->atom, aa[0]);
      return n;
    };

    auto a = g_arena->use();
    frame_guard c { g_ctx };
    lispy::run<node>("sfxdefs.lsp", src);
  }

  export void load(hai::fn<void> && callback) {
    sires::read("sfxdefs.lsp", &errs::jid, [=](auto, hai::cstr & src) mutable {
      run(src);
      callback();
    });
  }

  hashley::niamh g_warns { 127 };
  export void play(sv name) {
    auto n = g_ctx->defs[name];
    if (!n) {
      auto & warned = g_warns[name];
      if (warned) return;

      warned = 1;
      silog::warning(jute::fmt<"sound file does not exist: %s">(name));
      return;
    }

    using namespace lispy;
    
    temp_arena<node> a {};
    frame_guard c { g_ctx };
    temp_frame ctx {};
    auto nn = eval<node>(n);
    if (!is_atom(nn)) return;
    audio::play(cache[nn->atom].samples);
  }

  export void play(sv ent, sv name) {
    play(*jute::fmt<"%s-%s">(ent, name));
  }
}

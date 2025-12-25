module lootfx;
import glispy;
import hai;
import hashley;
import silog;

using namespace lispy;
using namespace lispy::experimental;

namespace lootfx {
  hai::varray<jute::view> keys { 128 };

  auto src_arena = arena<node>::make();
  auto src_ctx = [] {
    auto ctx = frame::make();
    ctx->fns["fxdef"] = [](auto n, auto aa, auto as) -> const node * {
      if (as != 2) erred(n, "expecting a name and an action");
      context()->def(to_atom(aa[0]), aa[1]);
      keys.push_back_doubling(aa[0]->atom);
      return n;
    };
    return ctx;
  }();

  void reset() {
    for (auto & m : map) m = {};

    rest.truncate(0);
    for (auto k : keys) rest.push_back_doubling(jute::heap { k });
  }

  void run(jute::view src) {
    auto a = src_arena->use();
    frame_guard ctx { src_ctx };
    lispy::run<node>("lootfx.lsp", src);
  }

  template<action A>
  static const node * act(const node * n, const node * const * aa, unsigned as) {
    if (as != 0) erred(n, "expecting no parameter");
    static_cast<action_list_t *>(context()->ptr("list"))->push_back(A);
    return n;
  }
  auto act_ctx = [] {
    auto ctx = frame::make();
    ctx->fns["damage"]   = act<action::damage>;
    ctx->fns["defence"]  = act<action::defence>;
    ctx->fns["heal"]     = act<action::heal>;
    ctx->fns["maxlife"]  = act<action::max_life>;
    ctx->fns["poison"]   = act<action::poison>;
    ctx->fns["strength"] = act<action::strength>;
    ctx->fns["weakness"] = act<action::weakness>;
    ctx->fns["wither"]   = act<action::wither>;
    return ctx;
  }();
  void apply(jute::view key, action_list_t * r) {
    auto n = src_ctx->defs[key];
    if (!n) silog::die("missing lootfx [%s]", key.cstr().begin());

    frame_guard sc { src_ctx };
    frame_guard ac { act_ctx };

    temp_frame ctx {};
    ctx.ptrs["list"] = r;
    auto _ = eval<node>(n);
  }

  void read(file::reader * r) {
    for (auto & v : map) v = r->read<jute::heap>();

    rest.truncate(0);
    auto len = r->read<unsigned>();
    for (auto i = 0; i < len; i++) rest.push_back_doubling(r->read<jute::heap>());
  }
  void write(file::writer * w) {
    for (auto v : map) w->write(v);

    w->write<unsigned>(rest.size());
    for (auto v : rest) w->write(v);
  }
}

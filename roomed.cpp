#pragma leco app

import casein;
import dotz;
import hai;
import jute;
import lispy;
import res;
import silog;
import sires;
import sprdef;
import v;

static int g_ed = 0;
static hai::array<hai::cstr> g_table {};
static int g_grid_size = 0;

static void on_frame() {
  auto m = v::map();

  auto font = sprdef::get("font").id;
  for (auto y = 0; y < g_table.size(); y++) {
    auto & row = g_table[y];
    for (auto x = 0; x < row.size(); x++) {
      m->push({
        .pos { x, y },
        .id = font + row.data()[x],
      });
    }
  }

  v::set_grid({ g_grid_size, g_grid_size });
}

using namespace lispy;
using namespace lispy::experimental;

static const node * load_room(const node * n, const node * const * aa, unsigned as) {
  if (as < 2) err(n, "rooms must have at least two rows");

  auto len0 = -1;
  g_table.set_capacity(as);
  for (auto i = 0; i < as; i++) {
    if (!is_atom(aa[i])) err(aa[i], "rooms must only have atoms as rows");

    auto len = aa[i]->atom.size();
    if (len < 2) err(aa[i], "rooms must have at least two rows");

    if (len0 == -1) len0 = len;
    else if (len != len0) err(aa[i], "all rows must have the same length");

    g_table[i] = (aa[i]->atom).cstr();
  }
  g_grid_size = dotz::max(as, len0) / 2;
  return n;
}

static const node * load_roomdefs(const node * n, const node * const * aa, unsigned as) {
  context ctx { n->ctx->allocator }; 
  ctx.fns["room"] = load_room;

  if (g_ed < 0) g_ed = as - 1;
  if (g_ed >= as) g_ed = 0;

  return eval<node>(&ctx, aa[g_ed]);
}

static void load(void *, const hai::cstr & src) try {
  basic_context<node> ctx {}; 
  ctx.fns["roomdefs"] = load_roomdefs;
  run<node>(src, &ctx);

  v::on_frame = on_frame;
} catch (const lispy::parser_error & e) {
  silog::error(lispy::to_file_err("roomdefs.lsp", e));
}

static void on_init() {
  sires::read("roomdefs.lsp", nullptr, load);
}

const int i = [] {
  using namespace casein;
  handle(KEY_DOWN, K_DOWN, [] { g_ed++; on_init(); });
  handle(KEY_DOWN, K_UP,   [] { g_ed--; on_init(); });

  res::load_all(on_init);
  return 0;
}();

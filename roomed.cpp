#pragma leco app
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

import casein;
import dotz;
import entdefs;
import hai;
import hay;
import jute;
import lispy;
import print;
import res;
import silog;
import sires;
import sprdef;
import themedefs;
import v;

using namespace lispy;
using namespace lispy::experimental;

static hai::array<hai::cstr> g_table {};
static float g_grid_size = 0;

static auto & ed(int max) {
  static int i = 0;
  if (i < 0) i = max - 1;
  if (i >= max) i = 0;
  return i;
}

static auto & cursor() {
  static dotz::ivec2 i {};
  dotz::ivec2 tbl_sz { g_table[0].size(), g_table.size() };
  i = dotz::clamp(i, { 0 }, tbl_sz - 1);
  return i;
}

static unsigned cursor_id() {
  static auto id = sprdef::get("fx/fx_orange_bite").id + 4;
  return id;
}

static unsigned font_id(char c) {
  static auto font = sprdef::get("font").id;
  return font + c;
}

static unsigned theme_id(char c) {
  temp_frame ctx {};
  themedefs::eval();

  jute::view str { &c, 1 };
  auto d = ctx.def(str);
  if (!d) return font_id(c);

  auto n = eval<node>(d);
  if (!is_atom(n)) return font_id(c);
  if (!entdefs::has(n->atom)) return font_id(c);

  return entdefs::get(n->atom).sprite;
}

static unsigned (*g_spr_id)(char) = font_id;

static void on_frame() {
  auto m = v::map();

  m->push({
    .pos = cursor(),
    .mult = 0.3,
    .id = cursor_id(),
  });

  for (auto y = 0; y < g_table.size(); y++) {
    auto & row = g_table[y];
    for (auto x = 0; x < row.size(); x++) {
      m->push({
        .pos { x, y },
        .id = g_spr_id(row.data()[x]),
      });
    }
  }

  dotz::vec2 p {
    (g_table[0].size() + 0.5f) / 2.0f,
    (g_table.size() + 0.5f) / 2.0f,
  };
  v::set_grid({ p, g_grid_size + 1 });
  v::on_frame = [] {};
}

static const node * load_room(const node * n, const node * const * aa, unsigned as) {
  if (as < 2) err(n, "rooms must have at least two rows");

  auto len0 = -1;
  g_table = hai::array<hai::cstr>{ as };
  for (auto i = 0; i < as; i++) {
    if (!is_atom(aa[i])) err(aa[i], "rooms must only have atoms as rows");

    auto len = aa[i]->atom.size();
    if (len < 2) err(aa[i], "rooms must have at least two rows");

    if (len0 == -1) len0 = len;
    else if (len != len0) err(aa[i], "all rows must have the same length");

    g_table[i] = (aa[i]->atom).cstr();
  }
  g_grid_size = static_cast<float>(dotz::max(as, len0)) / 2;
  return n;
}

static const node * load_roomdefs(const node * n, const node * const * aa, unsigned as) {
  temp_arena<node> a {};
  temp_frame ctx {}; 
  ctx.fns["room"] = load_room;
  return eval<node>(aa[ed(as)]);
}

static void load(void *, const hai::cstr & src) try {
  temp_frame ctx {}; 
  ctx.fns["roomdefs"] = load_roomdefs;
  run<node>(src);

  v::on_frame = on_frame;
} catch (const lispy::parser_error & e) {
  silog::error(lispy::to_file_err("roomdefs.lsp", e));
}

static auto mem = arena<node>::make();
static void on_init() {
  auto a = mem->use();
  sires::read("roomdefs.lsp", nullptr, load);
}

static const node * save_room(const node * n, const node * const * aa, unsigned as) {
  auto f = static_cast<FILE *>(context()->ptr("file"));
  fput(f, "  (room");
  for (auto i = 0; i < as; i++) {
    if (!is_atom(aa[i])) err(aa[i], "rooms must only have atoms as rows");
    fput(f, ' ', aa[i]->atom);
  }
  fputln(f, ")");
  return n;
}
static const node * save_roomdefs(const node * n, const node * const * aa, unsigned as) {
  auto f = static_cast<FILE *>(context()->ptr("file"));
  fputln(f, "(roomdefs");

  temp_frame ctx {};
  ctx.fns["room"] = save_room;
  for (auto i = 0; i < as; i++) {
    if (i != ed(as)) {
      auto _ = eval<node>(aa[i]);
      continue;
    }
    fput(f, "  (room");
    for (auto & row : g_table) fput(f, " ", row);
    fputln(f, ")");
  }

  fputln(f, ")");
  return n;
}
static void save_(void *, const hai::cstr & src) try {
  hay<FILE *, fopen, fclose> f { "roomdefs.lsp", "wb" };

  temp_frame ctx {};
  ctx.ptrs["file"] = static_cast<FILE *>(f);
  ctx.fns["roomdefs"] = save_roomdefs;
  run<node>(src);
} catch (const lispy::parser_error & e) {
  silog::error(lispy::to_file_err("roomdefs.lsp", e));
}

static void on_save() {
  temp_arena<node> a {};
  silog::info("Saving new rooms");
  sires::read("roomdefs.lsp", nullptr, save_);
}

static constexpr auto cursor(int x, int y) {
  return [=] {
    cursor() = cursor() + dotz::ivec2 { x, y };
    v::on_frame = on_frame;
  };
}
const int i = [] {
  using namespace casein;
  handle(KEY_DOWN, K_LBRACKET, [] { ed(0xFFFF)--; on_init(); });
  handle(KEY_DOWN, K_RBRACKET, [] { ed(0xFFFF)++; on_init(); });

  handle(KEY_DOWN, K_UP,    cursor( 0, -1));
  handle(KEY_DOWN, K_DOWN,  cursor( 0, +1));
  handle(KEY_DOWN, K_LEFT,  cursor(-1,  0));
  handle(KEY_DOWN, K_RIGHT, cursor(+1,  0));

  handle(KEY_DOWN, K_TAB, [] {
    g_spr_id = theme_id;
    if (!casein::keydown_repeating) v::on_frame = on_frame;
  });
  handle(KEY_UP, K_TAB, [] {
    g_spr_id = font_id;
    v::on_frame = on_frame;
  });

  handle(KEY_DOWN, K_ENTER, on_save);

  handle(KEY_DOWN, [] {
    temp_arena<node> a {};
    temp_frame ctx {};
    themedefs::eval();

    char c = casein::last_key;
    jute::view str { &c, 1 };
    if (!ctx.defs.has(str)) return;

    auto [x, y] = cursor();
    g_table[y].data()[x] = c;
    v::on_frame = on_frame;
  });

  res::load_all(on_init);
  return 0;
}();

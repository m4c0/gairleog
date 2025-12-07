#ifndef LECO_TARGET_WASM
#pragma leco test
#include <stdio.h>
#endif

import dotz;
import jojo;
import jute;
import hay;
import mtime;
import pprent;
import print;
import sysstd;
import stubby;

static stbi::pixel out[1024 * 1024];

static unsigned next_id = 1;
static FILE * lsp;

static void process(jute::view file) {
  auto fid = file.split('/').after.rsplit('.').before;
  auto id = next_id;
  auto img = stbi::load(jojo::slurp(file));
  auto data = reinterpret_cast<const stbi::pixel *>(*img.data);
  if (img.num_channels != 4) return;

  for (auto e = 0; e < img.height / 16; e++) {
    for (auto d = 0; d < img.width / 16; d++) {
      dotz::ivec2 id { next_id % 64, next_id / 64 };
      auto sp = id * 16;

      for (auto y = 0; y < 16; y++) {
        auto yp = out + (sp.y + y) * 1024 + sp.x;
        auto yo = data + (y + e * 16) * img.width + d * 16;
        for (auto x = 0; x < 16; x++) {
          yp[x] = yo[x];
        }
      }
      next_id++;
    }
  }

  fputln(lsp, "(sprdef ", fid, " ", id, " ", next_id - id, ")");
}

static void recurse(const char * dir) {
  auto dirv = jute::view::unsafe(dir);
  for (auto d : pprent::list(dir)) {
    if (d[0] == '.') continue;
    auto dv = jute::view::unsafe(d);
    auto p = (dirv + "/" + dv).cstr();
    jute::view pv { p };
    if (pv.ends_with(".png")) process(pv);
    if (dv.split('.').after == "") recurse(p.begin());
  }
}

static void embed_font() {
  auto id = next_id;
  auto img = stbi::load(jojo::slurp("dungeon-437.png"));
  auto * ptr = reinterpret_cast<unsigned *>(*img.data);
  for (auto cy = 0; cy < 16; cy++) {
    for (auto cx = 0; cx < 16; cx++) {
      dotz::ivec2 id { next_id % 64, next_id / 64 };
      auto sp = id * 16;

      auto pp = cy * 16 * 256 + cx * 16;
      for (auto y = 0; y < 16; y++) {
        auto yp = out + (sp.y + y) * 1024 + sp.x;
        for (auto x = 0; x < 16; x++) {
          unsigned char n = 0xFF & ((ptr[pp + y * 256 + x] & 0xFF00) >> 8);
          yp[x] = { n, n, n, n };
        }
      }

      next_id++;
    }
  }

  fputln(lsp, "(sprdef font ", id, " ", next_id - id, ")");
}

int main() {
  if (mtime::of("sprites/pixelite2.lsp") && mtime::of("sprites/pixelite2.png")) return 0;

  hay<FILE *, sysstd::fopen, fclose> lsp { "sprites/pixelite2.lsp", "wb" };
  fputln(lsp, "(sprdef empty 0 0)");

  ::lsp = lsp;
  recurse("PixeLike2_AssetPack");

  embed_font();

  stbi::write_rgba_unsafe("sprites/pixelite2.png", 1024, 1024, out);
}

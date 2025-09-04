#ifndef LECO_TARGET_WASM
#pragma leco test
#include <stdio.h>
#endif

import dotz;
import jute;
import hay;
import pprent;
import print;
import sysstd;
import stubby;

static stbi::pixel out[1024 * 1024];

static unsigned next_id = 1;
static hay<FILE *, sysstd::fopen, fclose> lsp { "sprites/pixelite2.lsp", "wb" };

static void process(jute::view file) {
  auto fid = file.split('/').after.rsplit('.').before;
  fputln(lsp, "(sprdef ", fid, " ", next_id, ")");
  stbi::load(file, nullptr, [&](auto, auto & img) {
    auto data = reinterpret_cast<const stbi::pixel *>(*img.data);
    // TODO: deal with animations
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
  });
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

int main() {
  recurse("PixeLike2_AssetPack");
  stbi::write_rgba_unsafe("sprites/pixelite2.png", 1024, 1024, out);
}

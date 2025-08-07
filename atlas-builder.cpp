#pragma leco test

import dotz;
import jute;
import pprent;
import print;
import stubby;

static stbi::pixel out[1024 * 1024];

static unsigned next_id = 0;

static void process(jute::view file) {
  dotz::ivec2 id { next_id % 64, next_id / 64 };
  auto sp = id * 16;

  stbi::load(file, nullptr, [&](auto, auto & img) {
    auto data = reinterpret_cast<const stbi::pixel *>(*img.data);
    // TODO: deal with animations
    if (img.num_channels != 4) return;

    for (auto y = 0; y < 16; y++) {
      auto yp = out + (sp.y + y) * 1024 + sp.x;
      auto yo = data + y * img.width;
      for (auto x = 0; x < 16; x++) {
        yp[x] = yo[x];
      }
    }
    next_id++;
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

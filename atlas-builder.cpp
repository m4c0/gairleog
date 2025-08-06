#pragma leco tool

import jute;
import pprent;
import print;

static void recurse(const char * dir) {
  auto dirv = jute::view::unsafe(dir);
  for (auto d : pprent::list(dir)) {
    if (d[0] == '.') continue;
    auto dv = jute::view::unsafe(d);
    auto p = (dirv + "/" + dv).cstr();
    jute::view pv { p };
    if (pv.ends_with(".png")) putln(p);
    if (dv.split('.').after == "") recurse(p.begin());
  }
}

int main() {
  recurse("PixeLike2_AssetPack");
}

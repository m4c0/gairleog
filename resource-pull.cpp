#pragma leco pretest

import glispy;
import hai;
import jojo;
import jute;
import mtime;
import pprent;
import print;
import sv;

static hai::cstr find(jute::view parent, sv name) {
  for (auto n : pprent::list(parent.begin())) {
    if (n[0] == '.') continue;

    auto dir = parent + "/" + sv::unsafe(n);
    if (name == sv::unsafe(n)) return dir.cstr();

    auto f = find(dir.cstr(), name);
    if (f.size()) return f;
  }
  return {};
}

int main() {
  lispy::temp_arena<lispy::node> a {};
  auto g = glispy::frame_guard();
  lispy::temp_frame ctx {};
  ctx.fns["sfxdef"] = [](auto n, auto aa, auto as) -> const lispy::node * {
    auto fname = ("sfx/"_sv + aa[0]->atom).cstr();
    if (mtime::of(fname.begin())) return n;

    auto src = find("assets", aa[0]->atom);
    if (src.size() == 0) die("file not found: ", aa[0]->atom);

    putan(src);
    jojo::write(fname, jojo::slurp(src));
    return n;
  };
  lispy::run<lispy::node>("sfxdefs.lsp", jojo::slurp("sfxdefs.lsp"));
}

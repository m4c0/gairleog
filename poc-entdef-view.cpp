#pragma leco tool

import entdefs;
import glispy;
import hai;
import jojo;
import jute;
import print;
import res;
import save;

using namespace lispy;

int main() try {
  hai::varray<jute::heap> names { 128 };

  temp_arena<node> a {};
  temp_frame ctx {};
  ctx.ptrs["names"] = &names;
  ctx.fns["entdef"] = [](auto n, auto aa, auto as) -> const node * {
    if (as == 0) erred(n, "missing name");
    if (!is_atom(aa[0])) erred(aa[0], "expecting atom as name");

    auto * ptr = static_cast<hai::varray<jute::heap> *>(context()->ptr("names"));
    ptr->push_back_doubling(aa[0]->atom);
    return n;
  };

  run<node>("entdefs.lsp", jojo::slurp("entdefs.lsp"));

  res::load_locally([] {});

  save::current_stage = 40;
  glispy::reset();

  putln("                Lif Att Def");
  auto g = glispy::frame_guard();
  for (auto n : names) {
    auto t = entdefs::get(n);
    if (!t.flags.enemy) continue;

    auto name = (t.name + "               ").heap();
    auto life = ("        " + jute::to_s(t.max_life)).heap();
    auto attk = ("        " + jute::to_s(t.strength)).heap();
    auto defn = ("        " + jute::to_s(t.defense)).heap();
    putan(
        (*name).subview(15).before,
        (*life).subview(life.size() - 3).after,
        (*attk).subview(attk.size() - 3).after,
        (*defn).subview(defn.size() - 3).after);
  }
} catch (const parser_error & e) {
  errln(to_file_err(e));
  return 1;
} catch (death) {
  return 2;
} catch (...) {
  return 3;
}

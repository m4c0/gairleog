#pragma leco test

import glispy;
import hai;
import jojo;
import lootfx;
import print;

// Mostly testing lisp syntax
int main() try {
  lootfx::run(jojo::slurp("lootfx.lsp"));
  if (lootfx::has(10)) die("sprites prematurely applied");

  lootfx::reset();
  if (lootfx::has(10)) die("sprites prematurely applied after reset");

  auto _ = lootfx::apply(10);
  auto n = lootfx::get(10);
  if (!n.size()) die("effect selection not working");

  for (auto i = 0; i < 1000; i++) auto _ = lootfx::apply(10);
  if (n != lootfx::get(10)) die("loot fx changed without reset");

  for (auto rep = 0; rep < 1000; rep++) {
    lootfx::reset();
    if (lootfx::has(10)) die("sprites are not resetting");

    auto _ = lootfx::apply(10);
    auto m = lootfx::get(10);
    if (m == n) continue;

    for (auto i = 0; i < 1000; i++) auto _ = lootfx::apply(10);
    if (m != lootfx::get(10)) die("loot fx inconsistent after reset");

    auto _ = lootfx::apply_by_name("level-exit-fx");
    return 0;
  }

  die("loot fx not really resetting");
} catch (const lispy::parser_error & err) {
  errln(lispy::to_file_err(err));
  return 1;
} catch (const hai::cstr & err) {
  errln(err);
  return 2;
} catch (death) {
  return 3;
}


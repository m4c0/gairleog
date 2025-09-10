#pragma leco test
#pragma leco add_resource "tiledefs.lsp"

import lispy;
import print;
import sprdef;
import tiledefs;

int main() try {
  sprdef::load("sprites/pixelite2.lsp", [](auto sprdefs) {
    tiledefs::load("tiledefs.lsp", sprdefs, [](auto tiledefs) {
    });
  });
} catch (const lispy::parser_error & e) {
  errln("tiledefs.lsp:", e.line, ":", e.col, ": ", e.msg);
  return 1;
}

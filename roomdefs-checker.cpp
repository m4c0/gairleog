#pragma leco test
import lispy;
import print;
import roomdefs;
import sprdef;
import tiledefs;

int main() try {
  sprdef::load("sprites/pixelite2.lsp", [](auto sprs) {
    tiledefs::load("tiledefs.lsp", sprs, [&](auto tdefs) {
      roomdefs::load(tdefs, "roomdefs.lsp", [&](auto rooms) {
      });
    });
  });
} catch (lispy::parser_error e) {
  errln("roomdefs.lsp:", e.line, ":", e.col, ": ", e.msg);
  return 3;
}

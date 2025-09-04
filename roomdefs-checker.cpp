#pragma leco test
import lispy;
import print;
import sprdef;
import roomdefs;

int main() try {
  sprdef::load("sprites/pixelite2.lsp", [](auto sprs) {
    roomdefs::load(sprs, "roomdefs.lsp", [&](auto rooms) {
    });
  });
} catch (lispy::parser_error e) {
  errln("roomdefs.lsp:", e.line, ":", e.col, ": ", e.msg);
  return 3;
}

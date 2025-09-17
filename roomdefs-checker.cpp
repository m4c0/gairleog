#pragma leco test
import jute;
import lispy;
import print;
import res;
import roomdefs;
import sprdef;
import tiledefs;

static void report(jute::view file, const lispy::parser_error e) {
  errln(file, ":", e.line, ":", e.col, ": ", e.msg);
}

static void check_rooms() {}

static void read_rooms() try {
  roomdefs::load(check_rooms);
} catch (lispy::parser_error e) {
  report("roomdefs.lsp", e);
}

static void read_tdefs() try {
  tiledefs::load(read_rooms);
} catch (lispy::parser_error e) {
  report("tiledefs.lsp", e);
}

int main() try {
  res::on_error([](auto msg) { errln(msg); });

  sprdef::load("sprites/pixelite2.lsp", read_tdefs);
} catch (lispy::parser_error e) {
  report("sprites/pixelite2.lsp", e);
  return 3;
}

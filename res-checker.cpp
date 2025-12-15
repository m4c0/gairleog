#pragma leco test
import entdefs;
import errs;
import hai;
import hitdefs;
import lispy;
import print;
import res;
import roomdefs;

int main() try {
  res::load_locally([] {
    // TODO: create strategies to test each resource throughly
    roomdefs::for_size(2, 2);
    roomdefs::for_size(8, 6);

    auto e = entdefs::get("player");
    assert(e.attack_sprite.id, "missing player attack sprite");
  });
} catch (const lispy::parser_error & e) {
  errln(lispy::to_file_err(e));
  return 1;
} catch (hai::cstr & e) {
  errln(e);
  return 1;
} catch (...) {
  return 2;
}

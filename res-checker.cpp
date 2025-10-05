#pragma leco test
import hai;
import hitdefs;
import print;
import res;
import roomdefs;

int main() try {
  res::load_locally([] {
    // TODO: create strategies to test each resource throughly
    roomdefs::for_size(2, 2);
    roomdefs::for_size(8, 6);
  });
} catch (hai::cstr & e) {
  errln(e);
  return 1;
} catch (...) {
  return 2;
}

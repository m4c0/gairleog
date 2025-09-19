#pragma leco test
import print;
import res;
import roomdefs;

int main() try {
  int res = 0;
  res::on_error([&](auto msg) { errln(msg); res = 1; });
  res::load_locally([] {
    roomdefs::for_size(2, 2);
    roomdefs::for_size(8, 6);
  });
  return res;
} catch (...) {
  return 2;
}

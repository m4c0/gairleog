#pragma leco test
import print;
import res;

int main() try {
  int res = 0;
  res::on_error([&](auto msg) { errln(msg); res = 1; });
  res::load_locally([] {});
  return res;
} catch (...) {
  return 2;
}

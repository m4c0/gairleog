#pragma leco test
import hai;
import hitdefs;
import jute;
import print;
import res;
import rng;

void run() {
  hai::array<jute::view> src_compos { 1 };
  src_compos[0] = "player";

  hai::chain<jute::view> tgt_compos {};
  tgt_compos.push_back("enemy");
  tgt_compos.push_back("toad");
  for (auto action : hitdefs::check(src_compos, tgt_compos)) {
    action();
  }

  putln("flipping");
  for (auto action : hitdefs::check(tgt_compos, src_compos)) {
    action();
  }
}

int main() try {
  rng::seed();

  int res = 0;
  res::on_error([&](auto msg) { errln(msg); res = 1; });
  res::load_locally(&run);
  return res;
} catch (...) {
  return 2;
}

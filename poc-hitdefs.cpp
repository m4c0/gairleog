#pragma leco test
import hai;
import hitdefs;
import jute;
import print;
import res;
import rng;

void run() {
  // Testing with two different array types, just because
  hai::array<jute::view> src_compos { 2 };
  src_compos[0] = "player";
  src_compos[1] = "solid";

  hai::chain<jute::view> tgt_compos {};
  tgt_compos.push_back("enemy");
  tgt_compos.push_back("toad");
  tgt_compos.push_back("solid");
  for (auto action : hitdefs::check(src_compos, tgt_compos)) {
    action();
  }

  putln("<>");
  for (auto action : hitdefs::check(tgt_compos, src_compos)) {
    action();
  }
}

int main() try {
  rng::seed();
  res::load_locally(&run);
} catch (const hai::cstr & msg) {
  errln(msg);
  return 1;
} catch (...) {
  return 2;
}

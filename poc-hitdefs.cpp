#pragma leco test
import hai;
import hitdefs;
import print;
import res;
import rng;
import tiledefs;

void run() {
  tiledefs::flags src_compos {
    .player = true,
    .solid  = true,
  };
  tiledefs::flags tgt_compos {
    .enemy = true,
    .solid = true,
    .toad  = true,
  };

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

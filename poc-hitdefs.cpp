#pragma leco test
import entdefs;
import hai;
import hitdefs;
import print;
import res;
import rng;

void run() {
  entdefs::flags src_compos {
    .player = true,
    .solid  = true,
  };
  entdefs::flags tgt_compos {
    .enemy = true,
    .solid = true,
    .toad  = true,
  };

  for (auto action : hitdefs::check(src_compos, tgt_compos)) {
    put(static_cast<int>(action), ' ');
  }
  putln();
  for (auto action : hitdefs::check(tgt_compos, src_compos)) {
    put(static_cast<int>(action), ' ');
  }
  putln();
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

#pragma leco test
import entdefs;
import errs;
import hai;
import hitdefs;
import lispy;
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
    .fluorescent = true,
    .solid = true,
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
} catch (const lispy::parser_error & err) {
  errln(lispy::to_file_err(err));
  return 1;
} catch (const hai::cstr & msg) {
  errln(msg);
  return 1;
} catch (...) {
  return 2;
}

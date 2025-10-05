#pragma leco test
import hitdefs;
import print;
import res;

void run() {
  for (auto action : hitdefs::check("player", "pot")) {
    action();
  }
  for (auto action : hitdefs::check("toad", "player")) {
    action();
  }
}

int main() try {
  int res = 0;
  res::on_error([&](auto msg) { errln(msg); res = 1; });
  res::load_locally(&run);
  return res;
} catch (...) {
  return 2;
}

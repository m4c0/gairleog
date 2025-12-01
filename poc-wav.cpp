#pragma leco test

import jojo;
import sv;

static bool run(sv data) {
  auto ptr = reinterpret_cast<const unsigned *>(data.begin());
  if (*ptr++ != 'FFIR') return false;
  if (*ptr++ != data.size() - 8) return false;
  if (*ptr++ != 'EVAW') return false;

  return true;
}

int main() {
  auto data = jojo::slurp("sfx/01_human_atk_sword_1.wav");
  return run(data) ? 0 : 1;
}

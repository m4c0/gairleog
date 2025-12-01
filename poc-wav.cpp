#pragma leco test

import jojo;
import jute;
import print;
import sv;
import wav;

int main() try {
  auto data = jojo::slurp("sfx/01_human_atk_sword_1.wav");
  auto wav = wav::load(data);

  putln("Num samples = ", wav.size());
  putln("Length = ", wav.size() / 44100.f);
} catch (const wav::error & e) {
  errln(e.msg);
  return 1;
}

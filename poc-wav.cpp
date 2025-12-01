#pragma leco tool

import jojo;
import print;
import siaudio;
import wav;

int main() try {
  static auto wav = wav::load(jojo::slurp("sfx/01_human_atk_sword_1.wav"));
  static unsigned ptr = 0;

  putln("Num samples = ", wav.size());
  putln("Length = ", wav.size() / 44100.f);

  siaudio::filler([](float * data, unsigned samples) {
    for (auto i = 0; i < samples; i++) {
      *data++ = ptr >= wav.size() ? 0 : wav[ptr++];
    }
  });
  siaudio::rate(44100);

  while (ptr < wav.size()) { /* wait */ }
} catch (const wav::error & e) {
  errln(e.msg);
  return 1;
}

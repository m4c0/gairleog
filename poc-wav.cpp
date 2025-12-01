#pragma leco test

import jojo;
import jute;
import print;
import sv;

struct error {
  jute::heap msg;
};

[[noreturn]] static void erred(sv msg) {
  throw error { .msg { msg } };
}

static void run(sv data) {
  auto ptr = reinterpret_cast<const unsigned *>(data.begin());
  if (*ptr++ != 'FFIR') erred("Invalid WAV file");
  if (*ptr++ != data.size() - 8) erred("WAV file size does not match");
  if (*ptr++ != 'EVAW') erred("Unsupported RIFF file");

  if (*ptr++ != ' tmf') erred("WAV files should have a FMT chunk");
  if (*ptr++ != 16) erred("Unsupported WAV FMT size");
  // WORD wFormatTag; // Format category
  // WORD wChannels; // Number of channels
  // DWORD dwSamplesPerSec; // Sampling rate
  // DWORD dwAvgBytesPerSec; // For buffer estimation
  // WORD wBlockAlign; // Data block size
}

int main() try {
  auto data = jojo::slurp("sfx/01_human_atk_sword_1.wav");
  run(data);
} catch (const error & e) {
  errln(e.msg);
  return 1;
}

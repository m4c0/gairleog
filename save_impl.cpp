module save;
import ents;
import file;
import fx;
import inv;
import lispy;
import lootfx;
import res;
import silog;

static bool g_exists = false;

// Currently, "save" is also the "state" manager
static void reset() {
  silog::log(silog::info, "Game reset");
  g_exists = false;
  ents::reset();
  fx::reset();
  inv::reset();
  lootfx::reset();
}

void save::reset(hai::fn<void> callback) {
  ::reset();
  store(callback);
}

void save::prefetch(hai::fn<void> callback) {
  ::reset();

  try {
    file::reader f {};
    ents::read(&f);
    inv::read(&f);
    lootfx::read(&f);
    
    silog::log(silog::info, "Game prefetched");
    g_exists = true;
  } catch (...) {
    ::reset();
  }

  callback();
}

bool save::exists() {
  return g_exists;
}

void save::store(hai::fn<void> callback) {
  silog::log(silog::info, "Saving game");

  try {
    file::writer f {};
    ents::write(&f);
    inv::write(&f);
    lootfx::write(&f);

    silog::log(silog::info, "Game saved");
  } catch (...) {
    silog::log(silog::error, "Error saving game");
  }
  g_exists = true;
  callback();
}

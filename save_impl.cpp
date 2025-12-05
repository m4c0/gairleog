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
  ents::reset();
  fx::reset();
  inv::reset();
  lootfx::reset();
}

void save::prefetch(hai::fn<void> callback) {
  g_exists = false;
  reset();

  try {
    file::reader f {};
    ents::read(&f);
    inv::read(&f);
    lootfx::read(&f);
    
    silog::log(silog::info, "Game prefetched");
    g_exists = true;
  } catch (...) {
    silog::log(silog::info, "Game reset");
    reset();
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

void save::init(hai::fn<void> callback) try {
  res::load_all(callback);
} catch (const lispy::parser_error & e) {
  silog::die("%s", lispy::to_file_err(e).begin());
} catch (const hai::cstr & e) {
  silog::die("Failure loading resource: %s", e.begin());
}

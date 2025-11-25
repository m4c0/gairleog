module save;
import ents;
import file;
import fx;
import inv;
import lootfx;
import res;
import silog;

static hai::fn<void> g_callback {};
static bool g_exists = false;

// Currently, "save" is also the "state" manager
static void invoke_callback() {
  ents::reset();
  fx::reset();
  inv::reset();
  lootfx::reset();

  if (g_callback) g_callback();
}

void save::prefetch(hai::fn<void> callback) {
  g_callback = callback;
  g_exists = false;

  file::read f {};
  if (!f) {
    silog::log(silog::info, "Game reset");
    return invoke_callback();
  }

  silog::log(silog::info, "Game prefetched");
  g_exists = true;
  invoke_callback();
}

bool save::exists() {
  return g_exists;
}

void save::store(hai::fn<void> callback) {
  silog::log(silog::info, "Saving game");

  g_exists = true;
  callback();
}

void save::init(hai::fn<void> callback) try {
  res::load_all(callback);
} catch (const hai::cstr & e) {
  silog::die("Failure loading resource: %s", e.begin());
}

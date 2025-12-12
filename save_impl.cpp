module save;
import ents;
import file;
import fx;
import inv;
import lootfx;
import silog;
import splats;

static constexpr const unsigned id = 'GAIR';
static constexpr const unsigned version = 1001;

static bool g_exists = false;

// Currently, "save" is also the "state" manager
static void reset() {
  silog::log(silog::info, "Game reset");
  g_exists = false;
  save::current_stage = 0;
  ents::reset();
  fx::reset();
  inv::reset();
  lootfx::reset();
}

void save::reset(hai::fn<void> callback) {
  ::reset();
  { file::writer f {}; }
  callback();
}

void save::prefetch(hai::fn<void> callback) {
  ::reset();

  try {
    file::reader f {};
    if (!f) return callback();

    if (f.read<unsigned>() != id) throw 0;
    if (f.read<unsigned>() != version) throw 0;
    current_stage = f.read<int>();
    ents::read(&f);
    inv::read(&f);
    lootfx::read(&f);
    splats::read(&f);
    
    silog::log(silog::info, "Game prefetched");
    g_exists = true;
  } catch (...) {
    silog::error("Invalid save file");
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
    f.write(id);
    f.write(version);
    f.write(current_stage);
    ents::write(&f);
    inv::write(&f);
    lootfx::write(&f);
    splats::write(&f);

    silog::log(silog::info, "Game saved");
  } catch (...) {
    silog::log(silog::error, "Error saving game");
  }
  g_exists = true;
  callback();
}

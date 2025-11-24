module save;
import buoy;
import jojo;
import silog;

static const auto file = buoy::path("gairleog", "save.dat");

static hai::fn<void> g_callback {};
static bool g_exists = false;

static void invoke_callback() {
  if (g_callback) g_callback();
}

void save::prefetch(hai::fn<void> callback) {
  g_callback = callback;
  jojo::read(file, &jid, [](auto, auto &) {
    silog::log(silog::info, "Game prefetched");
    g_exists = true;
    invoke_callback();
  });
}

void save::reset() {
  silog::log(silog::info, "Game reset");
  g_exists = false;
  invoke_callback();
}

bool save::exists() {
  return g_exists;
}

void save::store(hai::fn<void> callback) {
  g_exists = true;
  callback();
}

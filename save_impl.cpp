module save;
import buoy;
import jojo;

static const auto file = buoy::path("gairleog", "save.dat");

void save::prefetch(hai::fn<void> callback) {
  jojo::read(file, nullptr, [=](void *, hai::cstr & file) mutable {
    callback();
  });
}

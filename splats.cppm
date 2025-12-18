export module splats;
import dotz;
import file;
import hai;
import rng;
import silog;
import sprdef;

namespace splats {
  struct t {
    dotz::vec2 pos;
    dotz::vec2 size { 1 };
    unsigned sprite;
    float rotation = 0;
  };
  hai::varray<t> data { 128 };

  export void reset() {
    data.truncate(0);
  }

  export void add(t t) {
    t.rotation = rng::rand_i(0, 3) * 90;
    t.size.x = rng::rand_i(0, 1) * 2 - 1;
    t.size.y = rng::rand_i(0, 1) * 2 - 1;
    data.push_back_doubling(t);
  }

  export void for_each(auto && fn) {
    for (const auto & d : data) fn(d);
  }

  export void read(file::reader * r) {
    data.truncate(0);

    auto len = r->read<unsigned>();
    silog::infof("Reading %d splats", len);
    for (auto i = 0; i < len; i++) {
      data.push_back_doubling(r->read<t>());
    }
  }
  export void write(file::writer * w) {
    silog::infof("Storing %d entities", data.size());
    w->write(data.size());
    for (auto d : data) w->write(d);
  }
}

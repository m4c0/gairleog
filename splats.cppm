export module splats;
import dotz;
import file;
import hai;
import silog;
import sprdef;

namespace splats {
  // TODO: rotate/flip for variety
  struct t {
    dotz::vec2 pos;
    unsigned sprite;
  };
  hai::varray<t> data { 128 };

  export void reset() {
    data.truncate(0);
  }

  export void add(t t) {
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

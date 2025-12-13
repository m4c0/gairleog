export module inv; 
import entdefs;
import file;
import hai;
import jute;
import silog;
import sv;

namespace inv {
  hai::varray<entdefs::t> data { 128 };

  export void reset() {
    data.truncate(0);
  }

  export void add(entdefs::t tdef) {
    data.push_back_doubling(tdef);
  }

  static entdefs::t invalid {};
  export const auto & at(int idx) {
    if (idx < 0 || idx >= data.size()) return invalid;
    return data[idx];
  }

  export auto size() { return data.size(); }

  export void consume(int sel) {
    if (sel < 0 || sel >= data.size()) return;
    for (auto i = sel; i < data.size() - 1; i++) data[i] = data[i + 1];
    data.pop_back();
  }

  export void read(file::reader * r) {
    data.truncate(0);

    auto len = r->read<unsigned>();
    silog::infof("Reading %d inventory entries", len);
    for (auto i = 0; i < len; i++) {
      entdefs::t val {};
      val.loot = r->read<jute::heap>();
      static_cast<entdefs::data &>(val) = r->read<entdefs::data>();
      data.push_back_doubling(val);
    }
  }
  export void write(file::writer * w) {
    silog::infof("Storing %d inventory entries", data.size());
    w->write(data.size());
    for (const auto & d : data) {
      w->write(d.loot);
      w->write(static_cast<entdefs::data>(d));
    }
  }
}

export module inv; 
import entdefs;
import file;
import hai;
import jute;
import silog;
import sv;

namespace inv {
  export struct t : entdefs::t {
  };

  hai::varray<t> data { 128 };

  export void reset() {
    data.truncate(0);
  }

  export void add(entdefs::t tdef) {
    data.push_back_doubling(t { tdef });
  }

  static t invalid {};
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
      auto loot = r->read<jute::heap>();
      auto val = r->read<t>();
      val.loot = loot;
      data.push_back_doubling(val);
    }
  }
  export void write(file::writer * w) {
    silog::infof("Storing %d inventory entries", data.size());
    w->write(data.size());
    for (auto d : data) {
      w->write(d.loot);
      d.loot = {};
      w->write(d);
    }
  }
}

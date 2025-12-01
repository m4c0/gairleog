export module errs;
import jojo;
import jute;
import save;

namespace errs {
  export struct jid {} jid;
};

const int i = [] {
  jojo::on_error([](void * ptr, jute::view msg) {
    if (ptr == &errs::jid) throw msg.cstr();
  });

  return 0;
}();


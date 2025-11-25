export module errs;
import jojo;
import jute;
import res;
import save;

const int i = [] {
  jojo::on_error([](void * ptr, jute::view msg) {
    if (ptr == &res::jid) throw msg.cstr();
  });

  return 0;
}();


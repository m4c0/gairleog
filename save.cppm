#pragma leco add_impl save_impl

export module save;
import hai;

namespace save {
  export struct jid {} jid {};

  export bool exists();

  export void prefetch(hai::fn<void> callback);
  export void store(hai::fn<void> callback);
  export void reset(hai::fn<void> callback);
}

export module mainloop;

export namespace mainloop {
  using fn_t = void (*)(void);
  void push(fn_t);
}

#ifdef LECO_TARGET_WASM
#pragma leco add_impl mainloop_wasm
#else
#pragma leco add_impl mainloop_threads
#endif

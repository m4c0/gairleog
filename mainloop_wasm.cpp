module mainloop;
import vaselin;

void mainloop::push(mainloop::fn_t fn) {
  vaselin::set_timeout([](void * ptr) {
    reinterpret_cast<fn_t>(ptr)();
  }, reinterpret_cast<void *>(fn), 0);
}

#pragma leco tool

import mainloop;
import silog;
import sith;
import sitime;

_Atomic(int) g_count = 0;
void thread(sith::thread * t) {
  silog::infof("producer thread %p", t);
  while (!t->interrupted()) {
    silog::debug("pushing");
    sitime::sleep_ms(100);
    mainloop::push([] {
      silog::trace("processing", g_count++);
      sitime::sleep_ms(100);
    });
  }
}

int main() {
  sith::stateless_thread t1 { thread };
  sith::stateless_thread t2 { thread };

  sith::run_guard g1 { &t1 };
  sith::run_guard g2 { &t2 };

  sitime::sleep(100);

  silog::info("joining");
}

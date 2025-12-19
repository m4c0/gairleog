#pragma leco test

import silog;
import sith;

_Atomic(int) g_count = 0;

void consume(sith::thread * t) {
}
void produce(int n) {
}

void thread(sith::thread * t) {
  silog::infof("I'm thread %p", t);
  while (!t->interrupted()) consume(t);
}

int main() {
  sith::stateless_thread t1 { thread };
  sith::stateless_thread t2 { thread };

  sith::run_guard g1 { &t1 };
  sith::run_guard g2 { &t2 };

  for (auto i = 0; i < 10; i++) produce(i);

  silog::info("joining");
}

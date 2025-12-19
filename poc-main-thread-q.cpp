#pragma leco tool

import mtx;
import silog;
import sith;
import sitime;

using fn_t = void (*)(void);

static constexpr const auto queue_size = 128;

static fn_t g_queue[queue_size] {};
static int g_next_prod = 0;
static int g_next_cons = 0;
static mtx::mutex g_mtx {};
static mtx::cond g_cond_cons {};
static mtx::cond g_cond_prod {};

static fn_t take() {
  mtx::lock l { &g_mtx };
  if (g_next_prod == g_next_cons) {
    g_cond_cons.wait(&l, 1);
    if (g_next_prod == g_next_cons) return nullptr;
  }

  auto res = g_queue[g_next_cons];
  g_next_cons = (g_next_cons + 1) % queue_size;
  g_cond_prod.wake_one();
  return res;
}
static void push(fn_t fn) {
  mtx::lock l { &g_mtx };

  auto p = (g_next_prod + 1) % queue_size;
  if (p == g_next_cons) {
    silog::warning("event thread queue is full - this might slow down the whole app");
    g_cond_prod.wait(&l, 1);
    if (p == g_next_cons) silog::die("event thread became too busy - bailing out");
  }

  g_queue[g_next_prod] = fn;
  g_next_prod = p;
  g_cond_cons.wake_one();
}

void consumer(sith::thread * t) {
  silog::infof("consumer thread %p", t);
  while (!t->interrupted()) {
    // TODO: alarm if task takes too much time
    if (auto r = take()) r();
  }
}

//-----------

_Atomic(int) g_count = 0;
void thread(sith::thread * t) {
  silog::infof("producer thread %p", t);
  while (!t->interrupted()) {
    silog::debug("pushing");
    sitime::sleep_ms(100);
    push([] {
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

  sith::stateless_thread t { consumer };
  sith::run_guard g { &t };

  sitime::sleep(100);

  silog::info("joining");
}

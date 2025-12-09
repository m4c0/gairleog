export module metric;
import jute;
import no;
import silog;
import sitime;

class timer : no::no {
  sitime::stopwatch m_t {};
  unsigned * m_acc;
public:
  explicit timer(unsigned * a) : m_acc { a } {}
  ~timer() { *m_acc += m_t.millis(); }
};
export class metric : no::no {
  jute::heap m_name;
  unsigned m_count {};
  unsigned m_acc {};

public:
  explicit metric(jute::heap name) : m_name { name } {}
  ~metric() { 
    if (m_count == 0) silog::info(jute::fmt<"Metric %s never called">(m_name));
    else silog::info(jute::fmt<"Metric %s called %d times in %dms (~%f per call)">(
          m_name, m_count, m_acc, static_cast<float>(m_acc) / m_count));
  }

  [[nodiscard]] auto operator()() {
    m_count++;
    return timer { &m_acc };
  }
};

module file;

namespace file {
  class t {};

  reader::reader() {}
  reader::~reader() {}
  void reader::read(void *, unsigned) {}

  reader::operator bool() const { return m_f; }

  writer::writer() {}
  writer::~writer() {}
  void writer::write(const void *, unsigned) {}
}

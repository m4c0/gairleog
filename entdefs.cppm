export module entdefs;
import jute;
import lispy;

namespace entdefs {
  export struct error : lispy::parser_error {};

  export void run(jute::view src) {}
}

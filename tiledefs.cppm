export module tiledefs;
import jute;

namespace tiledefs {
  export struct t {
    float light {};
    unsigned sprite {};

    bool enemy {};
    bool food {};
    bool player {};
    bool pot {};
    bool solid {};
    bool toad {};
  };

  export unsigned bit_of(jute::view atom) {
    if (atom == "enemy")  return 1U << 0U;
    if (atom == "food")   return 1U << 1U;
    if (atom == "player") return 1U << 2U;
    if (atom == "pot")    return 1U << 3U;
    if (atom == "solid")  return 1U << 4U;
    if (atom == "toad")   return 1U << 5U;
    return 0U; // Should only happen whilst parsing
  }
}

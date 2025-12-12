module lootfx;
import console;
import ents;
import lispy;

lootfx::outcome lootfx::apply_on_player(const lootfx::action_list_t & actions) {
  ents::t * player;
  ents::foreach({ .player = true }, [&](auto & p) {
    player = &p;
  });
  if (!player || player->life == 0) return outcome::death;

  for (auto act : actions) {
    switch (act) {
      using enum lootfx::action;
      case damage:
        if (!ents::take_hit(player)) return outcome::death;
        break;
      case defence:
        console::push("You feel sturdier");
        player->defense++;
        break;
      case heal:
        console::push("You got healed");
        if (player->life < player->max_life) player->life++;
        break;
      case max_life:
        console::push("Your life was extended");
        player->max_life++;
        break;
      case poison:
        console::push("You got poisoned");
        player->poison++;
        break;
      case strength:
        console::push("You feel stronger");
        player->strength++;
        break;
      case weakness:
        console::push("You feel weaker");
        if (player->strength > 0) player->strength--;
        break;
      case wither:
        console::push("You feel flimser");
        if (player->defense > 0) player->defense--;
        break;
    }
  }
  return outcome::none;
}

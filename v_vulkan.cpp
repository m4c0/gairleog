module v;
import :tools;

void v::mapper::normalise(v::sprite & s) {}

// TODO: fix a rare sync issue between player and rest of game
void v::extent_stuff::render() {
  auto cb = v::vv::ss()->sw.command_buffer();
  float pc = v::vv::ss()->sw.aspect();
  v::vv::as()->ppl.cmd_draw(cb, &pc);
}

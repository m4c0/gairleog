(def gameover-sub (random
  "You did great!"
  "Next time will be better..."
  "You were awesome!"
  "This happens to all of us"
  "Hope you had fun!"
  "Thanks for playing!"
))

(def level-exit-fx "Random")

(def wall-sprites (perlin
  environment/walls/brick_clay
  environment/walls/walls_cave_brown
  environment/walls/walls_cave_gray
  environment/walls/walls_dungeon_gray
  environment/walls/walls_dungeon_yellow
))

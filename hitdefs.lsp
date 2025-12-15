(hitdef solid solid (block))

(hitdef player ceramic (hit))

(hitdef player exit (exit))

(hitdef player food
  (pick))

(hitdef player enemy
  (hit))

(hitdef poison player
  (random
    (miss)
    (miss)
    (miss)
    (poison)))

(hitdef enemy player
  (random
    (miss)
    (miss)
    (miss)
    (hit)))


(hitdef solid solid (block))

(hitdef player ceramic (hit))

(hitdef player exit (exit))

(hitdef player food
  (pick))

(hitdef player enemy
  (hit))

(hitdef enemy player
  (random
    (miss)
    (miss)
    (miss)
    (miss)
    (miss)
    (hit)))

(hitdef toad player
  (random
    (miss)
    (miss)
    (miss)
    (miss)
    (miss)
    (poison)))

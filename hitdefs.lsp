(hitdef solid solid (block))

(hitdef player exit (exit))

(hitdef player pot (hit))

(hitdef player food
  (pick))

(hitdef player enemy
  (hit))

(hitdef toad player
  (random
    (miss)
    (miss)
    (miss)
    (miss)
    (miss)
    (do (hit) (poison))
    (do (hit) (hit))
    (poison)))

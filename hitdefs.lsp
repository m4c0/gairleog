(hitdef player pot
  (do (block) (hit)))

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

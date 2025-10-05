(hitdef player pot
  (do (block) (damage 1)))

(hitdef player food
  (pick))

(hitdef player enemy
  (damage 1))

(hitdef toad player
  (random
    (miss)
    (miss)
    (miss)
    (miss)
    (miss)
    (do (poison) (damage 1))
    (damage 2)
    (poison)))

(hitdef
  (from player)
  (to pot)
  (damage 1))

(hitdef
  (from player)
  (to food)
  (pick))

(hitdef
  (from player)
  (to enemy)
  (damage 1))

(hitdef
  (from toad)
  (to player)
  (random
    (miss)
    (miss)
    (miss)
    (miss)
    (miss)
    (damage 1)
    (damage 2)
    (poison)))

(entdef apple)
(entdef player (life 3))
(entdef pot (life 1))
(entdef toad
  (life 2)
  (loot))

(hitdef
  (from player)
  (to pot)
  (damage 1))

(hitdef
  (from player)
  (to apple)
  (pick))

(hitdef
  (from player)
  (to pot)
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

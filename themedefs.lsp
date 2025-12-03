(def easy-enemy (random snake))
(def medium-enemy (random spirit-wotw spirit-faerie))

(def per-level-enemy (first-of
  (lte (level) 1 (random (easy-enemy) empty empty))
  (lte (level) 2 (random (easy-enemy) (easy-enemy) empty))
  (lte (level) 3 (easy-enemy))
  (lte (level) 4 (random (easy-enemy) (easy-enemy) (medium-enemy)))
  (lte (level) 5 (random (easy-enemy) (medium-enemy) (medium-enemy)))
  (lte (level) 6 (medium-enemy))
  (medium-enemy)
))

(def x (random pot candles grass empty empty))
(def e (per-level-enemy))
(random
  (do
    (def . (random grass empty empty))
    (def , (random boulder boulder))
    )
  (do
    (def . (random grass empty empty))
    (def , (random tree tree tree_withered))
    )
  (do
    (def . (random grass_swamp grass_swamp empty))
    (def , (random tree_swamp tree_withered))
    )
)

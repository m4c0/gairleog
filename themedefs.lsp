(def easy-enemy (random snake spirit));

(def per-level-enemy (first-of
  (lte (level) 1 (random (easy-enemy) empty empty))
  (lte (level) 3 (random (easy-enemy) (easy-enemy) empty))
  (easy-enemy)
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

(def easy-enemy   (random snake bat wolf slime))
(def medium-enemy (random spirit-wotw spirit-faerie slime-big))
(def hard-enemy   (random beholder leech ooze))
(def super-enemy  (random demon dragon-drake dragon-hydra dragon-wyrm))

(def per-level-enemy (first-of
  (lte (level) 1 (random (easy-enemy) empty empty))
  (lte (level) 2 (random (easy-enemy) (easy-enemy) empty))
  (lte (level) 3 (easy-enemy))
  (lte (level) 4 (random (easy-enemy) (easy-enemy) (medium-enemy)))
  (lte (level) 5 (random (easy-enemy) (medium-enemy) (medium-enemy)))
  (lte (level) 6 (medium-enemy))
  (lte (level) 7 (random (hard-enemy) (medium-enemy) (medium-enemy)))
  (lte (level) 8 (random (hard-enemy) (hard-enemy) (medium-enemy)))
  (lte (level) 9 (hard-enemy))
  (lte (level) 10 (random (hard-enemy) (hard-enemy) (super-enemy)))
  (lte (level) 11 (random (hard-enemy) (super-enemy) (super-enemy)))
  (super-enemy)
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

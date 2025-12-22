(def easy-enemy   (random bat wolf slime))
(def medium-enemy (random spirit-wotw spirit-faerie slime-big))
(def hard-enemy   (random beholder ooze))
(def super-enemy  (random demon dragon-wyrm))

(def easy-poison-enemy   snake)
(def medium-poison-enemy leech)
(def hard-poison-enemy   (random dragon-drake dragon-hydra))

(def per-level-enemy (first-of
  (lte (level)  1 (random (easy-enemy) empty empty))
  (lte (level)  2 (random (easy-enemy) empty))
  (lte (level)  3 (easy-enemy))
  (lte (level)  4 (random (easy-enemy) (easy-enemy) (easy-poison-enemy)))
  (lte (level)  5 (random (easy-enemy) (easy-poison-enemy)))
  (lte (level)  7 (easy-poison-enemy))
  (lte (level) 10 (random (easy-enemy) (medium-enemy)))
  (lte (level) 13 (medium-enemy))
  (lte (level) 16 (random (easy-poison-enemy) (medium-enemy)))
  (lte (level) 19 (medium-enemy))
  (lte (level) 22 (random (medium-poison-enemy) (medium-enemy)))
  (lte (level) 23 (medium-poison-enemy))
  (lte (level) 27 (random (medium-poison-enemy) (hard-enemy)))
  (lte (level) 29 (hard-enemy))
  (lte (level) 33 (random (hard-poison-enemy) (hard-enemy)))
  (lte (level) 34 (hard-poison-enemy))
  (lte (level) 40 (super-enemy))
  (random (super-enemy) (hard-poison-enemy))
))

(def per-level-pot (first-of
  (lte (level)  9 pot-red)
  (lte (level) 18 (random pot-red pot-blue))
  (lte (level) 27 pot-blue)
  (lte (level) 36 (random pot-blue pot-other))
  (lte (level) 45 pot-other)
  (random pot-red pot-blue pot-other)
))

(def i statue)
(def o (random spiderweb spiderweb empty))
(def 0 (random spiderweb-180 spiderweb-180 empty))
(def l pillar)
(def m mannequin)
(def - empty)
(def u (random lilypad empty empty))
(def y candles)
(def p (per-level-pot))
(def q door)
(def w wall)

(random
  (do
    (def / statue)
    (def \ pillar)
    )
  (do
    (def / pillar)
    (def \ statue)
    )
  )

(def e (per-level-enemy))
(random
  (do
    (def . (random grass grass empty))
    (def , (random boulder boulder))
    )
  (do
    (def . (random grass grass empty))
    (def , (random tree tree tree_withered))
    )
  (do
    (def . (random grass_swamp grass_swamp empty))
    (def , (random tree_swamp tree_withered))
    )
  (do
    (def . (random grass_swamp grass_swamp empty))
    (def , mushroom)
    )
)

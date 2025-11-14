(fx Heal (do
  (heal)
  (random (heal) ())
  (random (heal) ())))

(fx Damage (damage))
(fx Defence (defence))
(fx Strength (strength))

(fx "+1Str -1Def" (do (strength) (wither)))
(fx "+1Def -1Str" (do (weakness) (defence)))

(fx "+1 Life" (maxlife))

(fx Poison (do
  (poison)
  (random (poison) ())
  (random (poison) ())))

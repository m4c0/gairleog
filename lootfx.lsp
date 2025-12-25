(fxdef Heal (do
  (heal)
  (random (heal) ())
  (random (heal) ())))

(fxdef Damage (damage))
(fxdef Defence (defence))
(fxdef Strength (strength))

(fxdef "+1Str -1Def" (do (strength) (wither)))
(fxdef "+1Def -1Str" (do (weakness) (defence)))

(fxdef "+1 Life" (maxlife))

(fxdef Poison (do
  (poison)
  (random (poison) ())
  (random (poison) ())))

(fxdef "Random" (random
  (heal)
  (defence)
  (strength)
  (maxlife)
))

(def level-exit-fx ("Random"))

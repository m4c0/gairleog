(fx Heal (do
  (heal)
  (random (heal) ())
  (random (heal) ())))

(random
  (do
    (fx Strength (strength))
    (fx Defence (defence)))
  (do
    (fx "+1Str -1Def" (do (strength) (wither)))
    (fx "+1Def -1Str" (do (weakness) (defence)))))

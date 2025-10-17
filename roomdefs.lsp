(def X (random pot candles grass empty empty))
(themedef
  (random
    (do
      (def . (random grass empty empty))
      (def ! (random boulder boulder)))
    (do
      (def . (random grass empty empty))
      (def ! (random tree tree tree_withered)))
    (do
      (def . (random grass_swamp grass_swamp empty))
      (def ! (random tree_swamp tree_withered)))
  ))

(roomdefs
  (room .. ..)
  (room !. .!)
  (room !. ..)
  (room !! ..)
  (room !! !.)
  (room !! !!)

  (room ... ...)
  (room !.. .!.)
  (room !.. ...)
  (room !!. ...)
  (room !.! ...)
  (room !.. !..)
  (room !.! !.!)

  (room .X.. ....)
  (room ..X.. .....)
  (room ..X... ......)
  (room ...X... .......)
  (room ...X.... ........)
  (room ....X.... .........)
  (room ....X..... ..........)

  (room .... ..X. ....)
  (room ..... ..X.. .....)
  (room ...... ..X... ......)
  (room ....... ...X... .......)
  (room ........ ...X.... ........)
  (room ......... ....X.... .........)
  (room .......... ....X..... ..........)

  (room ..!. .... .... ....)
  (room ..!. .... ...! !...)
  (room .... .... .... ....)
)

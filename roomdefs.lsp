(def empty (tile (spr empty)))

(def boulder (tile
  (block)
  (random
    (spr environment/props/boulder1)
    (spr environment/props/boulder2)
    (spr environment/props/boulder3)
    (spr environment/props/boulder4)
  )))

(def candle (tile
  (light 1)
  (spr environment/props/candles)))

(def pot (tile
  (block)
  (random
    (spr environment/props/pot_yellow)
    (spr environment/props/pot_black)
    (spr environment/props/pot_blue)
    (spr environment/props/pot_green)
    (spr environment/props/pot_purple)
    (spr environment/props/pot_orange)
    (spr environment/props/pot_red)
    (spr environment/props/pot_indigo)
    (spr environment/props/pot_brown)
    (spr environment/props/pot_gray)
  )))

(def grass (tile
  (random
    (spr environment/floor_tiles/grass1)
    (spr environment/floor_tiles/grass2)
    (spr environment/floor_tiles/grass3)
    (spr environment/floor_tiles/grass4)
  )))
  
(def grass_swamp (tile
  (random
    (spr environment/floor_tiles/grass_swamp1)
    (spr environment/floor_tiles/grass_swamp2)
    (spr environment/floor_tiles/grass_swamp3)
    (spr environment/floor_tiles/grass_swamp4)
  )))

(def stone (tile
  (random
    (spr environment/floor_tiles/stone1)
    (spr environment/floor_tiles/stone2)
    (spr environment/floor_tiles/stone3)
    (spr environment/floor_tiles/stone4)
  )))

(def tree (tile
  (block)
  (random
    (spr environment/props/tree1)
    (spr environment/props/tree2)
    (spr environment/props/tree3)
    (spr environment/props/tree4)
  )))

(def tree_swamp (tile
  (block)
  (random
    (spr environment/props/tree1_swamp)
    (spr environment/props/tree2_swamp)
    (spr environment/props/tree3_swamp)
    (spr environment/props/tree4_swamp)
  )))

(def tree_withered (tile
  (block)
  (random
    (spr environment/props/tree1_withered)
    (spr environment/props/tree2_withered)
    (spr environment/props/tree3_withered)
    (spr environment/props/tree4_withered)
  )))

(def X (random pot candle grass empty empty))
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

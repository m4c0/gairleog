(def empty (tile (spr empty)))

(def boulder (tile
  (block)
  (random
    (spr environment/props/boulder1)
    (spr environment/props/boulder2)
    (spr environment/props/boulder3)
    (spr environment/props/boulder4)
  )))

(def candles (tile
  (light 5)
  (spr environment/props/candles)))

(def pot (tile
  (spr environment/props/pot_red)))

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

(def r23 (room 
  !!
  ..
  !!))

(def r24 
  (random
    (room .. !. .. !.)
    (room .. .! .. .!)
    (room .. !. .. .!)
    (room .. .! .. !.)

    (room .. !. .. ..)
    (room .. .! .. ..)
    (room .. .. !. ..)
    (room .. .. .! ..)))

(def r26 
  (random
    (room .. !. .. .! .. !.)
    (room .. .! .. !. .. .!)
    (room .. !. .. .! .. .!)
    (room .. .! .. !. .. !.)
                  
    (room .. !. .. !. .. ..)
    (room .. .! .. .! .. ..)
    (room .. .. !. .. !. ..)
    (room .. .. .! .. .! ..)))

(roomdefs
  (roomdef 2 2
    (random
      (room !. ..)
      (room .! ..)
      (room .. !.)
      (room .. .!)))
  
  (roomdef 2 3 (r23))
  (roomdef 2 4 (random (r23) (r24)))
  (roomdef 2 5 (random (r23)))
  (roomdef 2 6 (random (r23) (r24) (r26)))
  (roomdef 2 7 (random (r23)))
  (roomdef 2 8 (random (r23) (r24)))
  (roomdef 2 9 (random (r23)))
  (roomdef 2 10 (random (r23) (r24)))
  (roomdef 2 11 (random (r23)))
  (roomdef 2 12 (random (r23) (r24) (r26)))
  (roomdef 2 13 (random (r23)))
  (roomdef 2 14 (random (r23) (r24)))
  (roomdef 2 15 (random (r23)))
  (roomdef 2 16 (random (r23) (r24)))
)

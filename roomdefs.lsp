(def empty (tile (spr empty)))

(def boulder (tile
  (block)
  (spr environment/props/boulder1
       environment/props/boulder2
       environment/props/boulder3
       environment/props/boulder4)))

(def candles (tile
  (light 5)
  (spr environment/props/candles)))

(def pot (tile
  (spr environment/props/pot_red)))

(def grass (tile
  (spr environment/floor_tiles/grass1
       environment/floor_tiles/grass2
       environment/floor_tiles/grass3
       environment/floor_tiles/grass4)))

(def grass_swamp (tile
  (spr environment/floor_tiles/grass_swamp1
       environment/floor_tiles/grass_swamp2
       environment/floor_tiles/grass_swamp3
       environment/floor_tiles/grass_swamp4)))

(def stone (tile
  (spr environment/floor_tiles/stone1
       environment/floor_tiles/stone2
       environment/floor_tiles/stone3
       environment/floor_tiles/stone4)))

(def tree (tile
  (block)
  (spr environment/props/tree1
       environment/props/tree2
       environment/props/tree3
       environment/props/tree4)))

(def tree_swamp (tile
  (block)
  (spr environment/props/tree1_swamp
       environment/props/tree2_swamp
       environment/props/tree3_swamp
       environment/props/tree4_swamp)))

(def tree_withered (tile
  (block)
  (spr environment/props/tree1_withered
       environment/props/tree2_withered
       environment/props/tree3_withered
       environment/props/tree4_withered)))

(themedef
  (random
    (do
      (def . (random grass empty empty))
      (def ! (random tree tree tree_withered)))
    (do
      (def . (random grass_swamp grass_swamp empty))
      (def ! (random tree_swamp tree_withered)))
  ))

(room
  !!
  !!)

(room
  !!
  ..
  !!)

(room
  !!
  ..
  ..
  !!)

(room
  !!
  ..
  ..
  ..
  !!)

(room
  !!
  ..
  ..
  ..
  ..
  !!)

(room
  !!
  ..
  ..
  ..
  ..
  ..
  !!)

(room
  !!
  ..
  ..
  ..
  ..
  ..
  ..
  !!)

(room
  !.!
  ...
  !.!)

(room
  !..!
  ....
  ....
  !..!)

(room
  !...!
  .....
  !...!)

(room
  !.!
  ...
  ...
  ...
  !.!)

(room
  !...!
  .....
  .....
  .....
  !...!)

(room
  !......!
  !......!)

(room
  !.....!
  !.....!)

(room
  !....!
  !....!)

(room
  !...!
  !...!)

(room
  !..!
  !..!)

(room
  !.!
  !.!)

(room
  !....!
  ......
  !....!)

(room
  !....!
  ......
  ......
  !....!)

(room
  !....!
  ......
  ......
  ......
  !....!)

(room
  !....!
  ......
  ......
  ......
  ......
  !....!)

(room
  !.....!
  .......
  .......
  .......
  .......
  .......
  !.....!)

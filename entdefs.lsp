(entdef empty)

(entdef boulder
  (solid)
  (random
    (spr environment/props/boulder1)
    (spr environment/props/boulder2)
    (spr environment/props/boulder3)
    (spr environment/props/boulder4)
  ))

(entdef candles
  (light 1)
  (spr environment/props/candles))

(entdef pot
  (solid)
  (ceramic)
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
  ))

(entdef grass
  (random
    (spr environment/floor_tiles/grass1)
    (spr environment/floor_tiles/grass2)
    (spr environment/floor_tiles/grass3)
    (spr environment/floor_tiles/grass4)
  ))
  
(entdef grass_swamp
  (random
    (spr environment/floor_tiles/grass_swamp1)
    (spr environment/floor_tiles/grass_swamp2)
    (spr environment/floor_tiles/grass_swamp3)
    (spr environment/floor_tiles/grass_swamp4)
  ))

(entdef stone
  (random
    (spr environment/floor_tiles/stone1)
    (spr environment/floor_tiles/stone2)
    (spr environment/floor_tiles/stone3)
    (spr environment/floor_tiles/stone4)
  ))

(entdef tree
  (solid)
  (random
    (spr environment/props/tree1)
    (spr environment/props/tree2)
    (spr environment/props/tree3)
    (spr environment/props/tree4)
  ))

(entdef tree_swamp
  (solid)
  (random
    (spr environment/props/tree1_swamp)
    (spr environment/props/tree2_swamp)
    (spr environment/props/tree3_swamp)
    (spr environment/props/tree4_swamp)
  ))

(entdef tree_withered
  (solid)
  (random
    (spr environment/props/tree1_withered)
    (spr environment/props/tree2_withered)
    (spr environment/props/tree3_withered)
    (spr environment/props/tree4_withered)
  ))

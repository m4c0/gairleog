(entdef empty)

(entdef player
  (light 1)
  (life 10)
  (maxlife 10)
  (strength 1)
  (defense 0)
  (player)
  (solid)
  (spr characters/human_knight)
  (atkspr fx/fx_blue_slash)
  (sfx
    (attack player_attack)))

(entdef exit
  (light 0.3)
  (exit)
  (spr environment/props/door_closed))

(entdef apple   (food) (spr items/food/appletomato_red))
(entdef berry   (food) (spr items/food/cherriesberries_green))
(entdef chicken (food) (spr items/food/chicken_raw))
(entdef potato  (food) (spr items/food/potato_orange))
(entdef turnip  (food) (spr items/food/radishturnip_brown))

(entdef enemy
  (solid)
  (enemy)
  (strength 1)
  (defense 0)
  (loot apple)
  (atkspr fx/themed/fx_frost_bite)
  (random
    (spr characters/beast_snake)
    (spr characters/spirit_will_o_the_wisp)
  ))

(entdef boulder
  (solid)
  (ceramic)
  (life 30)
  (random
    (spr environment/props/boulder1)
    (spr environment/props/boulder2)
    (spr environment/props/boulder3)
    (spr environment/props/boulder4)
  ))

(entdef candles
  (light 0.3)
  (spr environment/props/candles))

(entdef pot
  (solid)
  (ceramic)
  (random
    (loot apple)
    (loot potato)
  )
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

(entdef tree_withered
  (solid)
  (ceramic)
  (life 30)
  (random
    (spr environment/props/tree1_withered)
    (spr environment/props/tree2_withered)
    (spr environment/props/tree3_withered)
    (spr environment/props/tree4_withered)
  ))

(entdef tree
  (solid)
  (ceramic)
  (life 5)
  (loot tree_withered)
  (random
    (spr environment/props/tree1)
    (spr environment/props/tree2)
    (spr environment/props/tree3)
    (spr environment/props/tree4)
  ))

(entdef tree_swamp
  (solid)
  (ceramic)
  (life 5)
  (loot tree_withered)
  (random
    (spr environment/props/tree1_swamp)
    (spr environment/props/tree2_swamp)
    (spr environment/props/tree3_swamp)
    (spr environment/props/tree4_swamp)
  ))


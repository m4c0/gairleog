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
    (attack player_attack)
    (block player_block)
    (miss player_miss)
    (pick player_pick)
    (poison player_poison)
    (walk player_walk)))

(entdef exit
  (light 0.3)
  (exit)
  (spr environment/props/door_closed))

(entdef apple   (food) (fluorescent) (spr items/food/appletomato_red))
(entdef berry   (food) (fluorescent) (spr items/food/cherriesberries_green))
(entdef chicken (food) (fluorescent) (spr items/food/chicken_raw))
(entdef potato  (food) (fluorescent) (spr items/food/potato_orange))
(entdef turnip  (food) (fluorescent) (spr items/food/radishturnip_brown))

; =====================================================================
; Enemies
; =====================================================================

(entdef bat
  (solid)
  (enemy)
  (strength 1)
  (defense 0)
  (loot (random apple empty))
  (atkspr fx/themed/fx_frost_bite)
  (spr characters/beast_bat))

(entdef beholder
  (solid)
  (enemy)
  (strength 1)
  (defense 0)
  (loot (random apple empty))
  (atkspr fx/themed/fx_frost_bite)
  (spr characters/beast_beholder))

(entdef demon
  (solid)
  (enemy)
  (strength 1)
  (defense 0)
  (loot (random apple empty))
  (atkspr fx/themed/fx_frost_bite)
  (spr characters/demon_knight))

(entdef dragon-drake
  (solid)
  (enemy)
  (strength 1)
  (defense 0)
  (loot (random apple empty))
  (atkspr fx/themed/fx_frost_bite)
  (spr characters/dragon_drake))

(entdef dragon-hydra
  (solid)
  (enemy)
  (strength 1)
  (defense 0)
  (loot (random apple empty))
  (atkspr fx/themed/fx_frost_bite)
  (spr characters/dragon_hydra))

(entdef dragon-wyrm
  (solid)
  (enemy)
  (strength 1)
  (defense 0)
  (loot (random apple empty))
  (atkspr fx/themed/fx_frost_bite)
  (spr characters/dragon_wyrm))

(entdef leech
  (solid)
  (enemy)
  (strength 1)
  (defense 0)
  (loot (random apple empty))
  (atkspr fx/themed/fx_frost_bite)
  (spr characters/beast_leech))

(entdef ooze
  (solid)
  (enemy)
  (strength 1)
  (defense 0)
  (loot (random apple empty))
  (atkspr fx/themed/fx_frost_bite)
  (spr characters/beast_ooze))

(entdef slime
  (solid)
  (enemy)
  (strength 1)
  (defense 0)
  (loot (random apple empty))
  (atkspr fx/themed/fx_frost_bite)
  (spr characters/beast_slime_green))

(entdef slime-big
  (solid)
  (enemy)
  (strength 1)
  (defense 0)
  (loot (random apple empty))
  (atkspr fx/themed/fx_frost_bite)
  (spr characters/beast_slime_green_large))

(entdef snake
  (solid)
  (enemy)
  (strength 1)
  (defense 0)
  (loot (random apple empty))
  (atkspr fx/themed/fx_frost_bite)
  (spr characters/beast_snake))

(entdef spirit_wotw
  (solid)
  (enemy)
  (strength 1)
  (defense 0)
  (loot (random berry empty))
  (atkspr fx/themed/fx_frost_bite)
  (spr characters/spirit_will_o_the_wisp))

(entdef spirit_faerie
  (solid)
  (enemy)
  (strength 1)
  (defense 0)
  (loot (random berry empty))
  (atkspr fx/fx_red_impact)
  (spr characters/spirit_faerie))

(entdef wolf
  (solid)
  (enemy)
  (strength 1)
  (defense 0)
  (loot (random apple empty))
  (atkspr fx/themed/fx_frost_bite)
  (spr characters/beast_wolf_brown))

; =====================================================================
; Breakables
; =====================================================================

(entdef pot-red
  (solid)
  (ceramic)
  (fluorescent)
  (random
    (loot apple)
    (loot potato)
  )
  (random
    (spr environment/props/pot_yellow)
    (spr environment/props/pot_orange)
    (spr environment/props/pot_red)
  ))

(entdef pot-blue
  (solid)
  (ceramic)
  (fluorescent)
  (random
    (loot apple)
    (loot potato)
  )
  (random
    (spr environment/props/pot_blue)
    (spr environment/props/pot_purple)
    (spr environment/props/pot_indigo)
  ))

(entdef pot-other
  (solid)
  (ceramic)
  (fluorescent)
  (random
    (loot apple)
    (loot potato)
  )
  (random
    (spr environment/props/pot_black)
    (spr environment/props/pot_brown)
    (spr environment/props/pot_gray)
  ))

; =====================================================================

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
  (solid)
  (ceramic)
  (life 3)
  (spr environment/props/candles))

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


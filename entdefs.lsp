(entdef empty)

(entdef player
  (light 1)
  (life 10)
  (strength 1)
  (defense 0)
  (player)
  (solid)
  (spr characters/human_knight)
  (atkspr fx/fx_blue_slash)
  (splatspr environment/props/splatter_red))

(entdef apple   (food) (fluorescent) (spr items/food/appletomato_red))
(entdef tomato  (food) (fluorescent) (spr items/food/appletomato_green))
(entdef bberry  (food) (fluorescent) (spr items/food/cherriesberries_blue))
(entdef gberry  (food) (fluorescent) (spr items/food/cherriesberries_green))
(entdef rberry  (food) (fluorescent) (spr items/food/cherriesberries_red))
(entdef chicken (food) (fluorescent) (spr items/food/chicken_raw))
(entdef potato  (food) (fluorescent) (spr items/food/potato_orange))
(entdef radish  (food) (fluorescent) (spr items/food/radishturnip_red))
(entdef turnip  (food) (fluorescent) (spr items/food/radishturnip_brown))

(def fruit-loot     
     (random (random apple bberry gberry rberry tomato) empty empty))
(def non-fruit-loot
     (random (random chicken potato radish turnip) empty empty))

; =====================================================================
; Must have as-is props
; =====================================================================

(entdef door
  (spr environment/props/door_closed)
  (life (level))
  (ceramic)
  (fluorescent)
  (solid))

(entdef wall
  (spr environment/walls/brick_clay)
  (fluorescent)
  (solid)
  (wall))

(entdef entry
  (solid)
  (spr environment/props/door_cage_closed))

(entdef exit
  (light 0.3)
  (exit)
  (spr environment/props/door_cage_open))

; =====================================================================
; Enemies
; =====================================================================

(entdef bat
  (solid)
  (enemy)
  (strength 1)
  (defense 0)
  (loot (fruit-loot))
  (atkspr fx/themed/fx_frost_bite)
  (splatspr environment/props/splatter_red)
  (spr characters/beast_bat))

(entdef beholder
  (solid)
  (enemy)
  (strength 3)
  (defense 3)
  (loot (fruit-loot))
  (atkspr fx/themed/fx_frost_bite)
  (splatspr environment/props/splatter_red)
  (spr characters/beast_beholder))

(entdef demon
  (solid)
  (enemy)
  (strength 5)
  (defense 4)
  (loot (fruit-loot))
  (atkspr fx/themed/fx_frost_bite)
  (splatspr environment/props/splatter_red)
  (spr characters/demon_knight))

(entdef dragon-drake
  (solid)
  (enemy)
  (poison)
  (strength 4)
  (defense 4)
  (loot (fruit-loot))
  (atkspr fx/themed/fx_frost_bite)
  (splatspr environment/props/splatter_red)
  (spr characters/dragon_drake_red))

(entdef dragon-hydra
  (solid)
  (enemy)
  (poison)
  (strength 4)
  (defense 4)
  (loot (fruit-loot))
  (atkspr fx/themed/fx_frost_bite)
  (splatspr environment/props/splatter_red)
  (spr characters/dragon_hydra_blue))

(entdef dragon-wyrm
  (solid)
  (enemy)
  (poison)
  (strength 4)
  (defense 4)
  (loot (fruit-loot))
  (atkspr fx/themed/fx_frost_bite)
  (splatspr environment/props/splatter_red)
  (spr characters/dragon_wyrm_green))

(entdef leech
  (solid)
  (enemy)
  (poison)
  (strength 2)
  (defense 3)
  (loot (fruit-loot))
  (atkspr fx/themed/fx_frost_bite)
  (splatspr environment/props/splatter_red)
  (spr characters/beast_leech))

(entdef ooze
  (solid)
  (enemy)
  (strength 3)
  (defense 3)
  (loot (fruit-loot))
  (atkspr fx/themed/fx_frost_bite)
  (spr characters/beast_ooze_blue))

(entdef slime
  (solid)
  (enemy)
  (strength 1)
  (defense 0)
  (loot (fruit-loot))
  (atkspr fx/themed/fx_frost_bite)
  (splatspr environment/props/splatter_red)
  (spr characters/beast_slime_green))

(entdef slime-big
  (solid)
  (enemy)
  (strength 2)
  (defense 2)
  (loot (fruit-loot))
  (atkspr fx/themed/fx_frost_bite)
  (splatspr environment/props/splatter_red)
  (spr characters/beast_slime_green_large))

(entdef snake
  (solid)
  (enemy)
  (poison)
  (strength 1)
  (defense 0)
  (loot (fruit-loot))
  (atkspr fx/themed/fx_frost_bite)
  (splatspr environment/props/splatter_red)
  (spr characters/beast_snake))

(entdef spirit-wotw
  (solid)
  (enemy)
  (strength 3)
  (defense 2)
  (loot (fruit-loot))
  (atkspr fx/themed/fx_frost_bite)
  (splatspr environment/props/splatter_red)
  (spr characters/spirit_will_o_the_wisp))

(entdef spirit-faerie
  (solid)
  (enemy)
  (strength 3)
  (defense 2)
  (loot (fruit-loot))
  (atkspr fx/fx_red_impact)
  (splatspr environment/props/splatter_red)
  (spr characters/spirit_faerie))

(entdef wolf
  (solid)
  (enemy)
  (strength 1)
  (defense 0)
  (loot (fruit-loot))
  (atkspr fx/themed/fx_frost_bite)
  (splatspr environment/props/splatter_red)
  (spr characters/beast_wolf_brown))

; =====================================================================
; Breakables
; =====================================================================

(entdef pot-red
  (solid)
  (ceramic)
  (fluorescent)
  (loot (non-fruit-loot))
  (random
    (spr environment/props/pot_yellow)
    (spr environment/props/pot_orange)
    (spr environment/props/pot_red)
  ))

(entdef pot-blue
  (solid)
  (ceramic)
  (fluorescent)
  (loot (non-fruit-loot))
  (random
    (spr environment/props/pot_blue)
    (spr environment/props/pot_purple)
    (spr environment/props/pot_indigo)
  ))

(entdef pot-other
  (solid)
  (ceramic)
  (fluorescent)
  (loot (non-fruit-loot))
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

(entdef lilypad
  (random
    (spr environment/props/lilypad1)
    (spr environment/props/lilypad2)
    (spr environment/props/lilypad3)
    (spr environment/props/lilypad4)
  ))

(entdef mannequin
  (solid)
  (ceramic)
  (life 3)
  (spr environment/props/mannequin))

(entdef mushroom
  (solid)
  (ceramic)
  (life 5)
  (random
    (spr environment/props/mushroom1)
    (spr environment/props/mushroom2)
    (spr environment/props/mushroom3)
    (spr environment/props/mushroom4)
  ))

(entdef pillar
  (solid)
  (ceramic)
  (life 10)
  (spr environment/props/pillar))

(entdef spiderweb (spr environment/props/spiderweb))

(entdef statue
  (solid)
  (ceramic)
  (life 10)
  (spr environment/props/statue))

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


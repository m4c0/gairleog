(sfxdef 02_human_atk_sword_2.wav)
(sfxdef 20_human_walk_stone_1.wav)

(def player_attack  02_human_atk_sword_2.wav)
(def player_block   (player_walk))
(def player_miss    (player_walk))
(def player_pick    "")
(def player_poison  "")
(def player_walk    20_human_walk_stone_1.wav)
(def mute           "") ; Any invalid file suffices
(def menu_selection (player_walk))
(def menu_click     (player_attack))

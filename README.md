# gairleog - Version 1.0.0-beta

Me having fun making an dungeon-crawler in C++.

Using assets from these packs:

* [PixeLike II Asset Pack](https://fartfish.itch.io/pixelike-ii).
* [DUNGEON.mode](https://datagoblin.itch.io/dungeonmode)
* [Minifantasy SFX Pack](https://leohpaz.itch.io/minifantasy-creatures-sfx-pack).

## Change log

### Version 1.0.0-beta

Work in progress.

Known issues:

* Damage does not show on console (TODO)
* Most actions have no associated sound (TODO)
* Web: Audio does not sync properly on certain browsers (won't fix)
* Windows: Audio occasionally fails (TODO)
* Windows: Room editor cannot save rooms (TODO)
* Errors in resource files are only found when they are accessed. This means
  certain failures might only appear late in the game.

Where:

* "won't fix" = will not fix in this version
* "todo" = will be fixed before release of this version
* no tags = no defined priority

### Version 1.0.0-alpha

Released on [itch.io](https://m4c0.itch.io/gairleog)

## Version scheme

Version scheme is inspired by "Semantic Versioning" but with a twist. Three
numbers - separated by dots - and a release state.

Example: 1.2.3-beta - major version 1, minor version 2, revision 3, state "beta".

Convention used:

* Major version: Only changes when major changes happens (example: replacing an
  engine, major rewrites, etc). Different installments of the game will have
  different versions. So, "Gairleog 2" is not the same as "Gairleog 1 - Version
  2.x.y".
* Minor version: Represents any minor change, like new entities that affect the
  game balance, etc. Expect these to contain a number of new features in their
  change logs, or rebalancing, etc.
* Revision: Bug fixes, mostly. 
* Release states: alpha (still exploring the idea), beta (game is well-defined),
  rc (release candidate), or none (released)

From a players perspective:

* Change in major version: uncharted territory
* Change in minor version: cool stuff to check out
* Change in revision: only if you are curious

* Alpha: only play if you want to give feedback
* Beta: expect bugs, missing assets, etc, but the core game is there
* RC: should be reasonably stable, with minor bugs and maybe some rebalancing
  between releases
* Release: "final". Any change will increase the version number


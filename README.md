# **READ THE ORIGINAL README.MD!!!**
+ [Original README](https://github.com/Rubberduckycooly/Sonic-Mania-Decompilation#readme)

# **Sonic Mania Plus: Extra Slot Amy**
### Welcome!
You may be familiar with the Sonic 3 A.I.R. "Extra Slot" mods, and if so, you should know exactly what this is!
But in case you don't, here's the jist of what this mod is, in one sentence:
*Amy as an entirely new character, as opposed to a character _replacement_.*

## Disclaimers/Notices
 * CHECK THE AMY MANIA GAMEBANANA PAGE FOR THE MOD ASSETS!!!
   * [Gamebanana page](https://gamebanana.com/wips/50006)
 * All art assets used within this mod are made by Troopsushi, NOT ME. I have full permission from Troopsushi to release this project.
 * This build is unfinished. I'm releasing this now due to sudden, debilitating health issues, and because of it, I am unsure about my ability to continue this project.
 * Amy is locked behind the Encore DLC, and as such, requires Plus to use. I am not changing this.
 * This project was tested on a 32-bit RSDKv5U.exe and a 64-bit Game.dll built with the Release and RSDKv5U - All configurations.
 
## Extra Slot Amy Additions
### Amy as a new character
  * Amy is a newly-added character, and doesn't replace anyone else, meaning the game now has six characters to choose from!
  * Amy has a unique palette and a unique Super palette. Do NOT try to open this modified 36D61A3DE508B500AE0ECF8D33F32943.bin in ManiaPal, as it can't properly read this modified file.
  * Amy collects Time Stones instead of Chaos Emeralds. This is purely cosmetic, meaning Amy can still turn Super (referred to as "Miracle" in-game) and obtains the Time Stones in the way you'd obtain Chaos Emeralds. Putting Amy in Egg Reverie Zone replaces the Chaos Emeralds with Time Stones as well.
  
### Amy's Moveset
  * Amy has her moveset from Troopsushi's Amy Mania mod, although the moves work a bit differently here.
    * Heli-Hammer: Pressing A or C while in the air activates Amy's Heli-Hammer move, where she will spin her hammer around her and can damage badniks and bosses. Falling on enemies or item boxes will give you a vertical boost, and the more falling momentum you have before pulling off the move, the higher the vertical boost will be! This move, however, does not work on bosses, and hitting them with this move will put you in a vulnerable falling state.
    * Tall Jump: Holding Up and pressing either A or C while at a stand-still will give Amy a much higher jump than other characters, but at the cost of damage vulnerability. No spin-jump protection here, be careful!
    * Hammer Whack: Pressing B while standing, walking, running, or dashing will make Amy hit whatever is in front of her with her hammer. Hammering conserves all momentum, so hammering won't slow you down at all. Works on badniks, item monitors, and bosses.
    * Za Waru- I mean, Time Stop: When in Amy's Miracle form, press the Z key when jumping to stop the timer. I hadn't got to freezing enemies or ring/item box animations yet, but given you can get 50 rings fast enough, that time bonus could be huge. This comes with the trade-off of losing the Miracle form, so you have to play carefully to get the biggest score bonuses you can! (This ability was originally going to be Mania/Encore layout swapping, (Mania Mode takes place in the present and Encore Mode takes place in the future, so to me this made sense) but I couldn't code everything I wanted to include with that ability, so I opted for this instead)
   
### Vanilla changes
 * Amy is the starting character in Encore Mode, to have a larger focus on the (now three) new Plus characters. Sonic, Tails, and Knuckles were the focus of Mania Mode, so it made sense to me to have the three other characters be the focus of Encore Mode.
 * "& Knuckles" Mode is replaced with "Amy Assist", allowing you to have Amy as Player 2 in Mania Mode. (yes, this does mean Amy & Amy is a thing now)
 
## Known Bugs
  * LOADING A SAVEDATA.BIN WITH AN AMY SLOT OUTSIDE OF EXTRA SLOT AMY WILL CRASH THE GAME UPON ENTERING MANIA MODE. BACKUP YOUR ORIGINAL SAVEDATA.BIN!!!
  * Amy uses Sonic's ending cinematic cutscenes. This could be easily fixed with a bit of coding.
  * Titanic Monarch Encore's escape sequence is bugged. Knuckles and Amy do not show up. Fix is unknown as of right now.
  * Amy does not have underwater Super palettes. This could be easily fixed with a bit of coding.
  * Encore Mode does not support having all six characters. Fix is unknown as of right now, so just choose your characters carefully when playing Encore.
  * Trying to rescue character number six from the Pinball bonus stage when your reserves are full will add an invalid character ID to reserve slot 2. The fix, and quite frankly, the cause of this, are unknown, so try to avoid the Pinball bonus stage if you can.
  
## Installation For Windows
To install this mod, you need to build this project into a Game.dll. Refer to [this tutorial](https://gamebanana.com/tuts/15254) for help with setting up dependencies for building Game.dlls and whatnot.
After building, put this in the same directory as your RSDKv5U.exe and make sure it is named Game.dll. Also make sure to enable required assets mod, (linked above) or else the mod will not function properly.
More in-depth installation instructions can be found [here](https://gamebanana.com/wips/50006).

## Installation for other platforms (Linux, Switch, etc.)

Make sure all packages are installed for your specific platform here:

### Linux
- **pacman (Arch):** `sudo pacman -S base-devel cmake glew glfw libtheora`
- **apt (Debian/Ubuntu):** `sudo apt install build-essential cmake libglew-dev libglfw3-dev libtheora-dev`
- **rpm (Fedora):** `sudo dnf install make cmake gcc glew-devel glfw-devel libtheora-devel zlib-devel`
- **xbps (Void):** `sudo xbps-install make cmake gcc pkg-config glew-devel glfw-devel libtheora-devel zlib-devel`

#### (make sure to [install GL shaders!](#q-why-arent-videosfilters-working-while-using-gl))

### Switch
[Setup devKitPro](https://devkitpro.org/wiki/Getting_Started), then run the following:
- `(dkp-)pacman -Syuu switch-dev switch-libogg switch-libtheora switch-sdl2 switch-glad`

Finally, follow the [compilation steps below](###compiling) using `-DCMAKE_TOOLCHAIN_FILE=/opt/devkitpro/cmake/Switch.cmake` as arguments for `cmake -B build`.

#### (make sure to [install GL shaders!](#q-why-arent-videosfilters-working-while-using-gl))

### Compiling/Building the mod

Run these at the root of the repo:

```
cmake -B build
cmake --build build --config release
```

You will see your respective Game.dll/libGame.so in your build/ folder.


## Installation for WASM/Emscripten

Alright just install [Emscripten](https://emscripten.org/docs/getting_started/downloads.html) along with the dependencies for your platform: 

### Windows
Install [CMake](https://cmake.org/download/)

### Linux
- **pacman (Arch):** `sudo pacman -S base-devel cmake glew glfw libtheora`
- **apt (Debian/Ubuntu):** `sudo apt install build-essential cmake libglew-dev libglfw3-dev libtheora-dev`
- **rpm (Fedora):** `sudo dnf install make cmake gcc glew-devel glfw-devel libtheora-devel zlib-devel`
- **xbps (Void):** `sudo xbps-install make cmake gcc pkg-config glew-devel glfw-devel libtheora-devel zlib-devel`

### Compiling/Building the mod

Run these at the root of the repo:

```
emcmake cmake -B build
cmake --build build
```

You should get a Game.wasm in your build/ folder.
This is made to work with this repo in mind: https://github.com/ant9012/RSDKv5-Decompilation-wasm/
Simply upload them via your custom launcher or the [RSDK-Library Engine Manager](https://github.com/Jdaslepre/RSDK)

(**MAKE SURE YOU HAVE THE FILES MODULE WORKING IF YOU'RE USING A CUSTOM LAUNCHER: https://github.com/Jdaslepre/RSDK-Library-FilesModule**)



### Sonic Mania flags
- `MANIA_FIRST_RELEASE`: Whether or not to build the first console release of Sonic Mania. Takes a boolean, defaults to `off`.
- `MANIA_PRE_PLUS`: Whether or not to build a pre-plus version of Sonic Mania. Takes a boolean, defaults to `off`.
- `GAME_INCLUDE_EDITOR`: Whether or not to include functions for use in certain RSDKv5 scene editors. Takes a boolean, defaults to `on`.
- `GAME_VERSION`: Which release version of Sonic Mania to target for. Takes an integer, defaults to `3` when `MANIA_PRE_PLUS` is enabled, and `6` otherwise (last steam release).


  
## Final Words
 * Huge thanks to RDC and Chuli for decompiling Mania in the first place, it's the only thing that makes this mod possible!
 * A gigantic shout-out to the folks over at REMS (Retro Engine Modding Server) for supporting this project over its roughly 20-day development cycle. (you should join [REMS](https://dc.railgun.works/retroengine), it's a pretty cool Discord server)
 * Big thanks to TroopSushi for allowing me to use all of their Amy sprites for this project. (gigachad move right there tbh)
 * Additional thanks to [CodenameGamma](https://www.patreon.com/CnGMods) for giving me the original Amy Mania code to reference when re-making Amy's moveset for the decomp.
 * Special thanks to my brother, bob.bmp, for originally inspiring me to make this mod, with his [Sonic 1 Score Attack](https://github.com/Gareth-McCullough/sth-score-attack) and [Sonic 1 w/ Super Sonic](https://github.com/Gareth-McCullough/sth-super-score-attack) mods for the original Genesis version.

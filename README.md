# Advanced Programming in C

![Dungeon Crawler Image](https://dev.maxwelltalley.com/assets/images/dungeonCrawler.png)

In this class, you create a roguelike game, similar to command line games like NetHack.

0-abelian-sandpile is the only program that is separate.

**To run the Abelian Sandpile program**
1. Pull the repo
1. Navigate to 0-abelian-sandpile
1. Type `make` on the command line
1. Run the compiled binary with `./sandpile`

# Roguelike Game

Each assignment beginning with a 1 is the roguelike game. Every assignment adds further functionality to the gameplay.

### 1.01-dungeon-generation
Generates an empty, fully connected dungeon. Each space in the dungeon has an assigned hardness for future use.

### 1.02-dungeon-save-load
Adds the ability to save and load to a folder in the home directory. This will allow for the continuation of gameplay in the future.

### 1.03-path-finding
Adds functionality for mapping distances from every point in the map to the player character(pc). This is for next week when we add monsters, so they can properly find the pc.

### 1.04-monster-movement
Adds functionality for monster movement and random pc movement. This is the only assignment that doesn't contain full functionality.

### 1.05-NCURSES
Converts the printing of the dungeon to ncurses, rather than just printing with 'printf' statements. Also adds in controlled pc movement. It also adds functionality to show all the monsters on the map and their positions relative to the pc, via a list.

| Action       | Key        |
| ------------ | ---------: |
| Up + Left    | 7 or y     |
| Up           | 8 or k     |
| Up + Right   | 9 or u     |
| Right        | 6 or l     |
| Down + Right | 3 or n     |
| Down         | 2 or j     |
| Down + Left  | 1 or b     |
| Left         | 4 or h     |
| Down Stairs  | >          |
| Up Stairs    | <          |
| Skip Turn    | 5 or ' '   |
| Monster List | m          |
| Scroll up    | up arrow   |
| Scroll down  | down arrow |
| Back to move | escape     |
| Quit game    | Q          |

### 1.06-fog-of-war
Adds functionality for fog of war, so only the 5x5 grid around the pc is visible at any time. The pc remembers previous seen terrain, but if it changes while it is out of the grid then the pc will not observe it. Only monsters that are visible within the 5xt grid show up. The pc does not remember monsters. You can toggle fog of war. There is also the added function of teleportation. 

| Action                    | Key           |
| ------------------------- | ------------: |
| Toggle fog of war         | f             | 
| Turn on teleport          | t             |
| Random teleport           | t + r         |
| Select teleport location  | movement keys |
| Confirm teleport location | t             |

## To compile a particular assignment:
1. Pull the repo
1. Navigate to the assignment that you want to run
1. Type `make` on the command line

### To run assignments 1.01 - 1.04:
1. Run the compiled binary with `./dungeon`

### To run assignmnets 1.05 - :
1. Run the compiled binary with './rlg327'

### Optional flags when running the program
These may be combined in any order or combination
* `--save`
    * Saves the generated dungeon to a folder in the home directory (`HOME/.rlg327`)
    * Does not print the dungeon
* `--load`
    * Loads the saved file from the home directory (`HOME/.rlg327`)
    * Prints the generated dungeon
* `--load "custom_file_name"`
    * Loads the custom file from the home directory (`HOME/.rlg327`)
    * Must be saved within the directory specified above
* `--p x y`
    * Specifies the span location of the player character (pc)
    * Where x and y are positions on the map and (x < 80) && (y < 21)
* `--nummon i`
	* Specifies the number of monsters to spawn in the dungeon
	* Where i is the number of monsters

Example:
* `./dungeon --save --load`
    * Loads the saved file and then saves it again
* `./dungeon --p 10 10`
    * Prints the pc at location x = 10, y = 10 on the map
    * Will print here even if it isn't a room (for testing)

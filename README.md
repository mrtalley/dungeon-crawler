# CS327
## Advanced Programming in C

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

## To run a roguelike-game assignment:
1. Pull the repo
1. Navigate to the assignment that you want to run
1. Type `make` on the command line
1. Run the compiled binary with `./dungeon`

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

Example:
* `./dungeon --save --load`
    * Loads the saved file and then saves it again
* `./dungeon --p 10 10`
    * Prints the pc at location x = 10, y = 10 on the map
    * Will print here even if it isn't a room (for testing)

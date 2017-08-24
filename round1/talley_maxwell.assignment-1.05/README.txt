README 1.05

PRE: to begin the game, choose either control or look mode.

To get the character to move, I added all relevant ncurses functions to the
pc_next_pos function. It takes in a character from the user and checks if its a valid
character. If not, it asks for input again. The character can move using all
specified keys, plus wasd for simplicity. Relevant code relating to going up and
down stairs is also included in this function.

To get the staircases to print in the dungeon, I added two new enums called
ter_floor_stair_up and ter_floor_stair_down. I also created a new function called
create_stairs. A room has a 1 in 5 chance of containing a staircase and a 50% chance
of it either being up or down. Each staircase is placed in or near the middle of the room
for simplicity. Render dungeon was altered to use ncurses prints (mvaddch) instead
of putchars. The characters '>' and '<' were added here.

The 80x24 window works well now. It stays centered around the PC the entire
time. When it approaches an edge, more spaces are printed where the array is
out of bounds. This is in order to prevent any seg faults. 

When playing the game, you can engage look mode by pressing 'L'. You can move
the camera around in a way similar to how you move the pc, though now you are
not restricted by the dungeon. You can exit look mode by pressing escape.

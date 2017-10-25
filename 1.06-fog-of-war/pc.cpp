#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>

#include "string.h"

#include "dungeon.h"
#include "pc.h"
#include "utils.h"
#include "move.h"
#include "path.h"
#include "npc.h"

void pc_delete(pc_t *pc)
{
    if (pc) {
        free(pc);
    }
}

uint32_t pc_is_alive(dungeon_t *d)
{
    return d->pc.alive;
}

void place_pc(dungeon_t *d)
{
    d->pc.position[dim_y] = rand_range(d->rooms->position[dim_y],
                                       (d->rooms->position[dim_y] +
                                        d->rooms->size[dim_y] - 1));
    d->pc.position[dim_x] = rand_range(d->rooms->position[dim_x],
                                       (d->rooms->position[dim_x] +
                                        d->rooms->size[dim_x] - 1));
}

void config_pc(dungeon_t *d)
{
    memset(&d->pc, 0, sizeof (d->pc));
    d->pc.symbol = '@';

    place_pc(d);

    d->pc.speed = PC_SPEED;
    d->pc.alive = 1;
    d->pc.sequence_number = 0;
    d->pc.pc = (pc_t *) calloc(1, sizeof (*d->pc.pc));
    d->pc.npc = NULL;
    d->pc.kills[kill_direct] = d->pc.kills[kill_avenged] = 0;

    d->character[d->pc.position[dim_y]][d->pc.position[dim_x]] = &d->pc;

    dijkstra(d);
    dijkstra_tunnel(d);
}

void take_stairs(dungeon_t *d)
{
    if(mappair(d->pc.position) == ter_stairs_down || mappair(d->pc.position) == ter_stairs_up) {
        delete_dungeon(d);
        init_dungeon(d);
        gen_dungeon(d);
        config_pc(d);
        gen_monsters(d);
        render_dungeon(d);
    }
}

void teleport_pc(dungeon_t *d)
{
    int key = 0;
    d->target.symbol = '*';
    int target_x = d->pc.position[dim_x], target_y = d->pc.position[dim_y];
    srand(time(NULL));

    do {
        clear();
        d->mode = 1;
        render_dungeon(d);
        mvprintw(0, 0, "Teleport Mode - x: %d, y: %d -- PC - x: %d, y: %d", target_x, target_y, d->pc.position[dim_x], d->pc.position[dim_y]);
        refresh();
        key = getch();
        
        if(key == 'r') {
            // set teleport target to random room location
            do {
                target_y = rand() % (DUNGEON_Y - 2) + 2;
                target_x = rand() % (DUNGEON_X - 1) + 1;
            } while(!in_room(d, target_y, target_x) && !charxy(target_x, target_y));
            key = 't';
        }
        
        // Up + Left
        if(key == '7' || key == 'y') {
            target_y--;
            target_x--;
        }

        // Up
        else if(key == '8' || key == 'k') {
            target_y--;
        }

        // Up + Right
        else if(key == '9' || key == 'u') {
            target_y--;
            target_x++;
        }

        // Right
        else if(key == '6' || key == 'l') {
            target_x++;
        }

        // Down + Right
        else if(key == '3' || key == 'n') {
            target_y++;
            target_x++;
        }

        // Down
        else if(key == '2' || key == 'j') {
            target_y++;
        }

        // Down + Left
        else if(key == '1' || key == 'b') {
            target_y++;
            target_x--;
        }

        // Left
        else if(key == '4' || key == 'h') {
            target_x--;
        }
            
        if(target_x <= 0) {
            target_x++;
        } else if(target_x >= DUNGEON_X) {
            target_x--;
        }
        
        if(target_y <= 0) {
            target_y++;
        } else if(target_y >= DUNGEON_Y) {
            target_y--;
        }
        
        d->target.position[dim_x] = target_x;
        d->target.position[dim_y] = target_y;
        render_dungeon(d);
        
    } while(key != 't');
    
    // send pc to targeted location
    // call update_visible_map function on new location
    if((target_x > 0 && target_x < DUNGEON_X) && (target_y > 0 && target_y < DUNGEON_Y)
        && !charxy(target_x, target_y)) {
        d->character[d->pc.position[dim_y]][d->pc.position[dim_x]] = NULL;
        d->pc.position[dim_y] = target_y;
        d->pc.position[dim_x] = target_x;
        d->character[d->pc.position[dim_y]][d->pc.position[dim_x]] = &d->pc;
    }
    d->target.position[dim_x] = -2;
    d->target.position[dim_y] = -2;
    d->mode = 0;
    update_visible_map(d);
    render_dungeon(d);
}

uint32_t pc_next_pos(dungeon_t *d, pair_t dir)
{
    const char *valid_keys = "7y8k9u6l3n2j1b4h5 Q";
    int key;
    static int mode = MOVE;
    static int offset = 0;

    dir[dim_y] = dir[dim_x] = 0;

    /* First, eat anybody standing next to us. */
    if (charxy(d->pc.position[dim_x] - 1, d->pc.position[dim_y] - 1)) {
        dir[dim_y] = -1;
        dir[dim_x] = -1;
    } else if (charxy(d->pc.position[dim_x], d->pc.position[dim_y] - 1)) {
        dir[dim_y] = -1;
    } else if (charxy(d->pc.position[dim_x] + 1, d->pc.position[dim_y] - 1)) {
        dir[dim_y] = -1;
        dir[dim_x] = 1;
    } else if (charxy(d->pc.position[dim_x] - 1, d->pc.position[dim_y])) {
        dir[dim_x] = -1;
    } else if (charxy(d->pc.position[dim_x] + 1, d->pc.position[dim_y])) {
        dir[dim_x] = 1;
    } else if (charxy(d->pc.position[dim_x] - 1, d->pc.position[dim_y] + 1)) {
        dir[dim_y] = 1;
        dir[dim_x] = -1;
    } else if (charxy(d->pc.position[dim_x], d->pc.position[dim_y] + 1)) {
        dir[dim_y] = 1;
    } else if (charxy(d->pc.position[dim_x] + 1, d->pc.position[dim_y] + 1)) {
        dir[dim_y] = 1;
        dir[dim_x] = 1;
    } else {
        do {
            mvprintw(0, 0, "                           "); refresh();
            mvprintw(0, 0, "MODE: %s", (mode == MOVE) ? "MOVE" : "VIEW MONSTERS"); refresh();
            
            key = getch();

            // Attempt to take stairs
            if(key == '>' || key == '<') {
                take_stairs(d);
            }
            
            if(key == 'f') {
                d->mode = !d->mode;
                render_dungeon(d);
            }
            
            if(key == 't') {
                teleport_pc(d);
            }

            if(key == 'm' || mode == VIEW_MONSTERS) {
                mode = VIEW_MONSTERS;
                generate_monster_list(d);
                clear();
    
                mvprintw(0, 0, "MODE: %s, NUM_MONSTERS: %d", (mode == MOVE) ? "MOVE" : "VIEW MONSTERS", d->num_monsters); refresh();
                
                if(key == KEY_UP && offset > 0) {
                    offset--;
                }
                
                // Scroll down, if possible
                if(key == KEY_DOWN && offset < d->num_monsters - DUNGEON_Y) {
                    offset++;
                }
                
                print_monster_list(d, offset);
                
                // Switch back to dungeon view
                if(key == KEY_ESC) {
                    mode = MOVE;
                    clear();
                    render_dungeon(d);
                }
                
                // Quit
                if (key == 'Q') {
                    d->pc.alive = 0;
                    break;
                }
            }
        } while(!strchr(valid_keys, key) || mode == VIEW_MONSTERS);

        if(mode == MOVE) {
            // Up + Left
            if(key == '7' || key == 'y') {
                dir[dim_y]--;
                dir[dim_x]--;
            }

            // Up
            else if(key == '8' || key == 'k') {
                dir[dim_y]--;
            }

            // Up + Right
            else if(key == '9' || key == 'u') {
                dir[dim_y]--;
                dir[dim_x]++;
            }

            // Right
            else if(key == '6' || key == 'l') {
                dir[dim_x]++;
            }

            // Down + Right
            else if(key == '3' || key == 'n') {
                dir[dim_y]++;
                dir[dim_x]++;
            }

            // Down
            else if(key == '2' || key == 'j') {
                dir[dim_y]++;
            }

            // Down + Left
            else if(key == '1' || key == 'b') {
                dir[dim_y]++;
                dir[dim_x]--;
            }

            // Left
            else if(key == '4' || key == 'h') {
                dir[dim_x]--;
            }
            
            // Quit
            else if(key == 'Q') {
                d->pc.alive = 0;
            }
        }
    }

    return 0;
}

uint32_t pc_in_room(dungeon_t *d, uint32_t room)
{
    if ((room < d->num_rooms)                                     &&
        (d->pc.position[dim_x] >= d->rooms[room].position[dim_x]) &&
        (d->pc.position[dim_x] < (d->rooms[room].position[dim_x] +
                                  d->rooms[room].size[dim_x]))    &&
        (d->pc.position[dim_y] >= d->rooms[room].position[dim_y]) &&
        (d->pc.position[dim_y] < (d->rooms[room].position[dim_y] +
                                  d->rooms[room].size[dim_y]))) {
        return 1;
    }

    return 0;
}

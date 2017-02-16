#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "dungeon.h"
#include "path.h"

void usage(char *name)
{
  fprintf(stderr,
          "Usage: %s [-r|--rand <seed>] [-l|--load [<file>]]\n"
          "          [-s|--save [<file>]] [-i|--image <pgm file>]\n",
          name);

  exit(-1);
}

int main(int argc, char *argv[])
{
  dungeon_t d;
  time_t seed;
  struct timeval tv;
  uint32_t i;
  uint32_t do_load, do_save, do_seed, do_image, do_place_pc;
  uint32_t long_arg;
  char *save_file;
  char *load_file;
  char *pgm_file;

  /* Default behavior: Seed with the time, generate a new dungeon, *
   * and don't write to disk.                                      */
  do_load = do_save = do_image = do_place_pc = 0;
  do_seed = 1;
  save_file = load_file = NULL;

  /* The project spec requires '--load' and '--save'.  It's common  *
   * to have short and long forms of most switches (assuming you    *
   * don't run out of letters).  For now, we've got plenty.  Long   *
   * forms use whole words and take two dashes.  Short forms use an *
    * abbreviation after a single dash.  We'll add '--rand' (to     *
   * specify a random seed), which will take an argument of it's    *
   * own, and we'll add short forms for all three commands, '-l',   *
   * '-s', and '-r', respectively.  We're also going to allow an    *
   * optional argument to load to allow us to load non-default save *
   * files.  No means to save to non-default locations, however.    *
   * And the final switch, '--image', allows me to create a dungeon *
   * from a PGM image, so that I was able to create those more      *
   * interesting test dungeons for you.                             */
 
 if (argc > 1) {
    for (i = 1, long_arg = 0; i < argc; i++, long_arg = 0) {
      if (argv[i][0] == '-') { /* All switches start with a dash */
        if (argv[i][1] == '-') {
          argv[i]++;    /* Make the argument have a single dash so we can */
          long_arg = 1; /* handle long and short args at the same place.  */
        }
        switch (argv[i][1]) {
        case 'r':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-rand")) ||
              argc < ++i + 1 /* No more arguments */ ||
              !sscanf(argv[i], "%lu", &seed) /* Argument is not an integer */) {
            usage(argv[0]);
          }
          do_seed = 0;
          break;
        case 'l':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-load"))) {
            usage(argv[0]);
          }
          do_load = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            load_file = argv[++i];
          }
          break;
        case 's':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-save"))) {
            usage(argv[0]);
          }
          do_save = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            save_file = argv[++i];
          }
          break;
        case 'i':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-image"))) {
            usage(argv[0]);
          }
          do_image = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            pgm_file = argv[++i];
          }
          break;
        case 'p':
          if ((!long_arg && argv[i][2])            ||
              (long_arg && strcmp(argv[i], "-pc")) ||
              argc <= i + 2                        ||
              argv[i + 1][0] == '-'                ||
              argv[i + 2][0] == '-') {
            usage(argv[0]);
          }
          do_place_pc = 1;
          if ((d.pc.position[dim_y] = atoi(argv[++i])) < 1 ||
              d.pc.position[dim_y] > DUNGEON_Y - 2         ||
              (d.pc.position[dim_x] = atoi(argv[++i])) < 1 ||
              d.pc.position[dim_x] > DUNGEON_X - 2) {
            fprintf(stderr, "Invalid PC position.\n");
            usage(argv[0]);
          }
          break;
        default:
          usage(argv[0]);
        }
      } else { /* No dash */
        usage(argv[0]);
      }
    }
  }

  if (do_seed) {
    /* Allows me to generate more than one dungeon *
     * per second, as opposed to time().           */
    gettimeofday(&tv, NULL);
    seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
  }

  printf("Seed is %ld.\n", seed);
  srand(seed);

  init_dungeon(&d);

  if (do_load) {
    read_dungeon(&d, load_file);
  } else if (do_image) {
    read_pgm(&d, pgm_file);
  } else {
    gen_dungeon(&d);
  }

  if (!do_place_pc) {
    i = rand() % d.num_rooms;
    d.pc.position[dim_x] = (d.rooms[i].position[dim_x] +
                            (rand() % d.rooms[i].size[dim_x]));
    d.pc.position[dim_y] = (d.rooms[i].position[dim_y] +
                            (rand() % d.rooms[i].size[dim_y]));
  }

  printf("PC is at (y, x): %d, %d\n",
         d.pc.position[dim_y], d.pc.position[dim_x]);

  render_dungeon(&d);

  dijkstra(&d);
  dijkstra_tunnel(&d);
  render_distance_map(&d);
  render_tunnel_distance_map(&d);

  if (do_save) {
    write_dungeon(&d, save_file);
  }

  delete_dungeon(&d);

  return 0;
}

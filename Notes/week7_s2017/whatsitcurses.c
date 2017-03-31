#include <ncurses.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int i;
  char a[4] = "\\|/-";

  initscr();

  for (i = 0; i < 320; i++) {
    usleep(125000);
    mvaddch(23, i / 4, a[i % 4]);
    refresh();
  }

  endwin();

  return 0;
}

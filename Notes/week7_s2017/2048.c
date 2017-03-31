#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>

typedef struct game_state {
  int32_t tableau[4][4];
  uint32_t score;
} game_state_t;

game_state_t game[32768];
uint32_t game_index;

chtype board[17][41];

void push_undo_state()
{
  memcpy(&game[game_index + 1], &game[game_index], sizeof (*game));

  game_index++;
}

void pop_undo_state()
{
  if (game_index > 1) {
    game_index--;
  }
}

void init_board()
{
  /* Can't initialize statically because ACS_* are not constants.  Could *
   * statically initialize with spaces and leave those out of here, also *
   * could calculate positional values, but this is clean, if big.       */

  board[ 0][ 0] = ACS_ULCORNER;
  board[ 0][ 1] = ACS_HLINE;
  board[ 0][ 2] = ACS_HLINE;
  board[ 0][ 3] = ACS_HLINE;
  board[ 0][ 4] = ACS_HLINE;
  board[ 0][ 5] = ACS_HLINE;
  board[ 0][ 6] = ACS_HLINE;
  board[ 0][ 7] = ACS_HLINE;
  board[ 0][ 8] = ACS_HLINE;
  board[ 0][ 9] = ACS_HLINE;
  board[ 0][10] = ACS_TTEE;
  board[ 0][11] = ACS_HLINE;
  board[ 0][12] = ACS_HLINE;
  board[ 0][13] = ACS_HLINE;
  board[ 0][14] = ACS_HLINE;
  board[ 0][15] = ACS_HLINE;
  board[ 0][16] = ACS_HLINE;
  board[ 0][17] = ACS_HLINE;
  board[ 0][18] = ACS_HLINE;
  board[ 0][19] = ACS_HLINE;
  board[ 0][20] = ACS_TTEE;
  board[ 0][21] = ACS_HLINE;
  board[ 0][22] = ACS_HLINE;
  board[ 0][23] = ACS_HLINE;
  board[ 0][24] = ACS_HLINE;
  board[ 0][25] = ACS_HLINE;
  board[ 0][26] = ACS_HLINE;
  board[ 0][27] = ACS_HLINE;
  board[ 0][28] = ACS_HLINE;
  board[ 0][29] = ACS_HLINE;
  board[ 0][30] = ACS_TTEE;
  board[ 0][31] = ACS_HLINE;
  board[ 0][32] = ACS_HLINE;
  board[ 0][33] = ACS_HLINE;
  board[ 0][34] = ACS_HLINE;
  board[ 0][35] = ACS_HLINE;
  board[ 0][36] = ACS_HLINE;
  board[ 0][37] = ACS_HLINE;
  board[ 0][38] = ACS_HLINE;
  board[ 0][39] = ACS_HLINE;
  board[ 0][40] = ACS_URCORNER;
  board[ 1][ 0] = ACS_VLINE;
  board[ 1][ 1] = ' ';
  board[ 1][ 2] = ' ';
  board[ 1][ 3] = ' ';
  board[ 1][ 4] = ' ';
  board[ 1][ 5] = ' ';
  board[ 1][ 6] = ' ';
  board[ 1][ 7] = ' ';
  board[ 1][ 8] = ' ';
  board[ 1][ 9] = ' ';
  board[ 1][10] = ACS_VLINE;
  board[ 1][11] = ' ';
  board[ 1][12] = ' ';
  board[ 1][13] = ' ';
  board[ 1][14] = ' ';
  board[ 1][15] = ' ';
  board[ 1][16] = ' ';
  board[ 1][17] = ' ';
  board[ 1][18] = ' ';
  board[ 1][19] = ' ';
  board[ 1][20] = ACS_VLINE;
  board[ 1][21] = ' ';
  board[ 1][22] = ' ';
  board[ 1][23] = ' ';
  board[ 1][24] = ' ';
  board[ 1][25] = ' ';
  board[ 1][26] = ' ';
  board[ 1][27] = ' ';
  board[ 1][28] = ' ';
  board[ 1][29] = ' ';
  board[ 1][30] = ACS_VLINE;
  board[ 1][31] = ' ';
  board[ 1][32] = ' ';
  board[ 1][33] = ' ';
  board[ 1][34] = ' ';
  board[ 1][35] = ' ';
  board[ 1][36] = ' ';
  board[ 1][37] = ' ';
  board[ 1][38] = ' ';
  board[ 1][39] = ' ';
  board[ 1][40] = ACS_VLINE;
  board[ 2][ 0] = ACS_VLINE;
  board[ 2][ 1] = ' ';
  board[ 2][ 2] = ' ';
  board[ 2][ 3] = ' ';
  board[ 2][ 4] = ' ';
  board[ 2][ 5] = ' ';
  board[ 2][ 6] = ' ';
  board[ 2][ 7] = ' ';
  board[ 2][ 8] = ' ';
  board[ 2][ 9] = ' ';
  board[ 2][10] = ACS_VLINE;
  board[ 2][11] = ' ';
  board[ 2][12] = ' ';
  board[ 2][13] = ' ';
  board[ 2][14] = ' ';
  board[ 2][15] = ' ';
  board[ 2][16] = ' ';
  board[ 2][17] = ' ';
  board[ 2][18] = ' ';
  board[ 2][19] = ' ';
  board[ 2][20] = ACS_VLINE;
  board[ 2][21] = ' ';
  board[ 2][22] = ' ';
  board[ 2][23] = ' ';
  board[ 2][24] = ' ';
  board[ 2][25] = ' ';
  board[ 2][26] = ' ';
  board[ 2][27] = ' ';
  board[ 2][28] = ' ';
  board[ 2][29] = ' ';
  board[ 2][30] = ACS_VLINE;
  board[ 2][31] = ' ';
  board[ 2][32] = ' ';
  board[ 2][33] = ' ';
  board[ 2][34] = ' ';
  board[ 2][35] = ' ';
  board[ 2][36] = ' ';
  board[ 2][37] = ' ';
  board[ 2][38] = ' ';
  board[ 2][39] = ' ';
  board[ 2][40] = ACS_VLINE;
  board[ 3][ 0] = ACS_VLINE;
  board[ 3][ 1] = ' ';
  board[ 3][ 2] = ' ';
  board[ 3][ 3] = ' ';
  board[ 3][ 4] = ' ';
  board[ 3][ 5] = ' ';
  board[ 3][ 6] = ' ';
  board[ 3][ 7] = ' ';
  board[ 3][ 8] = ' ';
  board[ 3][ 9] = ' ';
  board[ 3][10] = ACS_VLINE;
  board[ 3][11] = ' ';
  board[ 3][12] = ' ';
  board[ 3][13] = ' ';
  board[ 3][14] = ' ';
  board[ 3][15] = ' ';
  board[ 3][16] = ' ';
  board[ 3][17] = ' ';
  board[ 3][18] = ' ';
  board[ 3][19] = ' ';
  board[ 3][20] = ACS_VLINE;
  board[ 3][21] = ' ';
  board[ 3][22] = ' ';
  board[ 3][23] = ' ';
  board[ 3][24] = ' ';
  board[ 3][25] = ' ';
  board[ 3][26] = ' ';
  board[ 3][27] = ' ';
  board[ 3][28] = ' ';
  board[ 3][29] = ' ';
  board[ 3][30] = ACS_VLINE;
  board[ 3][31] = ' ';
  board[ 3][32] = ' ';
  board[ 3][33] = ' ';
  board[ 3][34] = ' ';
  board[ 3][35] = ' ';
  board[ 3][36] = ' ';
  board[ 3][37] = ' ';
  board[ 3][38] = ' ';
  board[ 3][39] = ' ';
  board[ 3][40] = ACS_VLINE;
  board[ 4][ 0] = ACS_LTEE;
  board[ 4][ 1] = ACS_HLINE;
  board[ 4][ 2] = ACS_HLINE;
  board[ 4][ 3] = ACS_HLINE;
  board[ 4][ 4] = ACS_HLINE;
  board[ 4][ 5] = ACS_HLINE;
  board[ 4][ 6] = ACS_HLINE;
  board[ 4][ 7] = ACS_HLINE;
  board[ 4][ 8] = ACS_HLINE;
  board[ 4][ 9] = ACS_HLINE;
  board[ 4][10] = ACS_PLUS;
  board[ 4][11] = ACS_HLINE;
  board[ 4][12] = ACS_HLINE;
  board[ 4][13] = ACS_HLINE;
  board[ 4][14] = ACS_HLINE;
  board[ 4][15] = ACS_HLINE;
  board[ 4][16] = ACS_HLINE;
  board[ 4][17] = ACS_HLINE;
  board[ 4][18] = ACS_HLINE;
  board[ 4][19] = ACS_HLINE;
  board[ 4][20] = ACS_PLUS;
  board[ 4][21] = ACS_HLINE;
  board[ 4][22] = ACS_HLINE;
  board[ 4][23] = ACS_HLINE;
  board[ 4][24] = ACS_HLINE;
  board[ 4][25] = ACS_HLINE;
  board[ 4][26] = ACS_HLINE;
  board[ 4][27] = ACS_HLINE;
  board[ 4][28] = ACS_HLINE;
  board[ 4][29] = ACS_HLINE;
  board[ 4][30] = ACS_PLUS;
  board[ 4][31] = ACS_HLINE;
  board[ 4][32] = ACS_HLINE;
  board[ 4][33] = ACS_HLINE;
  board[ 4][34] = ACS_HLINE;
  board[ 4][35] = ACS_HLINE;
  board[ 4][36] = ACS_HLINE;
  board[ 4][37] = ACS_HLINE;
  board[ 4][38] = ACS_HLINE;
  board[ 4][39] = ACS_HLINE;
  board[ 4][40] = ACS_RTEE;
  board[ 5][ 0] = ACS_VLINE;
  board[ 5][ 1] = ' ';
  board[ 5][ 2] = ' ';
  board[ 5][ 3] = ' ';
  board[ 5][ 4] = ' ';
  board[ 5][ 5] = ' ';
  board[ 5][ 6] = ' ';
  board[ 5][ 7] = ' ';
  board[ 5][ 8] = ' ';
  board[ 5][ 9] = ' ';
  board[ 5][10] = ACS_VLINE;
  board[ 5][11] = ' ';
  board[ 5][12] = ' ';
  board[ 5][13] = ' ';
  board[ 5][14] = ' ';
  board[ 5][15] = ' ';
  board[ 5][16] = ' ';
  board[ 5][17] = ' ';
  board[ 5][18] = ' ';
  board[ 5][19] = ' ';
  board[ 5][20] = ACS_VLINE;
  board[ 5][21] = ' ';
  board[ 5][22] = ' ';
  board[ 5][23] = ' ';
  board[ 5][24] = ' ';
  board[ 5][25] = ' ';
  board[ 5][26] = ' ';
  board[ 5][27] = ' ';
  board[ 5][28] = ' ';
  board[ 5][29] = ' ';
  board[ 5][30] = ACS_VLINE;
  board[ 5][31] = ' ';
  board[ 5][32] = ' ';
  board[ 5][33] = ' ';
  board[ 5][34] = ' ';
  board[ 5][35] = ' ';
  board[ 5][36] = ' ';
  board[ 5][37] = ' ';
  board[ 5][38] = ' ';
  board[ 5][39] = ' ';
  board[ 5][40] = ACS_VLINE;
  board[ 6][ 0] = ACS_VLINE;
  board[ 6][ 1] = ' ';
  board[ 6][ 2] = ' ';
  board[ 6][ 3] = ' ';
  board[ 6][ 4] = ' ';
  board[ 6][ 5] = ' ';
  board[ 6][ 6] = ' ';
  board[ 6][ 7] = ' ';
  board[ 6][ 8] = ' ';
  board[ 6][ 9] = ' ';
  board[ 6][10] = ACS_VLINE;
  board[ 6][11] = ' ';
  board[ 6][12] = ' ';
  board[ 6][13] = ' ';
  board[ 6][14] = ' ';
  board[ 6][15] = ' ';
  board[ 6][16] = ' ';
  board[ 6][17] = ' ';
  board[ 6][18] = ' ';
  board[ 6][19] = ' ';
  board[ 6][20] = ACS_VLINE;
  board[ 6][21] = ' ';
  board[ 6][22] = ' ';
  board[ 6][23] = ' ';
  board[ 6][24] = ' ';
  board[ 6][25] = ' ';
  board[ 6][26] = ' ';
  board[ 6][27] = ' ';
  board[ 6][28] = ' ';
  board[ 6][29] = ' ';
  board[ 6][30] = ACS_VLINE;
  board[ 6][31] = ' ';
  board[ 6][32] = ' ';
  board[ 6][33] = ' ';
  board[ 6][34] = ' ';
  board[ 6][35] = ' ';
  board[ 6][36] = ' ';
  board[ 6][37] = ' ';
  board[ 6][38] = ' ';
  board[ 6][39] = ' ';
  board[ 6][40] = ACS_VLINE;
  board[ 7][ 0] = ACS_VLINE;
  board[ 7][ 1] = ' ';
  board[ 7][ 2] = ' ';
  board[ 7][ 3] = ' ';
  board[ 7][ 4] = ' ';
  board[ 7][ 5] = ' ';
  board[ 7][ 6] = ' ';
  board[ 7][ 7] = ' ';
  board[ 7][ 8] = ' ';
  board[ 7][ 9] = ' ';
  board[ 7][10] = ACS_VLINE;
  board[ 7][11] = ' ';
  board[ 7][12] = ' ';
  board[ 7][13] = ' ';
  board[ 7][14] = ' ';
  board[ 7][15] = ' ';
  board[ 7][16] = ' ';
  board[ 7][17] = ' ';
  board[ 7][18] = ' ';
  board[ 7][19] = ' ';
  board[ 7][20] = ACS_VLINE;
  board[ 7][21] = ' ';
  board[ 7][22] = ' ';
  board[ 7][23] = ' ';
  board[ 7][24] = ' ';
  board[ 7][25] = ' ';
  board[ 7][26] = ' ';
  board[ 7][27] = ' ';
  board[ 7][28] = ' ';
  board[ 7][29] = ' ';
  board[ 7][30] = ACS_VLINE;
  board[ 7][31] = ' ';
  board[ 7][32] = ' ';
  board[ 7][33] = ' ';
  board[ 7][34] = ' ';
  board[ 7][35] = ' ';
  board[ 7][36] = ' ';
  board[ 7][37] = ' ';
  board[ 7][38] = ' ';
  board[ 7][39] = ' ';
  board[ 7][40] = ACS_VLINE;
  board[ 8][ 0] = ACS_LTEE;
  board[ 8][ 1] = ACS_HLINE;
  board[ 8][ 2] = ACS_HLINE;
  board[ 8][ 3] = ACS_HLINE;
  board[ 8][ 4] = ACS_HLINE;
  board[ 8][ 5] = ACS_HLINE;
  board[ 8][ 6] = ACS_HLINE;
  board[ 8][ 7] = ACS_HLINE;
  board[ 8][ 8] = ACS_HLINE;
  board[ 8][ 9] = ACS_HLINE;
  board[ 8][10] = ACS_PLUS;
  board[ 8][11] = ACS_HLINE;
  board[ 8][12] = ACS_HLINE;
  board[ 8][13] = ACS_HLINE;
  board[ 8][14] = ACS_HLINE;
  board[ 8][15] = ACS_HLINE;
  board[ 8][16] = ACS_HLINE;
  board[ 8][17] = ACS_HLINE;
  board[ 8][18] = ACS_HLINE;
  board[ 8][19] = ACS_HLINE;
  board[ 8][20] = ACS_PLUS;
  board[ 8][21] = ACS_HLINE;
  board[ 8][22] = ACS_HLINE;
  board[ 8][23] = ACS_HLINE;
  board[ 8][24] = ACS_HLINE;
  board[ 8][25] = ACS_HLINE;
  board[ 8][26] = ACS_HLINE;
  board[ 8][27] = ACS_HLINE;
  board[ 8][28] = ACS_HLINE;
  board[ 8][29] = ACS_HLINE;
  board[ 8][30] = ACS_PLUS;
  board[ 8][31] = ACS_HLINE;
  board[ 8][32] = ACS_HLINE;
  board[ 8][33] = ACS_HLINE;
  board[ 8][34] = ACS_HLINE;
  board[ 8][35] = ACS_HLINE;
  board[ 8][36] = ACS_HLINE;
  board[ 8][37] = ACS_HLINE;
  board[ 8][38] = ACS_HLINE;
  board[ 8][39] = ACS_HLINE;
  board[ 8][40] = ACS_RTEE;
  board[ 9][ 0] = ACS_VLINE;
  board[ 9][ 1] = ' ';
  board[ 9][ 2] = ' ';
  board[ 9][ 3] = ' ';
  board[ 9][ 4] = ' ';
  board[ 9][ 5] = ' ';
  board[ 9][ 6] = ' ';
  board[ 9][ 7] = ' ';
  board[ 9][ 8] = ' ';
  board[ 9][ 9] = ' ';
  board[ 9][10] = ACS_VLINE;
  board[ 9][11] = ' ';
  board[ 9][12] = ' ';
  board[ 9][13] = ' ';
  board[ 9][14] = ' ';
  board[ 9][15] = ' ';
  board[ 9][16] = ' ';
  board[ 9][17] = ' ';
  board[ 9][18] = ' ';
  board[ 9][19] = ' ';
  board[ 9][20] = ACS_VLINE;
  board[ 9][21] = ' ';
  board[ 9][22] = ' ';
  board[ 9][23] = ' ';
  board[ 9][24] = ' ';
  board[ 9][25] = ' ';
  board[ 9][26] = ' ';
  board[ 9][27] = ' ';
  board[ 9][28] = ' ';
  board[ 9][29] = ' ';
  board[ 9][30] = ACS_VLINE;
  board[ 9][31] = ' ';
  board[ 9][32] = ' ';
  board[ 9][33] = ' ';
  board[ 9][34] = ' ';
  board[ 9][35] = ' ';
  board[ 9][36] = ' ';
  board[ 9][37] = ' ';
  board[ 9][38] = ' ';
  board[ 9][39] = ' ';
  board[ 9][40] = ACS_VLINE;
  board[10][ 0] = ACS_VLINE;
  board[10][ 1] = ' ';
  board[10][ 2] = ' ';
  board[10][ 3] = ' ';
  board[10][ 4] = ' ';
  board[10][ 5] = ' ';
  board[10][ 6] = ' ';
  board[10][ 7] = ' ';
  board[10][ 8] = ' ';
  board[10][ 9] = ' ';
  board[10][10] = ACS_VLINE;
  board[10][11] = ' ';
  board[10][12] = ' ';
  board[10][13] = ' ';
  board[10][14] = ' ';
  board[10][15] = ' ';
  board[10][16] = ' ';
  board[10][17] = ' ';
  board[10][18] = ' ';
  board[10][19] = ' ';
  board[10][20] = ACS_VLINE;
  board[10][21] = ' ';
  board[10][22] = ' ';
  board[10][23] = ' ';
  board[10][24] = ' ';
  board[10][25] = ' ';
  board[10][26] = ' ';
  board[10][27] = ' ';
  board[10][28] = ' ';
  board[10][29] = ' ';
  board[10][30] = ACS_VLINE;
  board[10][31] = ' ';
  board[10][32] = ' ';
  board[10][33] = ' ';
  board[10][34] = ' ';
  board[10][35] = ' ';
  board[10][36] = ' ';
  board[10][37] = ' ';
  board[10][38] = ' ';
  board[10][39] = ' ';
  board[10][40] = ACS_VLINE;
  board[11][ 0] = ACS_VLINE;
  board[11][ 1] = ' ';
  board[11][ 2] = ' ';
  board[11][ 3] = ' ';
  board[11][ 4] = ' ';
  board[11][ 5] = ' ';
  board[11][ 6] = ' ';
  board[11][ 7] = ' ';
  board[11][ 8] = ' ';
  board[11][ 9] = ' ';
  board[11][10] = ACS_VLINE;
  board[11][11] = ' ';
  board[11][12] = ' ';
  board[11][13] = ' ';
  board[11][14] = ' ';
  board[11][15] = ' ';
  board[11][16] = ' ';
  board[11][17] = ' ';
  board[11][18] = ' ';
  board[11][19] = ' ';
  board[11][20] = ACS_VLINE;
  board[11][21] = ' ';
  board[11][22] = ' ';
  board[11][23] = ' ';
  board[11][24] = ' ';
  board[11][25] = ' ';
  board[11][26] = ' ';
  board[11][27] = ' ';
  board[11][28] = ' ';
  board[11][29] = ' ';
  board[11][30] = ACS_VLINE;
  board[11][31] = ' ';
  board[11][32] = ' ';
  board[11][33] = ' ';
  board[11][34] = ' ';
  board[11][35] = ' ';
  board[11][36] = ' ';
  board[11][37] = ' ';
  board[11][38] = ' ';
  board[11][39] = ' ';
  board[11][40] = ACS_VLINE;
  board[12][ 0] = ACS_LTEE;
  board[12][ 1] = ACS_HLINE;
  board[12][ 2] = ACS_HLINE;
  board[12][ 3] = ACS_HLINE;
  board[12][ 4] = ACS_HLINE;
  board[12][ 5] = ACS_HLINE;
  board[12][ 6] = ACS_HLINE;
  board[12][ 7] = ACS_HLINE;
  board[12][ 8] = ACS_HLINE;
  board[12][ 9] = ACS_HLINE;
  board[12][10] = ACS_PLUS;
  board[12][11] = ACS_HLINE;
  board[12][12] = ACS_HLINE;
  board[12][13] = ACS_HLINE;
  board[12][14] = ACS_HLINE;
  board[12][15] = ACS_HLINE;
  board[12][16] = ACS_HLINE;
  board[12][17] = ACS_HLINE;
  board[12][18] = ACS_HLINE;
  board[12][19] = ACS_HLINE;
  board[12][20] = ACS_PLUS;
  board[12][21] = ACS_HLINE;
  board[12][22] = ACS_HLINE;
  board[12][23] = ACS_HLINE;
  board[12][24] = ACS_HLINE;
  board[12][25] = ACS_HLINE;
  board[12][26] = ACS_HLINE;
  board[12][27] = ACS_HLINE;
  board[12][28] = ACS_HLINE;
  board[12][29] = ACS_HLINE;
  board[12][30] = ACS_PLUS;
  board[12][31] = ACS_HLINE;
  board[12][32] = ACS_HLINE;
  board[12][33] = ACS_HLINE;
  board[12][34] = ACS_HLINE;
  board[12][35] = ACS_HLINE;
  board[12][36] = ACS_HLINE;
  board[12][37] = ACS_HLINE;
  board[12][38] = ACS_HLINE;
  board[12][39] = ACS_HLINE;
  board[12][40] = ACS_RTEE;
  board[13][ 0] = ACS_VLINE;
  board[13][ 1] = ' ';
  board[13][ 2] = ' ';
  board[13][ 3] = ' ';
  board[13][ 4] = ' ';
  board[13][ 5] = ' ';
  board[13][ 6] = ' ';
  board[13][ 7] = ' ';
  board[13][ 8] = ' ';
  board[13][ 9] = ' ';
  board[13][10] = ACS_VLINE;
  board[13][11] = ' ';
  board[13][12] = ' ';
  board[13][13] = ' ';
  board[13][14] = ' ';
  board[13][15] = ' ';
  board[13][16] = ' ';
  board[13][17] = ' ';
  board[13][18] = ' ';
  board[13][19] = ' ';
  board[13][20] = ACS_VLINE;
  board[13][21] = ' ';
  board[13][22] = ' ';
  board[13][23] = ' ';
  board[13][24] = ' ';
  board[13][25] = ' ';
  board[13][26] = ' ';
  board[13][27] = ' ';
  board[13][28] = ' ';
  board[13][29] = ' ';
  board[13][30] = ACS_VLINE;
  board[13][31] = ' ';
  board[13][32] = ' ';
  board[13][33] = ' ';
  board[13][34] = ' ';
  board[13][35] = ' ';
  board[13][36] = ' ';
  board[13][37] = ' ';
  board[13][38] = ' ';
  board[13][39] = ' ';
  board[13][40] = ACS_VLINE;
  board[14][ 0] = ACS_VLINE;
  board[14][ 1] = ' ';
  board[14][ 2] = ' ';
  board[14][ 3] = ' ';
  board[14][ 4] = ' ';
  board[14][ 5] = ' ';
  board[14][ 6] = ' ';
  board[14][ 7] = ' ';
  board[14][ 8] = ' ';
  board[14][ 9] = ' ';
  board[14][10] = ACS_VLINE;
  board[14][11] = ' ';
  board[14][12] = ' ';
  board[14][13] = ' ';
  board[14][14] = ' ';
  board[14][15] = ' ';
  board[14][16] = ' ';
  board[14][17] = ' ';
  board[14][18] = ' ';
  board[14][19] = ' ';
  board[14][20] = ACS_VLINE;
  board[14][21] = ' ';
  board[14][22] = ' ';
  board[14][23] = ' ';
  board[14][24] = ' ';
  board[14][25] = ' ';
  board[14][26] = ' ';
  board[14][27] = ' ';
  board[14][28] = ' ';
  board[14][29] = ' ';
  board[14][30] = ACS_VLINE;
  board[14][31] = ' ';
  board[14][32] = ' ';
  board[14][33] = ' ';
  board[14][34] = ' ';
  board[14][35] = ' ';
  board[14][36] = ' ';
  board[14][37] = ' ';
  board[14][38] = ' ';
  board[14][39] = ' ';
  board[14][40] = ACS_VLINE;
  board[15][ 0] = ACS_VLINE;
  board[15][ 1] = ' ';
  board[15][ 2] = ' ';
  board[15][ 3] = ' ';
  board[15][ 4] = ' ';
  board[15][ 5] = ' ';
  board[15][ 6] = ' ';
  board[15][ 7] = ' ';
  board[15][ 8] = ' ';
  board[15][ 9] = ' ';
  board[15][10] = ACS_VLINE;
  board[15][11] = ' ';
  board[15][12] = ' ';
  board[15][13] = ' ';
  board[15][14] = ' ';
  board[15][15] = ' ';
  board[15][16] = ' ';
  board[15][17] = ' ';
  board[15][18] = ' ';
  board[15][19] = ' ';
  board[15][20] = ACS_VLINE;
  board[15][21] = ' ';
  board[15][22] = ' ';
  board[15][23] = ' ';
  board[15][24] = ' ';
  board[15][25] = ' ';
  board[15][26] = ' ';
  board[15][27] = ' ';
  board[15][28] = ' ';
  board[15][29] = ' ';
  board[15][30] = ACS_VLINE;
  board[15][31] = ' ';
  board[15][32] = ' ';
  board[15][33] = ' ';
  board[15][34] = ' ';
  board[15][35] = ' ';
  board[15][36] = ' ';
  board[15][37] = ' ';
  board[15][38] = ' ';
  board[15][39] = ' ';
  board[15][40] = ACS_VLINE;
  board[16][ 0] = ACS_LLCORNER;
  board[16][ 1] = ACS_HLINE;
  board[16][ 2] = ACS_HLINE;
  board[16][ 3] = ACS_HLINE;
  board[16][ 4] = ACS_HLINE;
  board[16][ 5] = ACS_HLINE;
  board[16][ 6] = ACS_HLINE;
  board[16][ 7] = ACS_HLINE;
  board[16][ 8] = ACS_HLINE;
  board[16][ 9] = ACS_HLINE;
  board[16][10] = ACS_BTEE;
  board[16][11] = ACS_HLINE;
  board[16][12] = ACS_HLINE;
  board[16][13] = ACS_HLINE;
  board[16][14] = ACS_HLINE;
  board[16][15] = ACS_HLINE;
  board[16][16] = ACS_HLINE;
  board[16][17] = ACS_HLINE;
  board[16][18] = ACS_HLINE;
  board[16][19] = ACS_HLINE;
  board[16][20] = ACS_BTEE;
  board[16][21] = ACS_HLINE;
  board[16][22] = ACS_HLINE;
  board[16][23] = ACS_HLINE;
  board[16][24] = ACS_HLINE;
  board[16][25] = ACS_HLINE;
  board[16][26] = ACS_HLINE;
  board[16][27] = ACS_HLINE;
  board[16][28] = ACS_HLINE;
  board[16][29] = ACS_HLINE;
  board[16][30] = ACS_BTEE;
  board[16][31] = ACS_HLINE;
  board[16][32] = ACS_HLINE;
  board[16][33] = ACS_HLINE;
  board[16][34] = ACS_HLINE;
  board[16][35] = ACS_HLINE;
  board[16][36] = ACS_HLINE;
  board[16][37] = ACS_HLINE;
  board[16][38] = ACS_HLINE;
  board[16][39] = ACS_HLINE;
  board[16][40] = ACS_LRCORNER;
}

void init_terminal(void)
{
  initscr();
  raw();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  start_color();
  init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
  init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
  init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
  init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
  init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
  init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
}

void render_board() {
  uint32_t y, x, xoff;
  int color;
  int ul;

  clear();

  /* Board will be 17x41. */
  /* Center it, and place the score above it. */

  mvprintw(0, 1, "CURSES-2048");
  mvprintw(2, 21, "Score: %u", game[game_index].score);

  for (y = 0; y < 17; y++) {
    for (x = 0; x < 41; x++) {
      mvaddch(3 + y, 20 + x, board[y][x]);
    }
  }

  for (y = ul = 0; y < 4; y++) {
    for (x = 0; x < 4; x++) {
      if (game[game_index].tableau[y][x] < 0) {
        attron(A_UNDERLINE);
        ul = 1;
        game[game_index].tableau[y][x] = -game[game_index].tableau[y][x];
      }
      if (game[game_index].tableau[y][x]) {
        switch (game[game_index].tableau[y][x]) {
        case 2:
          attron(COLOR_PAIR(COLOR_WHITE));
          color = COLOR_WHITE;
          xoff = 4;
          break;
        case 4:
          attron(COLOR_PAIR(COLOR_RED));
          color = COLOR_RED;
          xoff = 4;
          break;
        case 8:
          attron(COLOR_PAIR(COLOR_GREEN));
          color = COLOR_GREEN;
          xoff = 4;
          break;
        case 16:
          attron(COLOR_PAIR(COLOR_YELLOW));
          color = COLOR_YELLOW;
          xoff = 4;
          break;
        case 32:
          attron(COLOR_PAIR(COLOR_BLUE));
          color = COLOR_BLUE;
          xoff = 4;
          break;
        case 64:
          attron(COLOR_PAIR(COLOR_MAGENTA));
          color = COLOR_MAGENTA;
          xoff = 4;
          break;
        case 128:
          attron(COLOR_PAIR(COLOR_CYAN));
          color = COLOR_CYAN;
          xoff = 3;
          break;
        case 256:
          attron(COLOR_PAIR(COLOR_WHITE));
          color = COLOR_WHITE;
          attron(A_BOLD);
          xoff = 3;
          break;
        case 512:
          attron(COLOR_PAIR(COLOR_RED));
          color = COLOR_RED;
          attron(A_BOLD);
          xoff = 3;
          break;
        case 1024:
          attron(COLOR_PAIR(COLOR_GREEN));
          color = COLOR_GREEN;
          attron(A_BOLD);
          xoff = 3;
          break;
        case 2048:
          attron(COLOR_PAIR(COLOR_YELLOW));
          color = COLOR_YELLOW;
          attron(A_BOLD);
          xoff = 3;
          break;
        case 4096:
          attron(COLOR_PAIR(COLOR_BLUE));
          color = COLOR_BLUE;
          attron(A_BOLD);
          xoff = 3;
          break;
        case 8192:
          attron(COLOR_PAIR(COLOR_MAGENTA));
          color = COLOR_MAGENTA;
          attron(A_BOLD);
          xoff = 3;
          break;
        case 16384:
          attron(COLOR_PAIR(COLOR_CYAN));
          color = COLOR_CYAN;
          attron(A_BOLD);
          xoff = 2;
          break;
        }
        mvprintw(5 + (y * 4), 21 + xoff + (x * 10), "%d", game[game_index].tableau[y][x]);
        if (ul) {
          attroff(A_UNDERLINE);
          ul = 0;
        }
        attroff(A_BOLD);
        attroff(COLOR_PAIR(color));
      }
    }
  }

  mvprintw(23, 1, "Jeremy W. Sheaffer <jeremy@sheaffer.ws>");


  refresh();
}

void init_tableau()
{
  uint32_t y, x;

  y = rand() % 4;
  x = rand() % 4;
  game[game_index].tableau[y][x] = 2;

  do {
    y = rand() % 4;
    x = rand() % 4;
  } while (game[game_index].tableau[y][x]);
  game[game_index].tableau[y][x] = 4;
}

uint32_t shift_copy(int32_t a[4])
{
  uint32_t fail = 1;

  if (!a[3] && a[2]) {
    a[3] = a[2];
    a[2] = 0;
    fail = 0;
  }
  if (!a[3] && a[1]) {
    a[3] = a[1];
    a[1] = 0;
    fail = 0;
  }
  if (!a[3] && a[0]) {
    a[3] = a[0];
    a[0] = 0;
    fail = 0;
  }
  if (!a[2] && a[1]) {
    a[2] = a[1];
    a[1] = 0;
    fail = 0;
  }
  if (!a[2] && a[0]) {
    a[2] = a[0];
    a[0] = 0;
    fail = 0;
  }
  if (!a[1] && a[0]) {
    a[1] = a[0];
    a[0] = 0;
    fail = 0;
  }

  if (a[3] && (a[3] == a[2])) {
    a[3] *= 2;
    a[2] = 0;
    game[game_index].score += a[3];
    fail = 0;

    if (a[1] && (a[1] == a[0])) {
      a[1] *= 2;
      a[0] = 0;
      game[game_index].score += a[1];
    }
  } else {
    if (a[2] && (a[2] == a[1])) {
      a[2] *= 2;
      a[1] = 0;
      game[game_index].score += a[2];
      fail = 0;
    } else if (a[1] && (a[1] == a[0])) {
      a[1] *= 2;
      a[0] = 0;
      game[game_index].score += a[1];
      fail = 0;
    }

  }
  if (!a[2] && a[1]) {
    a[2] = a[1];
    a[1] = 0;
  }
  if (!a[2] && a[0]) {
    a[2] = a[0];
    a[0] = 0;
  }
  if (!a[1] && a[0]) {
    a[1] = a[0];
    a[0] = 0;
  }

  return fail;
}

uint32_t shift_row_right(uint32_t col)
{
  int32_t copy[4];

  copy[0] = game[game_index].tableau[col][0];
  copy[1] = game[game_index].tableau[col][1];
  copy[2] = game[game_index].tableau[col][2];
  copy[3] = game[game_index].tableau[col][3];

  if (!shift_copy(copy)) {
    game[game_index].tableau[col][0] = copy[0];
    game[game_index].tableau[col][1] = copy[1];
    game[game_index].tableau[col][2] = copy[2];
    game[game_index].tableau[col][3] = copy[3];

    return 0;
  }

  return 1;
}

uint32_t shift_right()
{
  return (shift_row_right(0) *
          shift_row_right(1) *
          shift_row_right(2) *
          shift_row_right(3));
}

uint32_t shift_row_left(uint32_t col)
{
  int32_t copy[4];

  copy[0] = game[game_index].tableau[col][3];
  copy[1] = game[game_index].tableau[col][2];
  copy[2] = game[game_index].tableau[col][1];
  copy[3] = game[game_index].tableau[col][0];

  if (!shift_copy(copy)) {
    game[game_index].tableau[col][3] = copy[0];
    game[game_index].tableau[col][2] = copy[1];
    game[game_index].tableau[col][1] = copy[2];
    game[game_index].tableau[col][0] = copy[3];

    return 0;
  }

  return 1;
}

uint32_t shift_left()
{
  return (shift_row_left(0) *
          shift_row_left(1) *
          shift_row_left(2) *
          shift_row_left(3));
}

uint32_t shift_column_up(uint32_t row)
{
  int32_t copy[4];

  copy[0] = game[game_index].tableau[3][row];
  copy[1] = game[game_index].tableau[2][row];
  copy[2] = game[game_index].tableau[1][row];
  copy[3] = game[game_index].tableau[0][row];

  if (!shift_copy(copy)) {
    game[game_index].tableau[3][row] = copy[0];
    game[game_index].tableau[2][row] = copy[1];
    game[game_index].tableau[1][row] = copy[2];
    game[game_index].tableau[0][row] = copy[3];

    return 0;
  }

  return 1;
}

uint32_t shift_up()
{
  return (shift_column_up(0) *
          shift_column_up(1) *
          shift_column_up(2) *
          shift_column_up(3));
}

uint32_t shift_column_down(uint32_t row)
{
  int32_t copy[4];

  copy[0] = game[game_index].tableau[0][row];
  copy[1] = game[game_index].tableau[1][row];
  copy[2] = game[game_index].tableau[2][row];
  copy[3] = game[game_index].tableau[3][row];

  if (!shift_copy(copy)) {
    game[game_index].tableau[0][row] = copy[0];
    game[game_index].tableau[1][row] = copy[1];
    game[game_index].tableau[2][row] = copy[2];
    game[game_index].tableau[3][row] = copy[3];

    return 0;
  }

  return 1;
}

uint32_t shift_down()
{
  return (shift_column_down(0) *
          shift_column_down(1) *
          shift_column_down(2) *
          shift_column_down(3));
}

uint32_t add_tile()
{
  uint32_t y, x;

  do {
    y = rand() % 4;
    x = rand() % 4;
  } while (game[game_index].tableau[y][x]);
  /* Add a negative so that we can make it in the display. *
   * Change it to a positive there.                        */
  game[game_index].tableau[y][x] = ((rand() % 10) ? -2 : -4);

  if (!game[game_index].tableau[0][0] || !game[game_index].tableau[0][1] ||
      !game[game_index].tableau[0][2] || !game[game_index].tableau[0][3] ||
      !game[game_index].tableau[1][0] || !game[game_index].tableau[1][1] ||
      !game[game_index].tableau[1][2] || !game[game_index].tableau[1][3] ||
      !game[game_index].tableau[2][0] || !game[game_index].tableau[2][1] ||
      !game[game_index].tableau[2][2] || !game[game_index].tableau[2][3] ||
      !game[game_index].tableau[3][0] || !game[game_index].tableau[3][1] ||
      !game[game_index].tableau[3][2] || !game[game_index].tableau[3][3] ||
      (abs(game[game_index].tableau[0][0]) ==
       abs(game[game_index].tableau[0][1])) ||
      (abs(game[game_index].tableau[0][0]) ==
       abs(game[game_index].tableau[1][0])) ||
      (abs(game[game_index].tableau[0][1]) ==
       abs(game[game_index].tableau[0][2])) ||
      (abs(game[game_index].tableau[0][1]) ==
       abs(game[game_index].tableau[1][1])) ||
      (abs(game[game_index].tableau[0][2]) ==
       abs(game[game_index].tableau[0][3])) ||
      (abs(game[game_index].tableau[0][2]) ==
       abs(game[game_index].tableau[1][2])) ||
      (abs(game[game_index].tableau[0][3]) ==
       abs(game[game_index].tableau[1][3])) ||
      (abs(game[game_index].tableau[1][0]) ==
       abs(game[game_index].tableau[1][1])) ||
      (abs(game[game_index].tableau[1][0]) ==
       abs(game[game_index].tableau[2][0])) ||
      (abs(game[game_index].tableau[1][1]) ==
       abs(game[game_index].tableau[1][2])) ||
      (abs(game[game_index].tableau[1][1]) ==
       abs(game[game_index].tableau[2][1])) ||
      (abs(game[game_index].tableau[1][2]) ==
       abs(game[game_index].tableau[1][3])) ||
      (abs(game[game_index].tableau[1][2]) ==
       abs(game[game_index].tableau[2][2])) ||
      (abs(game[game_index].tableau[1][3]) ==
       abs(game[game_index].tableau[2][3])) ||
      (abs(game[game_index].tableau[2][0]) ==
       abs(game[game_index].tableau[2][1])) ||
      (abs(game[game_index].tableau[2][0]) ==
       abs(game[game_index].tableau[3][0])) ||
      (abs(game[game_index].tableau[2][1]) ==
       abs(game[game_index].tableau[2][2])) ||
      (abs(game[game_index].tableau[2][1]) ==
       abs(game[game_index].tableau[3][1])) ||
      (abs(game[game_index].tableau[2][2]) ==
       abs(game[game_index].tableau[2][3])) ||
      (abs(game[game_index].tableau[2][2]) ==
       abs(game[game_index].tableau[3][2])) ||
      (abs(game[game_index].tableau[2][3]) ==
       abs(game[game_index].tableau[3][3])) ||
      (abs(game[game_index].tableau[3][0]) ==
       abs(game[game_index].tableau[3][1])) ||
      (abs(game[game_index].tableau[3][1]) ==
       abs(game[game_index].tableau[3][2])) ||
      (abs(game[game_index].tableau[3][2]) ==
       abs(game[game_index].tableau[3][3]))) {
    return 0;
  }

  return 1;
}

void game_loop()
{
  uint32_t dead, no_op;
  int32_t key;

  dead = no_op = 0;

  render_board();
  while (!dead) {
    key = getch();
    switch (key) {
    case KEY_DOWN:
      no_op = shift_down();
      break;
    case KEY_UP:
      no_op = shift_up();
      break;
    case KEY_LEFT:
      no_op = shift_left();
      break;
    case KEY_RIGHT:
      no_op = shift_right();
      break;
    case 'U':
      no_op = 1;
      pop_undo_state();
      pop_undo_state();
      push_undo_state();
      render_board();
      break;
    case 'Q':
      dead = 1;
      no_op = 1;
      break;
      /*
    case ' ':
      no_op = 0;
      break;
      */
    default:
      mvprintw(21, 1,
               "Use arrow keys to shift and combine tiles, "
               "'U' to undo a move, or 'Q' to quit.");
      no_op = 1;
      break;
    }
    if (!no_op) {
      dead = add_tile();
      push_undo_state();
      render_board();
    }
  }

  render_board();
  mvprintw(22, 36, "GAME OVER");
  getch();
}

int32_t highest_tile()
{
  int32_t ht;
  uint32_t y, x;

  for (ht = game[game_index].tableau[0][0], y = 0; y < 4; y++) {
    for (x = 0; x < 4; x++) {
      if (ht < game[game_index].tableau[y][x]) {
        ht = game[game_index].tableau[y][x];
      }
    }
  }

  return ht;
}

int main(int argc, char *argv[])
{
  srand(time(NULL));

  init_terminal();

  init_board();

  init_tableau();

  game_loop();

  endwin();

  printf("Game over with highest tile %d and score %u\n",
         highest_tile(), game[game_index].score);

  return 0;
}

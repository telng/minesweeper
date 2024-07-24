// Minesweeper.c

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define XY(x,y) ((x) + (y)*(board_x+2))
int move[16] = { -1, -1, 0, -1, 1, -1, -1, 0, 1, 0, -1, 1, 0, 1, 1, 1 };
void clear(int guess_x, int guess_y, int board_x, int board_y, char* mboard, char* dboard) {
    if (mboard[XY(guess_x, guess_y)]) return;    // Already been here
    if (guess_x < 1 || guess_y < 1 || guess_x > board_x || guess_y > board_y) return;   // Out of bounds
    mboard[XY(guess_x, guess_y)] = 1;
    if (dboard[XY(guess_x, guess_y)] == 0) {
        for (int i = 0; i < 16; i += 2) {
            clear(guess_x + move[i], guess_y + move[i + 1], board_x, board_y, mboard, dboard);
        }
    }
}
int main(int argc, char *argv[])
{
    int guess_x, guess_y;               // Where we step next
    int i, x, y, c;                     // Iterators etc.
    if (argc != 4) exit(0);
    int board_x = atoi(argv[1]);
    int board_y = atoi(argv[2]);
    int mines = atoi(argv[3]);
    char* board = (char *) calloc((board_x+2) * (board_y+2), 1);    // Margin around the edge of the board to ease display calcs
    do {
        board[XY(1 + (rand() % board_x), 1 + (rand() % board_y))] = 1;
        for (c = i = 0; i < (board_x + 2) * (board_y + 2); i++) c += board[i];  // Count mines (so we cope if two are laid in the same place)
    } while (c < mines);
    char* dboard = (char*) calloc((board_x+2) * (board_y+2), 1);    // Shows number of mines around each cell
    for (x = 1; x < board_x + 1; x++) {
        for (y = 1; y < board_y + 1; y++) {
            dboard[XY(x, y)] = board[XY(x-1, y)] + board[XY(x+1, y)] + board[XY(x-1, y-1)] + board[XY(x, y-1)] + board[XY(x+1, y-1)] + 
                board[XY(x-1, y+1)] + board[XY(x, y+1)] + board[XY(x+1, y+1)];
        }
    }
    char *mboard = (char*)calloc((board_x + 2) * (board_y + 2), 1); // Mask - set to 1 if a cell's been revealed
    int alive = 1;
    for (;;) {
        c = 0;  // Count of cells remaining unknown
        for (int y = 0; y < board_y + 1; y++) {
            printf((y > 0) ? "\n%2d" : "  ", y);
            for (int x = 1; x < board_x + 1; x++) {
                if ((y == 0) || (mboard[XY(x, y)])) printf("%3d", (y == 0) ? x : dboard[XY(x, y)]);
                else printf("  .");
                if (y > 0) c += mboard[XY(x, y)] == 0 ? 1 : 0;
            }
        }
        if (!alive) {
            printf("\nBOOM!!");
            exit(0);
        }
        if (c == mines) {
            printf("\nYOU WIN!!\n");
            exit(0);
        }
        do {
            printf("\nEnter co-ordinates as x,y\n");
            scanf("%d,%d", &guess_x, &guess_y);
        } while (guess_x < 1 || guess_x > board_x || guess_y < 1 || guess_y > board_y);
        if (board[XY(guess_x, guess_y)]) {
            alive = 0;
            memset(mboard, 1, (board_x + 2) * (board_y + 2));   // Show everything
        } else {
            clear(guess_x, guess_y, board_x, board_y, mboard, dboard);
        }
    }
}
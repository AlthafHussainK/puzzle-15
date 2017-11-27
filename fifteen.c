/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d, moves = 0, bt_r, bt_c;;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void swap(int *x, int *y);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void swap(int *x, int *y)
 {
   int temp = *x;
   *x = *y;
   *y = temp;
 }

void init(void)
{
    int n = d*d - 1;
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            board[i][j] = n;
            n--;
        }
    }
    if(d % 2 == 0)
    {
        swap(&board[d-1][d-2], &board[d-1][d-3]);
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if(board[i][j] == 0)
                printf(" _ ");
            else
                printf("%2d ", board[i][j]);

        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */

bool move(int tile)
{
    if(moves == 0)
    {
        bt_r = d-1;
        bt_c = d-1;
    }
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if(board[i][j] == tile)
            {
                if(((i == bt_r + 1 || i == bt_r - 1) && j == bt_c) || ((j == bt_c + 1 || j == bt_c - 1) && i == bt_r))
                {
                    swap(&board[i][j], &board[bt_r][bt_c]);
                    bt_r = i;
                    bt_c = j;
                    moves++;
                    return true;
                }
                else
                {
                    moves++;
                    return false;
                }
            }
        }
    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
bool won(void)
{
  //left to right
  for(int i = 0; i < d; i++)
    {
      for(int j = 0; j < d; j++)
      {
        for(int k = j+1; k < d; k++)
        {
          if(i == d-1 && k == d-1)
            break;

        if(board[i][j] > board[i][k])
            return false;
        }
      }
    }
    //top to bottom
    for(int n = 0; n < d; n++)
    {
      for(int m = 0; m < d; m++)
      {
        for(int l = m+1; l < d; l++)
        {
          if(l == d-1 && n == d-1)
            break;

          if(board[m][n] > board[l][n])
            return false;
        }
      }
    }

  return true;
}

// PacMan in C

#include <ncurses.h> //Includes input/output and keybaord function
#include <unistd.h>  //Includes for sleep function
#include <time.h>   
#include <stdlib.h>     //For random

#define PacMan_Pair 1 // Color-pair macro for the pacman
#define Ghost_Pair 2  // Color-pair macro for the ghost

char map[33][73] = {{"#######################################################################"}, // MAP
                    {"#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  #"},
                    {"#  -  ###########  -  ###########  -  ###########  -  ###########  -  #"},
                    {"#  -  #         #  -  #         #  -  #         #  -  #         #  -  #"},
                    {"#  -  ###########  -  ###########  -  ###########  -  ###########  -  #"},
                    {"#  -  - - - - - -  -  - - - - - -  -  - - - - - -  -  - - - - - -  -  #"},
                    {"#  -  ###########  -  ##     #############     ##  -  ###########  -  #"},
                    {"#  -  ###########  -  ##     #####   #####     ##  -  ###########  -  #"},
                    {"#  -  - -  -  - -  -  ##         #   #         ##  -  - -  - - - - -  #"},
                    {"#########  -  ###  -  #####      #   #      #####  -  ###  -  #########"},
                    {"        #  -  # #  -  #####      #   #      #####  -  # #  -  #        "},
                    {"        #  -  # #  -  ##         #   #         ##  -  # #  -  #        "},
                    {"        #  -  # #  -  ##         # # #         ##  -  # #  -  #        "},
                    {"#########  -  ###  -                               -  ###  -  #########"},
                    {"           -  - -  -  ## ######         ###### ##  -  - -  -           "},
                    {"#########  -  ###  -  ## #                   # ##  -  ###  -  #########"},
                    {"        #  -  # #  -  ## #                   # ##  -  # #  -  #        "},
                    {"        #  -  # #  -  ## ##################### ##  -  # #  -  #        "},
                    {"        #  -  # #  -  ##                       ##  -  # #  -  #        "},
                    {"        #  -  # #  -  ##    ###############    ##  -  # #  -  #        "},
                    {"#########  -  ###  -  ##    ######   ######    ##  -  ###  -  #########"},
                    {"#  -  - -  - - - - -             #   #             - - - - -  - -  -  #"},
                    {"#  -  #######  -  ##########     #   #     ##########  -  #######  -  #"},
                    {"#  -  #######  -  ##########     #####     ##########  -  #######  -  #"},
                    {"#  - -  -  ##  - - - - - - - - - - - - - - - - - - - - -  ##  -  - -  #"},
                    {"######  -  ##  -  #####  -  ###############  -  #####  -  ##  -  ######"},
                    {"######  -  ##  -  #   #  -  ######   ######  -  #   #  -  ##  -  ######"},
                    {"#  - -  -  - - -  #   #  - -  -  #   #  -  - -  #   #  -  - - -  - -  #"},
                    {"#  -  #############   ######  -  #   #  -  ######   #############  -  #"},
                    {"#  -  #                    #  -  #   #  -  #                    #  -  #"},
                    {"#  -  ######################  -  #####  -  ######################  -  #"},
                    {"#  - - - - - - - - - - - - -  -  -   -  -  - - - - - - - - - - - - -  #"},
                    {"#######################################################################"}};

int GameStatus;                                                    // Game is running(1) - GameOver(0)
int forbidden_left, forbidden_right, forbidden_up, forbidden_down; // Region where Pacman cannot enter

typedef enum Direction                           // Direction of pacman
{
    STATIC,                                      // Initial state, when the game begins, pacman is stationary
    LEFT,
    RIGHT,
    UP,
    DOWN
} direction;
direction dir;

typedef struct Pacman                           // Pacman info
{
    int Px, Py;                                 // Position of Pacman
    int VPx, VPy;                               // Speed of Pacman
} Pac;
Pac pac;

typedef struct Ghosts // Ghost info
{
    int Gx, Gy;   // Position of Ghost
    int Vgy, Vgx; // Speed of Ghost
    // int start;

} Ghost;
Ghost gh[4];

const char ghost_direction[4] = {'L', 'U', 'R', 'D'};        // Direction of movement of ghost

void start_screen();
void draw_map();                                             // Drawing the layout
void init();                                                 // Initialising the coordinates of entities
void key_input();                                            // Taking Key input from the user
void motion();                                               // Movement of pacman
void ghost_movement(int i, const char[]);                    // Movement of the ghosts
void collision();                                            // Checks for collision of pacman with the ghost
void food_empty();                                           // Winning condition

int main()
{
    init();
    start_screen();
    sleep(2);

    srand(time(NULL));

    while (GameStatus)
    {
        draw_map();
        key_input();
        motion();
        for (int i = 0; i < 4;i++){
            ghost_movement(i, ghost_direction);
        }
        
    }
    // getch();
    sleep(3);
    endwin();
}

void key_input()
{
    keypad(stdscr, TRUE);
    halfdelay(1);

    int KeyPress = getch();

    switch (KeyPress)
    {
    case KEY_LEFT:
        dir = LEFT;
        break;
    case KEY_RIGHT:
        dir = RIGHT;
        break;
    case KEY_UP:
        dir = UP;
        break;
    case KEY_DOWN:
        dir = DOWN;
        break;
    default:
        dir = STATIC;
        break;
    }
}

void draw_map()
{
    clear();
    map[pac.Py][pac.Px] = 'C';
    for (int i = 0; i <= 32; i++)
    {
        for (int j = 0; j <= 72; j++)
        {
            if (j == gh[0].Gx && i == gh[0].Gy)
            {
                attron(COLOR_PAIR(Ghost_Pair));
                mvaddch(i, j, 'M');
                attroff(COLOR_PAIR(Ghost_Pair));
                continue;
            }
            if (j == gh[1].Gx && i == gh[1].Gy)
            {
                attron(COLOR_PAIR(Ghost_Pair));
                mvaddch(i, j, 'M');
                attroff(COLOR_PAIR(Ghost_Pair));
                continue;
            }
            if (j == gh[2].Gx && i == gh[2].Gy)
            {
                attron(COLOR_PAIR(Ghost_Pair));
                mvaddch(i, j, 'M');
                attroff(COLOR_PAIR(Ghost_Pair));
                continue;
            }
            if (j == gh[3].Gx && i == gh[3].Gy)
            {
                attron(COLOR_PAIR(Ghost_Pair));
                mvaddch(i, j, 'M');
                attroff(COLOR_PAIR(Ghost_Pair));
                continue;
            }
            if (map[i][j] == 'C')
            {
                attron(COLOR_PAIR(PacMan_Pair));
                mvaddch(i, j, map[i][j]);
                attroff(COLOR_PAIR(PacMan_Pair));
            }
            else{
               printw("%c", map[i][j]); 
            }
        }
        printw("\n");
    }
    refresh();
}

void init()
{
    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);

    GameStatus = 1;
    dir = STATIC;

    start_color();
    init_pair(PacMan_Pair, COLOR_YELLOW, COLOR_BLACK);
    init_pair(Ghost_Pair, COLOR_RED, COLOR_BLACK);

    pac.VPx = 1;
    pac.VPy = 1;
    pac.Px = 35;                                // Inital x coordinate of pacman
    pac.Py = 31;                                // intial y coordinate of pacman
    map[pac.Py][pac.Px] = 'C';

    gh[0].Gx = 26;
    gh[0].Gy = 6;
    gh[0].Vgx = 1;
    gh[0].Vgy = 1;

    gh[1].Gx = 45;
    gh[1].Gy = 6;
    gh[1].Vgx = 1;
    gh[1].Vgy = 1;

    gh[2].Gx = 26;
    gh[2].Gy = 21;
    gh[2].Vgx = 1;
    gh[2].Vgy = 1;

    gh[3].Gx = 47;
    gh[3].Gy = 21;
    gh[3].Vgy = 1;
    gh[3].Vgx = 1;
}

void motion()
{
    collision();
    food_empty();

    forbidden_left = ((pac.Py == 14 && pac.Px == 11) || ((pac.Py == 21 || pac.Py == 13) && pac.Px == 51)) ? 1 : 0;
    forbidden_right = ((pac.Py == 14 && pac.Px == 59) || ((pac.Py == 21 || pac.Py == 13) && pac.Px == 19)) ? 1 : 0;
    forbidden_up = ((pac.Px >= 23 && pac.Px <= 48) && pac.Py == 24) ? 1 : 0;
    forbidden_down = ((pac.Px >= 23 && pac.Px <= 48) && pac.Py == 5) ? 1 : 0;

    switch (dir)
    {
    case LEFT:
        if (map[(pac.Py)][(pac.Px - 3)] != '#' && !forbidden_left)
        {
            map[pac.Py][pac.Px] = ' ';
            pac.Px -= pac.VPx;
        }
        break;
    case RIGHT:
        if (map[(pac.Py)][(pac.Px + 3)] != '#' && !forbidden_right)
        {
            map[pac.Py][pac.Px] = ' ';
            pac.Px += pac.VPx;
        }
        break;
    case UP:
        if ((!(map[(pac.Py - 1)][(pac.Px)] == '#') && !(map[(pac.Py - 1)][(pac.Px - 1)] == '#' || map[(pac.Py - 1)][(pac.Px - 2)] == '#' || map[(pac.Py - 1)][(pac.Px + 1)] == '#' || map[(pac.Py - 1)][(pac.Px + 2)] == '#')) && !forbidden_up)
        {
            map[pac.Py][pac.Px] = ' ';
            pac.Py -= pac.VPy;
        }
        break;
    case DOWN:
        if ((!(map[(pac.Py + 1)][(pac.Px)] == '#') && !(map[(pac.Py + 1)][(pac.Px - 1)] == '#' || map[(pac.Py + 1)][(pac.Px - 2)] == '#' || map[(pac.Py + 1)][(pac.Px + 1)] == '#' || map[(pac.Py + 1)][(pac.Px + 2)] == '#')) && !forbidden_down)
        {
            map[pac.Py][pac.Px] = ' ';
            pac.Py += pac.VPy;
        }
        break;
    default:
        break;
    }
}

void ghost_movement(int i, const char ghost_direction[])
{
    collision();
    static char ghost_dir[4] = {'0', '0', '0', '0'};
    switch (ghost_dir[i])
    {
    case 'L':
        if (map[(gh[i].Gy)][(gh[i].Gx - 3)] != '#')
        {
            gh[i].Gx -= gh[i].Vgx;
        }
        else
        {
            ghost_dir[i] = ghost_direction[rand() % 4];
        }
        break;
    case 'R':
        if (map[(gh[i].Gy)][(gh[i].Gx + 3)] != '#')
        {
            gh[i].Gx += gh[i].Vgx;
        }
        else
        {
            ghost_dir[i] = ghost_direction[rand() % 4];
        }
        break;
    case 'U':
        if (!(map[(gh[i].Gy - 1)][(gh[i].Gx)] == '#') && !(map[(gh[i].Gy - 1)][(gh[i].Gx - 1)] == '#' || map[(gh[i].Gy - 1)][(gh[i].Gx - 2)] == '#' || map[(gh[i].Gy - 1)][(gh[i].Gx + 1)] == '#' || map[(gh[i].Gy - 1)][(gh[i].Gx + 2)] == '#'))
        {
            gh[i].Gy -= gh[i].Vgy;
        }
        else
        {
            ghost_dir[i] = ghost_direction[rand() % 4];
        }
        break;
    case 'D':
        if (!(map[(gh[i].Gy + 1)][(gh[i].Gx)] == '#') && !(map[(gh[i].Gy + 1)][(gh[i].Gx - 1)] == '#' || map[(gh[i].Gy + 1)][(gh[i].Gx - 2)] == '#' || map[(gh[i].Gy + 1)][(gh[i].Gx + 1)] == '#' || map[(gh[i].Gy + 1)][(gh[i].Gx + 2)] == '#'))
        {
            gh[i].Gy += gh[i].Vgy;
        }
        else 
        {
            ghost_dir[i] = ghost_direction[rand() % 4];
        }
        break;
    default:
        ghost_dir[i] = ghost_direction[rand() % 4];
        break;
    }
    
}

void start_screen()
{
    printw("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printw("\t\t\t-------       -        --------   -       -        -       --      -\n");
    printw("\t\t\t-     -      - -       -          - -   - -       - -      - -     -\n");
    printw("\t\t\t-------     -   -      -          -  - -  -      -   -     -  -    -\n");
    printw("\t\t\t-          -  -  -     -          -   -   -     -  -  -    -   -   -\n");
    printw("\t\t\t-         -       -    -          -       -    -       -   -    -  -\n");
    printw("\t\t\t-        -         -   --------   -       -   -         -  -      --\n");
    refresh();
}

void collision(){
    for(int i = 0;i < 4; i++){
        if(gh[i].Gx == pac.Px && gh[i].Gy == pac.Py){
            GameStatus = 0;
            break;
        }
    }
    if(GameStatus==0){
        sleep(2);
        clear();
        printw("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\tYOU LOST!");
        refresh();
    }
}

void food_empty(){
    int check = 0;
    for (int i = 0; i <= 32; i++)
    {
        for (int j = 0; j <= 72; j++){
            if(map[i][j]=='-'){
                check = 1;
                break;
            }
        }
        if(check == 1){
            break;
        }
    }
    if(check == 0){
        GameStatus = 0;
        sleep(2);
        clear();
        printw("CONGRATULATIONS! YOU WON");
        refresh();
        sleep(2);
        exit(1);
    }
}
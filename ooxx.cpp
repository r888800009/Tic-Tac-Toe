#include <iostream>
#include <cstdlib>
#include <ctime>

/*
 * display code
 * credit BTea
 */

#define AI_MODE_NOLOST      1
#define AI_MODE_WINFIRST    2
#define AI_MODE_DEAD        3

using namespace std;
char grid[9];
int num = 0;
const char charO = 'o', charX = 'x';

const int checkWinList[8][3] =
{
    {0, 1, 2}, {3, 4, 5},
    {6, 7, 8}, {0, 3, 6},
    {1, 4, 7}, {2, 5, 8},
    {0, 4, 8}, {2, 4, 6},
};

int matchOX (char grid[], char OX, int i)
{
    bool match = true;
    for (int j = 0; j < 3; j++) {
        if (grid[checkWinList[i][j]] != OX) {
            match = false;
            break;
        }
    }
    return match;
}

int checkWin (char grid[])
{
    for (int i = 0; i < 8; i++) {
        if (matchOX(grid, charO, i))
            return charO;

        if (matchOX(grid, charX, i))
            return charX;
    }
    return 0;
}

void display ()
{
    cout << "     |     |" << endl;
    cout << "  "<< grid[0] << "  |  " << grid[1] << "  |  " << grid[2] <<endl;
    cout << "_____|_____|_____" << endl;
    cout << "     |     |" << endl;
    cout << "  "<< grid[3] << "  |  " << grid[4] << "  |  " << grid[5] <<endl;
    cout << "_____|_____|_____" << endl;
    cout << "     |     |" << endl;
    cout << "  "<< grid[6] << "  |  " << grid[7] << "  |  " << grid[8] <<endl;
    cout << "     |     |" << endl;
}

void putOX (char OX, int cho)
{
    if (cho >= 0 && cho < 9)
        if (grid[cho] == ' ') {
            grid[cho] = OX;
            num++;
        }
}

int gridToStatus (unsigned char *opt, char *ipt)
{
    int counter = 0;
    for (int i = 0; i < 9; i++) {
        if (ipt[i] == ' ')
            opt[counter++] = i;
    }
    return counter;
}

struct probability
{
     float win, lost, draw;
};

int computerCho (char grid[], struct probability &P, int aiMode, int who)
{
    unsigned char status[9];
    int size = gridToStatus(status, grid);
    int win, resultIndex = status[0];
    struct probability bestP = {-1};

    win = checkWin(grid);

    if (win != 0) {
        if(win == who)
            P.win  = 1;
        else
            P.lost = 1;
        return -1;
    }

    if (!size) {
        P.draw = 1;
        return -1;
    }

    for (int i = 0, id; i < size; i++) {
        id = status[i];
        grid[id] = (size & 1) ? charO : charX;

        struct probability retP = {0};
        computerCho(grid, retP, aiMode, who);
        P.win  += retP.win  / size;
        P.lost += retP.lost / size;
        P.draw += retP.draw / size;

        float best, current;
        switch (aiMode) {
            case AI_MODE_WINFIRST:
                best    = bestP.win;
                current = retP.win;
                break;
            case AI_MODE_NOLOST:
                best    = bestP.win + bestP.draw;
                current = retP.win + retP.draw;
                break;
            case AI_MODE_DEAD:
                best    = bestP.lost;
                current = retP.lost;
        }

        if (best < current) {
            resultIndex = id;
            bestP = retP;
        }

        grid[id] = ' ';
    }

    return resultIndex;
}

int main(void)
{
    int win = 0;
    srand(time(NULL));
    for(int i = 0; i < 9; i++)
        grid[i] = ' ';

    do {
        display();
        int numPre = num;
        while (num == numPre) {
            if (!(num & 1)) {
                //computer
                struct probability P = {0};
                int id;
                putOX(charO, id = computerCho(grid, P, AI_MODE_NOLOST, charO));
                cout << id << endl;
                cout << P.draw << endl;
                cout << P.win << endl;
                cout << P.lost << endl;

            } else {
                //player

                int cho;
                cin >> cho;
                putOX(charX, cho);
                /*
                struct probability P = {0};
                putOX(charO, computerCho(grid, P, AI_MODE_DEAD, charO));
                */
            }
        }
    } while (num < 8 && !(win = checkWin(grid)));

    if (num == 8 && !win) {
        unsigned char status[1];
        gridToStatus(status, grid);
        grid[status[0]] = charO;
        win = checkWin(grid);
    }

    display();
    if (!win)
        cout << "Draw~";
    if (win == charO)
        cout << "You Win!!";
    if (win == charX)
        cout << "You lose...";
    cout << endl;

    return 0;
}

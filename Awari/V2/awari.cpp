#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <map>
#include "mcts.h"
#include "awari.h"
using namespace std;

void Game_MCTS(bool computerFirst = true)
{
    Awari Game;
    Game.Show();

    MCTS<Awari> MC(Game.toString(), 0.8, 10000);

    bool flag = computerFirst;
    bool Error = false;
    while (Game.GameOn() && !Error)
    {
        string Move = "#";
        MC.ShowNextMoves();
        bool Changed = false;
        if (flag)
        {
            printf("Computer Thinking...  \t\t");
            Move = MC.nextMove();
            if (Move != "#")
            {
                Changed = Game.Play(Move);
            }
            else
            {
                printf("Computer failed\n");
                Error = true;
            }
        }
        else
        {
            cin >> Move;
            if (Move == "exit")
                break;
            Changed = Game.Play(Move);
        }

        if (Changed)
        {
            printf("%s\n", Move.c_str());
            Game.Show();
            if (MC.Play(Move))
            {
                //printf("SimCount=%d\n", MC.SimCount);
            }
            else
            {
                //No Moves Left
                printf("Play Error\n");
                Error = true;
                break;
            }
            flag = !flag;
        }
        else
        {
            if (flag)
            {
                Error = true;
            }
            printf("Invalid Move! Please Try Again!\n");
        }
    }
    if (Game.GameOn())
    {
        if (Error)
        {
            printf("Error : no result\n");
        }
        else
        {
            printf("User Interrupt\n");
        }
        return;
    }
    printf("\n\n---------- Game Over ------------\n");
    //printf("%s\n", game.History().c_str());
    Game.Show();
    int S = Game.getState();
    if (S == 2)
    {
        printf("Draw!\n");
    }
    else if ((S == 0 && computerFirst) || (S == 1 && !computerFirst))
    {
        printf("Computer Wins!\n");
    }
    else
    {
        printf("You win!\n");
    }
}

int main()
{
    srand(time(NULL));
    Game_MCTS();
    //Awari Game;
    //Game.Show();
    //cout << Game.RandomSimulation() << endl;
    return 0;
}
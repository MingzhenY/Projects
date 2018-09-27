#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <map>
#include "mcts.h"
#include "mancala.h"
#include "gametest.h"
using namespace std;

void Game_MCTS(bool computerFirst = true)
{
    Mancala Game;
    Game.Show();

    MCTS<Mancala> MC(Game.toString(), 0.4, 100000);

    bool flag = computerFirst;
    bool Error = false;
    while (Game.GameOn() && !Error)
    {
        string Move = "#";
        //MC.ShowNextMoves();
        bool Changed = false;
        if (flag)
        {
            printf("Computer Thinking...  \t\t\n");
            Move = MC.nextMove();
            MC.ShowNextMoves();
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
            if (Move == "exit" || Move == "EXIT")
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
            if (Move[Move.length() - 1] != '|')
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
            printf("History: %s\n", Game.History().c_str());
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
    GameTest<Mancala> Test;
    Test.TestAll(false);
    if (Test.TestAll(false))
    {
        Game_MCTS();
    }
    return 0;
}
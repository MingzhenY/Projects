#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include "mcts.h"
#include "gametest.h"
#include "gameplay.h"
#include "games.h"
using namespace std;

int main()
{
    GameTest<TicTacToe> Test;
    if (Test.TestAll(true))
    {
        GamePlay<TicTacToe> Game;
        Game.Game_MCTS();
    }
    return 0;
}
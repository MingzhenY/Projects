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
    GameTest<Awari> Test;
    if (Test.Test_SingleGame(true))
    {
        GamePlay<Awari> Game;
        Game.Single_MCTS(true, 1.0, 10000);
    }
    return 0;
}
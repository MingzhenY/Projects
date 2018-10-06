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
    GameTest<Adi> Test;
    if (Test.TestAll(true))
    {
        GamePlay<Adi> Game;
        Game.Game_MCTS(true, 1.0, 10000);
    }
    return 0;
}
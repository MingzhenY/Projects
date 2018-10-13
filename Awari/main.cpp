#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include "mcts.h"
#include "gametest.h"
#include "gameplay.h"
#include "games.h"
using namespace std;
#define TEST_SINGLE_GAME(name)              \
    GameTest<name> Test_##name;             \
    if (Test_##name.Test_SingleGame(false)) \
        printf("pass");                     \
    else                                    \
        printf("fail");                     \
    printf(" ... %s\n", #name);

void TestAllGames()
{
    printf("Testing Games:\n");
    TEST_SINGLE_GAME(TicTacToe)
    TEST_SINGLE_GAME(Adi)
    TEST_SINGLE_GAME(Awari)
    TEST_SINGLE_GAME(Qelat)
    TEST_SINGLE_GAME(Bechi)
    printf("\n\n");
}
int main()
{
    TestAllGames();
    GameTest<Bechi> Test;
    //Test.Test_BasicGamePlay(10000, true);
    //Test.ShowLog();
    //return 0;
    if (Test.Test_SingleGame(false))
    {
        GamePlay<Bechi> Game;
        Game.Single_MCTS(true, 1.0, 10000);
    }
    return 0;
}
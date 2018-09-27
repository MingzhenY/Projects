#include <iostream>
#include <vector>
#include <ctime>

/*
Here defines the code to test for all the 
requirements so that the class Game works 
well with mcts.h and gameplay.h
*/
template <class Game>
class GameTest
{

    std::vector<std::string> LogVec;
    void Log(std::string test, bool pass, std::string msg)
    {
        LogVec.push_back(test + " - " + (pass ? "pass" : "  fail") + "   " + msg);
    }

  public:
    GameTest()
    {
        srand(time(NULL));
    }
    void ShowLog()
    {
        for (auto log : LogVec)
        {
            printf("%s\n", log.c_str());
        }
    }
    /*
    Test_BasicGamePlay:
    Requirements:
    1. Game():
        initialize the game.
        player 0 goes first.
    2. Game.GameOn():
        returns true if the game is still going on,
        false otherwise.

    3. Game.ValidMovesVec():
        returns a vector of strings.
        each string represent a move the current player can make.

    4. Game.Play():
        plays a move.
        return true if successful, false otherwise.

    Expectations:
    0. Game() initializes the game.
    1. GameOn() returns true if the game is still on.
    2. GameOn() should return true after initialization.
    3. ValidMovesVec() will return a vector of (all) valid moves.
    4. ValidMovesVec() will return a non-empty vector if GameOn() is true.
    5. ValidMovesVec() will return a empty vector if GameOn() is false.
    6. Play() should return true for any valid move.
    7. A Game that plays randomly will always terminate.
    */
    bool Test_BasicGamePlay(int T = 100)
    {
        std::string msg;
        bool pass = true;
        for (int t = 0; t < T && pass; ++t)
        {
            //check for E0
            Game game;
            //check for E2
            if (!game.GameOn())
            {
                msg = "GameOn() returns false after Game()";
                pass = false;
                break;
            }
            //check for E1
            while (game.GameOn() && pass)
            {
                //check for E3
                std::vector<std::string> Moves = game.ValidMovesVec();
                int N = Moves.size();
                //check for E4
                if (!N)
                {
                    msg = "ValidMovesVec() returns empty vector when GameOn() is true";
                    pass = false;
                    break;
                }
                //check for E6
                bool success = game.Play(Moves[rand() % N]);
                if (!success)
                {
                    msg = "Play() returns false for a valid move";
                    pass = false;
                    break;
                }
            }
            //check for E5
            if (!game.GameOn() && pass)
            {
                std::vector<std::string> Moves = game.ValidMovesVec();
                if (!Moves.empty())
                {
                    msg = "ValidMovesVec() returns non-empty when GameOn() is false";
                    pass = false;
                }
            }
        }

        //check for E7 (E7 fail => endless loop)
        Log("Test_BasicGamePlay", pass, msg);
        return pass;
    }

    bool TestAll(bool showMsg = false)
    {
        bool pass = true;
        pass = pass && Test_BasicGamePlay();
        if (showMsg)
        {
            ShowLog();
            printf("\n\t%s\n\n", pass ? "Passed All Tests" : "Failed Some Tests");
        }
        return pass;
    }
};
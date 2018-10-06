#include <iostream>
#include <vector>
#include <ctime>

/*
Here defines the code to test for all the 
requirements so that the class Game works 
well with mcts.h and gameplay.h

Basic rules:
1.It's a two player perfect information game.
2.Player 0 always goes first.
3.The game board is represented by std::string.
    This should include which player moves next.
4.A move of the game is represented by std::string.
    The first byte of a move is used (as a boolean) 
    to indicate if the player to move has changed.

Below are requirements from different sources:
Rules from mcts.h:
1.Game.Game() initializes the game.
2.Game.Game(gameBoard) initializes the game.
3.Game.ValidMoves() returns valid moves as a vector of string.
4.Game.Play(move) plays the move and returns if successful.
    Also updates move (so that it matches ValidMoves()).
5.Game.IfPlay(move) returns the new board after playing the move.
    or "#" if the move is incorrect.
    This does not change the state of Game.
    Does not update move.
6.Game.State() returns
    -1: game on
    0 : player 0 wins
    1 : player 1 wins
    2 : draw
7.Game.GameOn() returns if the game is still going on.
8.Game.Player() returns the next player to move.

Rules from gameplay.h:
1.Game.Board() returns the game board.
2.Game.Show() prints the game board
3.Game.History() returns a string of move history

Rules from class Game itself:
1.UpdateState()
    updating state according to the game board.
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
    2. Player()
        returns the player to move.
        player 0 goes first.
    3. Game.GameOn():
        returns true if the game is still going on,
        false otherwise.
    4. Game.ValidMoves():
        returns a vector of strings.
        each string represent a move the current player can make.
    5. Game.Play():
        plays a move.
        return true if successful, false otherwise.
    */
    bool Test_BasicGamePlay(int T = 100)
    {
        std::string msg;
        bool pass = true;
        for (int t = 0; t < T && pass; ++t)
        {
            Game game;
            if (game.Player())
            {
                msg = "Player() returns true after Game()";
                pass = false;
                break;
            }
            if (!game.GameOn())
            {
                msg = "GameOn() returns false after Game()";
                pass = false;
                break;
            }
            while (game.GameOn() && pass)
            {
                std::vector<std::string> Moves = game.ValidMoves();
                int N = Moves.size();
                if (!N)
                {
                    msg = "ValidMoves() returns empty vector when GameOn() is true";
                    pass = false;
                    break;
                }
                bool success = game.Play(Moves[rand() % N]);
                if (!success)
                {
                    msg = "Play() returns false for a valid move";
                    pass = false;
                    break;
                }
            }
            if (!game.GameOn() && pass)
            {
                std::vector<std::string> Moves = game.ValidMoves();
                if (!Moves.empty())
                {
                    msg = "ValidMoves() returns non-empty when GameOn() is false";
                    pass = false;
                }
            }
        }
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
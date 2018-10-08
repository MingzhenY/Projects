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
    to indicate if the player's turn has ended.
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
    6. Game.State():
        returns -1,0,1 or 2
    */
    bool Test_BasicGamePlay(int T = 100, bool show = false)
    {
        std::string msg;
        bool pass = true;
        for (int t = 0; t < T && pass; ++t)
        {
            if (show && (t % 1000 == 0))
                printf("t=%d\n", t);
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
            int Count = 0;
            while (game.GameOn() && pass)
            {
                if (Count++ >= 1000)
                {
                    if (show)
                        game.Show();
                    msg = "Game takes too long";
                    pass = false;
                    break;
                }
                std::vector<std::string> Moves = game.ValidMoves();
                int N = Moves.size();
                if (!N)
                {
                    if (show)
                        game.Show();
                    msg = "ValidMoves() returns empty vector when GameOn() is true";
                    pass = false;
                    break;
                }
                //game.Show();
                bool success = game.Play(Moves[rand() % N]);
                if (!success)
                {
                    msg = "Play() returns false for a valid move";
                    pass = false;
                    break;
                }
            }
            if (!pass)
                break;
            if (!game.GameOn())
            {
                std::vector<std::string> Moves = game.ValidMoves();
                if (!Moves.empty())
                {
                    msg = "ValidMoves() returns non-empty when GameOn() is false";
                    pass = false;
                    break;
                }
            }
            if (game.State() >= 0 && game.State() <= 2)
            {
                //This is normal, do nothing
            }
            else
            {
                if (game.State() == -1)
                    msg = "Game Ends with a state of -1";
                else
                    msg = "Game Ends with invalid state";
                pass = false;
                break;
            }
        }
        Log("Test_BasicGamePlay", pass, msg);
        return pass;
    }
    /*
    Test_MultipleRoundGamePlay:
    1.Game.Reset()
        use the result of last game to
        initialize this  game.
    2.Game.Result()
        returns the result
        passed to Reset()
    3.Game.GameOn()
    4.Game.ValidMoves()
    5.Game.Play()
    6.Should also consider the change of palyer.
    
    */
    bool Test_MultipleRoundGamePlay(int T = 100)
    {
        std::string msg;
        std::string result;
        bool pass = true;
        Game game;
        for (int t = 0; t < T && pass; ++t)
        {
            game.Reset(result);
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
            result = game.Result();
        }
        Log("Test_MultipleRoundGamePlay", pass, msg);
        return pass;
    }
    /*
    Test_IfPlay():
    1.Game.IfPlay(move)
        Plays the move without updating the board.
        returns the updated board
    2.Game.Game(Board)
        Use Board to initialize game
    3.Game.Board()
        returns the game board
    4.Game.MoveHistory()
        returns the move history
    5.Game.Play()
    6.Game.GameOn()
    7.Game.ValidMoves()
    */
    bool Test_IfPlay(int T = 100)
    {
        std::string msg;
        bool pass = true;
        Game A, B;
        for (int t = 0; t < T && pass; t++)
        {
            while (A.GameOn() && pass)
            {
                std::vector<std::string> Moves = A.ValidMoves();
                int N = Moves.size();
                if (!N)
                {
                    msg = "ValidMoves() returns empty vector when GameOn() is true";
                    pass = false;
                    break;
                }
                //Let B be a clone of A
                B = Game(A.Board());

                //choose a move for A
                std::string move = Moves[rand() % N];
                std::string HistoryBeforePlay = A.History();
                bool success = A.Play(move);
                if (!success)
                {
                    msg = "Play() returns false for a valid move";
                    pass = false;
                    break;
                }
                //Test MoveHistory()
                if (A.History().length() <= HistoryBeforePlay.length())
                {
                    msg = "History() does not grow after play";
                    pass = false;
                    break;
                }
                //Test IfPlay()
                std::string old_Board = B.Board();
                if (B.IfPlay(move) != A.Board())
                {
                    msg = "IfPlay() returns different game board than Play()";
                    pass = false;
                    break;
                }
                if (B.Board() != old_Board)
                {
                    msg = "IfPlay() changes game board";
                    pass = false;
                    break;
                }
            }
        }
        Log("Test_IfPlay", pass, msg);
        return pass;
    }
    bool Test_SingleGame(bool showMsg = false)
    {
        bool pass = true;
        pass = pass && Test_BasicGamePlay();
        pass = pass && Test_IfPlay();
        if (showMsg)
        {
            ShowLog();
            printf("\n\t%s\n\n", pass ? "Passed All Tests" : "Failed Some Tests");
        }
        return pass;
    }
};
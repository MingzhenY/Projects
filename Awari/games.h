#include <iostream>
#include <vector>
/*
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
    Also updates move (so that it matches ValidMoves()).
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
*/
class TicTacToe
{
    std::string board;
    std::string moveHistory;

  public:
    TicTacToe()
    {
        board = std::string(11, '.');
        board[9] = false;
        board[10] = -1;
        moveHistory = "#";
    }
    TicTacToe(std::string _Board)
    {
        board = _Board;
        moveHistory = "";
    }
    std::vector<std::string> ValidMoves()
    {
        int state = board[10];
        //A end game has no valid moves
        if (!GameOn())
            return {};
        std::vector<std::string> ret;
        for (int i = 0; i < 9; ++i)
        {
            if (board[i] == '.')
            {
                ret.push_back(std::string("#") + char('A' + i));
            }
        }
        return ret;
    }
    bool Play(std::string &Move)
    {
        bool player = board[9];
        if (Move.length() > 1 &&
            Move[1] >= 'A' && Move[1] <= 'I' &&
            board[Move[1] - 'A'] == '.')
        {
            board[Move[1] - 'A'] = player ? 'X' : 'O';
            moveHistory += Move[1];
            player = !player;
            board[9] = !board[9];
            UpdateState();
            return true;
        }
        return false;
    }
    std::string IfPlay(std::string &Move)
    {
        TicTacToe TTT(board);
        TTT.Play(Move);
        return TTT.Board();
    }
    void UpdateState()
    {
        if (!GameOn())
            return;
        int positions[8][3] = {
            {0, 1, 2},
            {3, 4, 5},
            {6, 7, 8},
            {0, 3, 6},
            {1, 4, 7},
            {2, 5, 8},
            {0, 4, 8},
            {2, 4, 6}};
        int CountDot = 0;
        for (int i = 0; i < 9; ++i)
            CountDot += board[i] == '.';
        if (CountDot == 0)
        {
            board[10] = 2;
            return;
        }
        for (int k = 0; k < 8; ++k)
        {
            if (board[positions[k][0]] != '.' &&
                board[positions[k][0]] == board[positions[k][1]] &&
                board[positions[k][1]] == board[positions[k][2]])
            {
                board[10] = board[positions[k][0]] == 'X';
            }
        }
    }
    bool Player()
    {
        return board[9];
    }
    int State()
    {
        return board[10];
    }
    bool GameOn()
    {
        return State() == -1;
    }
    std::string Board()
    {
        return board;
    }
    std::string History()
    {
        return moveHistory;
    }
    void Show()
    {
        printf("%c|%c|%c\n", board[0], board[1], board[2]);
        printf("-+-+-\n");
        printf("%c|%c|%c\n", board[3], board[4], board[5]);
        printf("-+-+-\n");
        printf("%c|%c|%c\n\n", board[6], board[7], board[8]);
    }
};
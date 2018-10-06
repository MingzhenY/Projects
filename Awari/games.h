#include <iostream>
#include <vector>
/*
All the mancala games come from "The Complete mancala games Book" by Larry Russ
This book will be refered to as "the book" in the rest of this file.
There are many different rules played in diffrent part of the world.
I will implement most interesting ones here.


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
4.Game.Result() returns a string of result.
    This is needed because the result of some of the games
    will affect how the next games are played.
5.Game.Reset(result)
    Set up the game according to result.
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
        moveHistory = "";
    }
    TicTacToe(std::string _Board)
    {
        board = _Board;
        moveHistory = "";
    }
    void Reset(std::string _Result)
    { //Past results have no effect on TicTactoe
        //The same as TicTacToe()
        board = std::string(11, '.');
        board[9] = false;
        board[10] = -1;
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
    std::string Result()
    {
        //Past results have no effect on TicTactoe
        //So return nothing here
        return "";
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
/*
Adi(From page 59-61 of the book)

Game Board:
        f   e   d   c   b   a    
    12  5   4   3   2   1   0    
        6   7   8   9  10  11  13
        A   B   C   D   E   F    

Rules:
1.Player 0 controls pit 0 to 5, and uses 12 to store captured seeds.
    Player 1 controls pit 6 to 11, and uses 13 to store captured seeds.
2.At the start of the game, 
    there are 4 seeds in each of the pits(except for 12 and 13).
3.In a move(called sow), the player chooses a non-empty pit that the player 
    controls and pickes up all the seeds inside.
    Then starting from the next pit, moving counterclockwise, put one seed
    in each of the pit(skipping pit 12 and 13). 
    As long as the last seed doesn't make a 1 or 4, the player picks up 
    all the seeds in this pit and continue sowing.

    If the last seed makes a 1, the player stops.
    If the last seed makes a 4, the player takes all 4 seeds and 
    stores them in pit 12 or 13 depending on the player.
4.As play proceeds, some of the pits will accumulate 4 seeds.
    Each player, even during his opponents's turn, immediately captures
    the groups of 4 that appears on the pits the player controls(If they
    changes from 3 to 4),except the 4 formed by the last seed that according
    to rule 2 belongs to the player that makes the move.

    If a player neglects to capture a four that has accumulated and a fifth
    seed is placed on top of it, the player can no longer capture the 4.
5.If a player cannot move, the player passes the turn.
6.When there are 8 seeds left, the player who captures the next 4 also
    gets the last 4.
7.When the board is empty, the player with more seed, start filling up
    each pit with 4 seeds starting from the leftmost pit, counterclockwise.
    All the pits the player fills this way(even the pits previously 
    controled by the opponent) is controled by the player in the 
    next game. The rest of the pits are controlled by the player with less
    seed.
    In the case where the player with more seed has a seed number that is
    not a multiple of 4. Three seeds will get another pit, two or less will not.
    The player that controls less pits moves first.
8.The game ends when a player owns all 12 pits.
9.If a move goes into an infinity loop, the seeds are split evenly between players.
*/
class Adi
{
#define PLAYER(board) board[14]
#define STATE(board) board[15]
#define OWNER(board) (board[16] << 6) | board[17]
    std::string board;
    int owner; //a bitmap of owner
    std::string moveHistory;

  public:
    Adi()
    {
        Init();
    }
    Adi(std::string Board)
    {
        Init(Board);
    }
    void Init()
    {
        board = std::string(18, char(4));
        board[12] = board[13] = 0;
        PLAYER(board) = false;
        STATE(board) = -1;
        owner = 63 << 6;
        board[16] = owner >> 6;
        board[17] = owner & 63;
    }
    void Init(std::string Board)
    {
        board = Board;
        owner = OWNER(Board);
    }
    void Reset(std::string result)
    {
        board = std::string(18, char(4));
        board[12] = board[13] = 0;
        PLAYER(board) = result[0] > result[1];
        STATE(board) = -1;
        //Determine territory
        bool PlayerWithMoreSeeds = result[0] >= result[1];
        int current_pit = PlayerWithMoreSeeds ? 6 : 0;
        int current_seed = result[PlayerWithMoreSeeds];
        if (PlayerWithMoreSeeds)
            owner = 0;
        else
            owner = (1 << 12) - 1;

        while (current_seed >= 4)
        {
            //mark current pit
            owner = owner | (PlayerWithMoreSeeds << current_pit);
            current_seed -= 4;
            current_pit = (current_pit + 1) % 12;
        }
        board[16] = owner >> 6;
        board[17] = owner & 63;
    }
    std::vector<std::string> ValidMoves()
    {
        if (!GameOn())
            return {};
        std::vector<std::string> ret;
        for (int k = 0; k < 12; ++k)
        {
            if (!(owner & (1 << k)) == !PLAYER(board) &&
                board[k] != 0)
            {
                ret.push_back(std::string("#") + char(k < 6 ? k + 'a' : k + 'A' - 6));
            }
        }
        //use "#" to indicate passing the turn
        if (ret.empty() && GameOn())
            return {"#"};
        else
            return ret;
    }
    bool Play(std::string &Move)
    {
        if (Move.length() < 2)
        {
            if (Move == "#")
            {
                PLAYER(board) = !PLAYER(board);
                moveHistory += "#";
                return true;
            }
            else
                return false;
        }
        if ((Move[1] >= 'a' && Move[1] <= 'f') ||
            (Move[1] >= 'A' && Move[1] <= 'F'))
        {
            int pitn = Move[1] >= 'a' ? Move[1] - 'a'
                                      : Move[1] - 'A' + 6;
            if (!(owner & (1 << pitn)) == !PLAYER(board) &&
                board[pitn] != 0)
            {
                //Move is valid
                moveHistory += Move[1];
                int Sow = board[pitn], Capture = 0;
                int Laps = 0;
                do
                {
                    int sow = board[pitn], seedLeft = sow;
                    board[pitn] = 0;
                    while (seedLeft-- > 0)
                    {
                        pitn = (pitn + 1) % 12;
                        board[pitn]++;
                        if (board[pitn] == 4 && seedLeft > 0)
                        {
                            //the owner of this pit takes 4
                            bool pitOwner = (owner >> pitn) & 1;
                            board[12 + pitOwner] += board[pitn];
                            board[pitn] = 0;
                            //check for Rule 6
                            if (board[12] + board[13] == 44)
                            {
                                board[12 + pitOwner] += 4;
                                for (int k = 0; k < 12; ++k)
                                    board[k] = 0;
                            }
                        }
                    }
                } while (board[pitn] != 1 && board[pitn] != 4 && ++Laps < 100);
                if (Laps >= 100)
                {
                    //Caught in a loop
                    //split the remaining stones.
                    int Sum = 0;
                    for (int k = 0; k < 12; ++k)
                    {
                        Sum += board[k];
                        board[k] = 0;
                    }
                    board[12] += Sum / 2;
                    board[13] += Sum / 2;
                }
                else
                {
                    //Capture
                    if (board[pitn] == 4)
                    {
                        Capture = board[pitn];
                        board[pitn] = 0;
                        //Rule 6
                        if (board[12] + board[13] == 40)
                        {
                            Capture += 4;
                            for (int k = 0; k < 12; ++k)
                                board[k] = 0;
                        }
                        board[12 + PLAYER(board)] += Capture;
                    }
                }
                PLAYER(board) = !PLAYER(board);
                UpdateState();
                Move = Move.substr(0, 2) + std::to_string(Sow);
                return true;
            }
            else
                return false;
        }
        return false;
    }
    std::string IfPlay(std::string &Move)
    {
        Adi game(board);
        game.Play(Move);
        return game.Board();
    }
    void UpdateState()
    {
        if (GameOn())
        {
            if (board[12] + board[13] == 48)
            {
                STATE(board) = board[12] == board[13] ? 2
                                                      : board[13] > board[12];
            }
        }
    }
    int State()
    {
        return STATE(board);
    }
    bool GameOn()
    {
        return State() == -1;
    }
    bool Player()
    {
        return PLAYER(board);
    }
    std::string Board()
    {
        return board;
    }
    void Show()
    {
        printf("      f   e   d   c   b   a\n");
        printf("    ");
        for (int k = 5; k >= 0; --k)
        {
            if (owner & (1 << k))
                printf("--- ");
            else
                printf("    ");
        }
        printf("\n%3d %3d %3d %3d %3d %3d %3d\n", board[12], board[5], board[4], board[3], board[2], board[1], board[0]);
        printf("    %3d %3d %3d %3d %3d %3d %3d\n", board[6], board[7], board[8], board[9], board[10], board[11], board[13]);
        printf("    ");
        for (int k = 6; k <= 11; k++)
        {
            if (owner & (1 << k))
                printf("--- ");
            else
                printf("    ");
        }
        printf("\n      A   B   C   D   E   F\n\n");
        printf("--------------------------------\n\n\n");
    }
    std::string History()
    {
        return moveHistory;
    }
    std::string Result()
    {
        return board.substr(12, 2);
    }
#undef PLAYER
#undef STATE
#undef OWNER
};
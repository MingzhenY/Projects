#include <iostream>
#include <vector>
/*
All the mancala games come from "The Complete mancala games Book" by Larry Russ
This book will be refered to as "the book" in the rest of this file.
There are many different rules played in diffrent part of the world.
I will implement interesting ones here.


Below are requirements for the Game class

To pass GameTest.Test_SimgleGame():
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
9.Game.Halt() terminates the game early and 
    estimates the outcome.
10.Game.Board() returns the game board.
11.Game.Show() prints the game board
12.Game.History() returns a string of move history

To pass GameTest.Test_MultipleGame():
1.Pass Test_SimgleGame()
2.More
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
        if (Move.length() < 2)
            return false;
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
        if (TTT.Play(Move))
            return TTT.Board();
        else
            return "#";
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
    void Halt()
    {
        //assume draw
        board[10] = 2;
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
        if (game.Play(Move))
            return game.Board();
        else
            return "#";
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
    void Halt()
    {
        if (GameOn())
        {
            STATE(board) = board[12] == board[13] ? 2
                                                  : board[13] > board[12];
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
#undef PLAYER
#undef STATE
#undef OWNER
};

/*
Awari
    Game Board:
        f   e   d   c   b   a    
    12  5   4   3   2   1   0    
        6   7   8   9  10  11  13
        A   B   C   D   E   F    
    
    Player 0 controls pits 0 to 5 and pit 13. pit 13 is used to store captured stones.
    Player 1 controls pits 6 to 12 where pit 12 is used to store captured stones.
    pits 0 to 5 are called a to f.
    pits 6 to 11 are called A to F.

    The rules I used here comes from the paper "Searching for Solutions 
    in Games and Artificial Intelligence" by Louis Victor Allis

    Awari Rules(one of the versions):
    1. At the start of the game, each pit(except for 12 and 13) has 4 stones.
    2. At each step a player selects a non-empty pit X from his/her row, starting 
        with X's neighbor, he/she then sows all stones from X, one at a time, 
        counter-clockwise over the board(omitting 12 and 13 and itself). After 
        the move, X will be empty even if X has 12 stones or more. Captured Stones 
        are removed and stored in either pit 12 or 13, depending on the player.
    3. Stones are captured if the last stone sown lands in an enemy pit which after 
        landing contains 2 or 3 stones. If such a capture is made, and the preceding 
        pit contains 2 or 3 stones and the pit is an enemy pit, those stones are also 
        captured. This procedure is successively repeated for the pits preceding and 
        ends as soon as a pit is encountered containing a number of stones other than 
        2 or 3, or the end of the opposing row is reached.
    4. A move is described by the name of the pit, followed by the number of stones sown
        (the name of the pit by itself defines the move, but such a notations is prone to error).
        The number of stones captured, if any, is indicated by the amount preceded by a "x".
        For example: A1, C4 x 2, D19 x 7, ...
    5. End of the game:
    5.1. The Goal of awari is to capture more stones than the opponent. The game ends 
        as soon as one of the player has collected 25 or more stones.
    5.2. If a player is unable to move, the remaining stones are captured by the opponent. 
    5.3. If the same position is encountered for the third time, with the same player to move, 
        the remaining stones on the board are evenly devided among the players.
    6. The winner is the player who captured the most stones. If both players capture 24 stones, 
        the game is drawn.
    7. A last rule exists to prevent players from running out of moves early in the game. Whenever 
        possible, a player is forced to choose a move such that the opponent is able to make a reply 
        move. It is, however, not compulsory to look several moves ahead to ensure that the opponent 
        will continue to be able to reply. 
*/

class Awari
{
#define PLAYER(board) board[14]
#define STATE(board) board[15]
    std::string board;
    std::string moveHistory;

    int validCount;
    bool isMoveValid[6];
    std::string nextMove[6];
    std::string nextBoard[6];

    int StepCount;

  public:
    Awari()
    {
        Init();
    }
    Awari(std::string Board)
    {
        Init(Board);
    }
    void Init()
    {
        board = std::string(16, char(4));
        board[12] = board[13] = 0;
        PLAYER(board) = false;
        STATE(board) = -1;
        StepCount = 0;
        NextMoves();
    }
    void Init(std::string Board)
    {
        if (Board.length() == 16)
        {
            board = Board;
            StepCount = 0;
            NextMoves();
        }
        else
        {
            Init();
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
    void ClearTable(std::string &Board, bool even_split = false)
    {
        //Sum up the stones on the board
        int Sum = 0;
        for (int i = 0; i < 12; ++i)
        {
            Sum += Board[i];
            Board[i] = 0;
        }

        if (even_split)
        {
            //split evenly
            Board[12] += Sum / 2;
            Board[13] += Sum / 2;
            if (Sum & 1)
            {
                if (Board[12] > Board[13])
                    ++Board[12];
                else
                    ++Board[13];
            }
        }
        else
        {
            //If one player cannot reply at his turn
            //all the stones belong to the other player
            Board[13 - PLAYER(Board)] += Sum;
        }
    }
    void UpdateState(std::string &Board)
    {
        if (!GameOn())
            return;
        //if one cannot move, all remaining stones belong to opponent
        int Sum = 0;
        for (int base = PLAYER(Board) ? 6 : 0, k = 0; k < 6; ++k)
        {
            Sum += Board[base + k];
        }
        if (0 == Sum)
        {
            ClearTable(Board);
        }
        //check for winner
        if (Board[12] >= 25 || Board[13] >= 25)
        {
            //a player wins
            STATE(Board) = Board[13] >= 25;
        }
        else if (Board[12] + Board[13] == 48 &&
                 Board[12] == Board[13])
        {
            STATE(Board) = 2;
        }
    }
    void UpdateState()
    {
        UpdateState(board);
        if (!GameOn())
        {
            NextMoves();
        }
    }
    bool Play(std::string &Board, std::string &Move)
    {
        if (Move.length() < 2)
            return false;
        int pitn = -1;
        if (PLAYER(Board) && Move[1] >= 'A' && Move[1] <= 'F')
        {
            pitn = Move[1] - 'A' + 6;
        }
        if (!PLAYER(Board) && Move[1] >= 'a' && Move[1] <= 'f')
        {
            pitn = Move[1] - 'a';
        }
        if (-1 == pitn || 0 == Board[pitn])
        {
            //cannot play the move
            return false;
        }
        int sow = Board[pitn], capture = 0;
        //Sow
        int cnt = pitn, nstone = sow;
        Board[pitn] = 0;
        while (nstone-- > 0)
        {
            //move up one pit
            cnt = (cnt + 1) % 12;
            //skip self
            cnt += cnt == pitn;
            cnt %= 12;
            //put a stone
            Board[cnt]++;
        }
        //Capture
        int base = PLAYER(Board) ? 0 : 6;
        while ((Board[cnt] == 2 || Board[cnt] == 3) &&
               cnt >= base && cnt - base <= 5)
        {
            //capture
            capture += Board[cnt];
            Board[cnt] = 0;
            //move cnt backwards and check again
            cnt = (cnt + 11) % 12;
        }
        Board[12 + PLAYER(Board)] += capture;
        Move = Move.substr(0, 2) + std::to_string(sow) + "X" + std::to_string(capture);
        PLAYER(Board) = !PLAYER(Board);
        UpdateState(Board);
        return true;
    }
    void NextMoves()
    {
        //Clear everything when game ends
        if (!GameOn())
        {
            memset(isMoveValid, 0, sizeof(isMoveValid));
            validCount = 0;
            return;
        }
        bool canReply[6];
        int replyCount;
        replyCount = validCount = 0;
        //try all 6 moves
        std::vector<std::string> moves;
        if (PLAYER(board))
            moves = std::vector<std::string>{"A", "B", "C", "D", "E", "F"};
        else
            moves = std::vector<std::string>{"a", "b", "c", "d", "e", "f"};

        for (int k = moves.size() - 1; k >= 0; --k)
        {
            //store results in NextBoard[k]
            nextBoard[k] = board;
            nextMove[k] = std::string("#") + moves[k];
            isMoveValid[k] = Play(nextBoard[k], nextMove[k]);
            if (isMoveValid[k])
            {
                validCount++;
                canReply[k] = false;
                for (int base = PLAYER(board) ? 0 : 6, i = 0; i < 6; ++i)
                {
                    canReply[k] = canReply[k] || nextBoard[k][base + i] > 0;
                }
                replyCount += canReply[k];
            }
        }
        //check for rule 7
        if (replyCount == 0 || replyCount == validCount)
        {
            //if all can reply or all cannot reply
            //do nothing
        }
        else
        {
            //if some cannot reply
            //eliminate the moves where the opponent cannot reply
            validCount = 0;
            for (int k = 0; k < 6; ++k)
            {
                isMoveValid[k] = isMoveValid[k] && canReply[k];
                validCount += isMoveValid[k];
            }
        }
    }
    bool Play(std::string &Move)
    {
        if (Move.length() < 2)
            return false;
        for (int k = 0; k < 6; ++k)
        {
            if (isMoveValid[k] && nextMove[k][1] == Move[1])
            {
                board = nextBoard[k];
                //Move is updated here
                Move = nextMove[k];
                moveHistory += Move[1];
                //Clear Table if more than 1000 steps.
                if (++StepCount >= 1000)
                {
                    ClearTable(board, true);
                    UpdateState(board);
                }
                NextMoves();
                return true;
            }
        }
        return false;
    }
    std::string IfPlay(std::string &Move)
    {
        for (int k = 0; k < 6; ++k)
        {
            if (isMoveValid[k] && nextMove[k][1] == Move[1])
            {
                Move = nextMove[k];
                return nextBoard[k];
            }
        }
        return "#";
    }
    std::vector<std::string> ValidMoves()
    {
        if (!GameOn())
            return {};
        std::vector<std::string> ret;
        for (int k = 0; k < 6; ++k)
        {
            if (isMoveValid[k])
            {
                ret.push_back(nextMove[k]);
            }
        }
        return ret;
    }
    void Halt()
    {
        ClearTable(board, true);
        UpdateState(board);
    }
    std::string Board()
    {
        return board;
    }
    std::string History()
    {
        return moveHistory;
    }
    bool Player()
    {
        return PLAYER(board);
    }
    void Show()
    {
        printf("      f   e   d   c   b   a\n");
        printf("%3d %3d %3d %3d %3d %3d %3d\n", board[12], board[5], board[4], board[3], board[2], board[1], board[0]);
        printf("    %3d %3d %3d %3d %3d %3d %3d\n", board[6], board[7], board[8], board[9], board[10], board[11], board[13]);
        printf("      A   B   C   D   E   F\n\n");
        printf("--------------------------------\n\n\n");
    }
#undef PLAYER
#undef STATE
};

/*
Qelat(From page 19 of the book):
Game Board:
   <-  <-  <-   ->  ->  ->
    f   e   d   c   b   a    
    5   4   3   2   1   0    
    6   7   8   9  10  11 
    A   B   C   D   E   F    
   <-  <-  <-   ->  ->  ->

Player 0 controls pits 0 to 5.
Player 1 controls pits 6 to 11.
Pits 0 to 5 are called a to f.
Pits 6 to 11 are called A to F.

Rules:
1. At the start of the game, each pit has 4 stones.
2. At each step a player selects a non-empty pit X that is not marked(explained later)
    from his/her row, starting with X's neighbor, he/she then sows all 
    stones from X, one at a time, in a certain direction. 

    The direction depends on the starting pit.
    d,e,f,D,E,F : counterclockwise
    a,b,c,A,B,C : clockwise
3. A player can only mark certain pits.
    Player 0 can only mark a,f,A,B,E,F.
    Player 1 can only mark A,F,a,b,e,f.
    A pit is marked if the last stone is put in a pit with 3 stones(thus making a 4).
4. If a player has no move left, the player passes the turn.
5. The game ends when neither player can move or the game goes into an infinite loop.
    The winner of the game is the one that accumulated the greater number of stones 
    in his/her marked pits.

Board:
board[0..11] : actual board
board[12..15] : record marked pit.
    board[12..13]: if a pit is marked
    board[14..15]: who marked the pit
board[16] : player
board[17] : state of the game
*/

class Qelat
{
#define IS_MARKED(board, k) ((board[12 + ((k) >= 6)] >> ((k) % 6)) & 1)
#define OWNER_MARKED(board, k) ((board[14 + ((k) >= 6)] >> ((k) % 6)) & 1)
#define MARK(board, k, player)                                          \
    board[12 + ((k) >= 6)] = board[12 + ((k) >= 6)] | (1 << ((k) % 6)); \
    if (player)                                                         \
        board[14 + ((k) >= 6)] = board[14 + ((k) >= 6)] | (1 << ((k) % 6));
#define PLAYER(board) board[16]
#define STATE(board) board[17]
    std::string board;
    std::string moveHistory;

  public:
    Qelat()
    {
        Init();
    }
    void Init()
    {
        board = std::string(12, char(4)) + std::string(6, char(0));
        PLAYER(board) = false;
        STATE(board) = -1;
    }
    Qelat(std::string Board)
    {
        Init(Board);
    }
    void Init(std::string Board)
    {
        board = Board;
    }
    std::vector<std::string> ValidMoves()
    {
        if (!GameOn())
            return {};
        std::vector<std::string> ret;
        for (int base = PLAYER(board) ? 6 : 0, k = 0; k < 6; ++k)
        {
            if (board[base + k] && !IS_MARKED(board, base + k))
            {
                ret.push_back(std::string("#") + char(base ? 'A' + k : 'a' + k));
            }
        }
        if (ret.empty())
            ret.push_back(std::string("#"));
        return ret;
    }
    bool Play(std::string &Move)
    {
        if (Move.length() < 2)
        {
            //Pass the turn
            if (Move == "#")
            {
                moveHistory += "#";
                PLAYER(board) = !PLAYER(board);
                return true;
            }
            else
                return false;
        }
        int pitn = Move[1] >= 'a' ? Move[1] - 'a' : Move[1] - 'A' + 6;
        if (IS_MARKED(board, pitn))
        {
            return false;
        }
        int Sow, SeedsLeft;
        Sow = SeedsLeft = board[pitn];
        board[pitn] = 0;
        int direction = (pitn <= 2 || (pitn >= 6 && pitn <= 8)) ? 11 : 1;
        //Sow
        while (SeedsLeft-- > 0)
        {
            pitn = (pitn + direction) % 12;
            board[pitn]++;
        }
        bool marked = false;
        //Mark
        if (!IS_MARKED(board, pitn) && board[pitn] == 4)
        {
            if ((!PLAYER(board) &&
                 (pitn == 0 || pitn == 5 ||
                  pitn == 6 || pitn == 11 ||
                  pitn == 7 || pitn == 10)) ||
                (PLAYER(board) &&
                 (pitn == 0 || pitn == 5 ||
                  pitn == 1 || pitn == 4 ||
                  pitn == 6 || pitn == 11)))
            {
                MARK(board, pitn, PLAYER(board));
                marked = true;
            }
        }
        Move = Move.substr(0, 2) + std::to_string(Sow);
        if (marked)
            Move = Move + "X" + char(pitn < 6 ? 'a' + pitn : 'A' + pitn - 6);
        moveHistory += Move[1];
        PLAYER(board) = !PLAYER(board);
        UpdateState();
        return true;
    }
    std::string IfPlay(std::string &Move)
    {
        Qelat game(board);
        if (game.Play(Move))
            return game.Board();
        else
            return "#";
    }
    void UpdateState(std::string &Board)
    {
        //Check for normal ending
        int N[2] = {0, 0};
        for (int k = 0; k < 12; ++k)
        {
            if (IS_MARKED(Board, k))
                N[OWNER_MARKED(Board, k)] += Board[k];
        }
        bool GameEnds = N[0] >= 25 ||
                        N[1] >= 25 ||
                        N[0] + N[1] == 48;
        //Check for infinite loop
        if (N[0] + N[1] >= 45)
        {
            bool LeftLoop = !IS_MARKED(Board, 5) &&
                            !IS_MARKED(Board, 6) &&
                            Board[5] + Board[6] == 1;
            bool RightLoop = !IS_MARKED(Board, 0) &&
                             !IS_MARKED(Board, 11) &&
                             Board[0] + Board[11] == 1;
            if (LeftLoop || RightLoop)
                GameEnds = true;
        }
        //Findout result if game ends
        if (GameEnds)
        {
            Halt(Board);
        }
    }
    void UpdateState()
    {
        UpdateState(board);
    }
    void Halt(std::string &Board)
    {
        int N[2] = {0, 0};
        for (int k = 0; k < 12; ++k)
        {
            if (IS_MARKED(Board, k))
            {
                N[OWNER_MARKED(Board, k)] += Board[k];
            }
        }
        STATE(Board) = N[1] == N[0] ? 2
                                    : N[1] > N[0];
    }
    void Halt()
    {
        Halt(board);
    }
    bool Player()
    {
        return PLAYER(board);
    }
    bool GameOn()
    {
        return State() == -1;
    }
    int State()
    {
        return STATE(board);
    }
    std::string Board()
    {
        return board;
    }
    void Show()
    {
        printf(" f   e   d   c   b   a\n");
        for (int k = 5; k >= 0; --k)
        {
            printf("%2d", board[k]);
            if (IS_MARKED(board, k))
                printf("%c ", OWNER_MARKED(board, k) ? '_' : '^');
            else
                printf("  ");
        }
        printf("\n");
        for (int k = 6; k < 12; ++k)
        {
            printf("%2d", board[k]);
            if (IS_MARKED(board, k))
                printf("%c ", OWNER_MARKED(board, k) ? '_' : '^');
            else
                printf("  ");
        }
        printf("\n");
        printf(" A   B   C   D   E   F\n");
    }
    std::string History()
    {
        return moveHistory;
    }
#undef IS_MARKED
#undef OWNER_MARKED
#undef MARK
#undef PLAYER
#undef STATE
};

/*
Bechi(From page 17 of the book):
Game Board:
       <-  <-   ->  ->
        d   c   b   a    
    8   3   2   1   0    
        4   5   6   7  9
        A   B   C   D 
       <-  <-   ->  ->

Player 0 controls pits 0 to 3. Pit 8 stores captured stones.
Player 1 controls pits 4 to 7. Pit 9 stores captured stones.
Pits 0 to 3 are called a to d.
Pits 4 to 7 are called A to D.

1. At the start of the game, each pit has 6 stones.
2. At each step a player selects a pit X that that has at least 2 stones
    from his/her row. He/She then picks up all stones (except one) from X,
    and distribute them one at a time, in a certain direction. 

    The direction depends on the starting pit.
    c,d,C,D : counterclockwise
    a,b,A,B : clockwise
3. If the last stone of a move lands in any pit which now contains an 
    even number of stones(including the stone just dropped in it), the 
    stones in this pit are captured and placed in the player's store.
    In addition, if the following pit or pits also contain an even number
    of stones, these too are captured.
4. Stones can only be captured from a pit that previously in the game 
    (including this move) has been chosen to begin a move.
5. If it's a player's turn to move and he/she has no valid move,
    he/she passes the turn and continue passing until he has a valid move.
6. When neither player can move, each player captures the stones on his/her
    side of the board. The player with more stones wins.
*/
class Bechi
{
#define IS_MARKED(board, k) ((board[10] >> (k)) & 1)
#define MARK(board, k) \
    board[10] = board[10] | (1 << (k))
#define PLAYER(board) board[11]
#define STATE(board) board[12]
    std::string board;
    std::string moveHistory;

  public:
    Bechi()
    {
        Init();
    }
    Bechi(std::string Board)
    {
        Init(Board);
    }
    void Init()
    {
        board = std::string(10, char(6)) + std::string(3, char(0));
        board[8] = board[9] = 0;
        PLAYER(board) = false;
        STATE(board) = -1;
    }
    void Init(std::string Board)
    {
        if (Board.length() == 13)
        {
            board = Board;
            moveHistory = "";
        }
        else
            Init();
    }
    std::vector<std::string> ValidMoves()
    {
        if (!GameOn())
            return {};
        std::vector<std::string> ret;
        for (int base = PLAYER(board) ? 4 : 0, k = 0; k < 4; ++k)
        {
            if (board[base + k] > 1)
            {
                ret.push_back(std::string("#") + char(base ? 'A' + k : 'a' + k));
            }
        }
        //Pass the turn if no move left
        if (ret.empty())
            ret.push_back(std::string("#"));
        return ret;
    }
    bool Play(std::string &Move)
    {
        if (Move.length() < 2)
        {
            //Pass the turn
            if (Move == "#")
            {
                moveHistory += "#";
                PLAYER(board) = !PLAYER(board);
                return true;
            }
            else
                return false;
        }
        int pitn = Move[1] >= 'a' ? Move[1] - 'a' : Move[1] - 'A' + 4;
        int Sow, SeedsLeft, Capture;
        Sow = SeedsLeft = board[pitn] - 1;
        board[pitn] = 1;
        int direction = (pitn <= 1 || (pitn >= 4 && pitn <= 5)) ? 7 : 1;
        MARK(board, pitn);
        //Sow
        while (SeedsLeft-- > 0)
        {
            pitn = (pitn + direction) % 8;
            board[pitn]++;
        }
        //Capture
        Capture = 0;
        while (board[pitn] > 0 &&
               board[pitn] % 2 == 0 &&
               IS_MARKED(board, pitn))
        {
            Capture += board[pitn];
            board[pitn] = 0;
            pitn = (pitn + direction) % 8;
        }

        Move = Move.substr(0, 2) + std::to_string(Sow) + "X" + std::to_string(Capture);
        moveHistory += Move[1];
        board[8 + PLAYER(board)] += Capture;
        PLAYER(board) = !PLAYER(board);
        UpdateState();
        return true;
    }
    std::string IfPlay(std::string &Move)
    {
        Bechi Game(board);
        if (Game.Play(Move))
            return Game.Board();
        else
            return "#";
    }
    void UpdateState()
    {
        UpdateState(board);
    }
    void UpdateState(std::string &Board)
    {
        //Check for normal ending
        bool GameEnds = Board[8] >= 25 ||
                        Board[9] >= 25 ||
                        Board[8] + Board[9] == 48;
        //Check for no moves left
        int NumberOfMoreThanOne = 0;
        for (int i = 0; i < 8; ++i)
            NumberOfMoreThanOne += board[i] >= 2;
        if (NumberOfMoreThanOne == 0)
        {
            GameEnds = true;
            Board[8] += Board[0] + Board[1] + Board[2] + Board[3];
            Board[9] += Board[4] + Board[5] + Board[6] + Board[7];
            for (int i = 0; i < 8; ++i)
                Board[i] = 0;
        }
        //Findout result if game ends
        if (GameEnds)
        {
            Halt(Board);
        }
    }
    void Halt(std::string &Board)
    {
        STATE(Board) = Board[8] == Board[9] ? 2
                                            : Board[9] > Board[8];
    }
    void Halt()
    {
        Halt(board);
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

        printf("     d   c   b   a\n");
        printf("%3d ", board[8]);
        for (int k = 3; k >= 0; --k)
        {
            printf("%2d", board[k]);
            if (!IS_MARKED(board, k))
                printf("* ");
            else
                printf("  ");
        }
        printf("\n    ");
        for (int k = 4; k < 8; ++k)
        {
            printf("%2d", board[k]);
            if (!IS_MARKED(board, k))
                printf("* ");
            else
                printf("  ");
        }
        printf("%3d ", board[9]);
        printf("\n");
        printf("     A   B   C   D\n");
    }
    std::string History()
    {
        return moveHistory;
    }
#undef IS_MARKED
#undef MARK
#undef PLAYER
#undef STATE
};
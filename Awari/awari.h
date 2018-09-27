#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#ifndef AWARI_HEADER
#define AWARI_HEADER

/*
    Game Board:
        f   e   d   c   b   a    
    12  5   4   3   2   1   0    
        6   7   8   9  10  11  13
        A   B   C   D   E   F    
    
    Player 0 controls pits 0 to 5 and pit 13. pit 13 is used to store captured stones.
    Player 1 controls pits 6 to 12 where pit 12 is used to store captured stones.
    pits 0 to 5 are called a to f.
    pits 6 to 11 are called A to F.
*/

/*
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
    //Set One: The Game
    std::string board;
    //player 0 always goes first
    bool player;
    //-1 for ongoing
    //0,1 for player 0 and 1 wins
    //2 for draw
    int state;
    //count reached boards
    std::map<std::string, int> M;

    //Set Two: Valid Moves
    int validCount;
    bool isMoveValid[6];
    std::string nextMove[6];
    std::string nextBoard[6];

  public:
    Awari()
    {
        Init();
    }
    Awari(std::string Board)
    {
        Init(Board);
    }
    //Initialize the Game
    void Init()
    {
        board = std::string(14, char(4));
        board[12] = board[13] = 0;
        player = false;
        state = -1;
        ValidMoves();
    }
    //Initialize the Game using Board
    void Init(std::string Board)
    {
        if (Board.length() == 16)
        {
            board = Board.substr(0, 14);
            player = Board[14];
            state = Board[15];
            ValidMoves();
        }
        else
        {
            Init();
        }
    }
    bool GameOn()
    {
        return state == -1;
    }
    void ClearTable(bool even_split)
    {
        //Sum up the stones on the board
        int Sum = 0;
        for (int i = 0; i < 12; ++i)
        {
            Sum += board[i];
            board[i] = 0;
        }

        if (even_split)
        {
            //split evenly
            board[12] += Sum / 2;
            board[13] += Sum / 2;
        }
        else
        {
            //If one player cannot reply at his turn
            //all the stones belong to the other player
            board[13 - player] += Sum;
        }
    }
    int getState()
    {
        return state;
    }
    void UpdateState()
    {
        if (state != -1)
        {
            return;
        }
        //Check for winner
        if (board[12] >= 25 || board[13] >= 25)
        {
            //a player wins
            state = board[13] >= 25;
        }
        else if (board[12] + board[13] == 48 &&
                 board[12] == board[13])
        {
            state = 2;
        }
    }
    //Board = board(0:14) + player(14) + state(15)
    void UpdateState(std::string &Board)
    {
        if (Board[15] != -1)
        {
            return;
        }
        //Check for winner
        if (Board[12] >= 25 || Board[13] >= 25)
        {
            //a player wins
            Board[15] = Board[13] >= 25;
        }
        else if (Board[12] + Board[13] == 48 &&
                 Board[12] == Board[13])
        {
            Board[15] = 2;
        }
    }
    //Play the move if possible, update Board and Move
    //does not check for rule 7
    bool Play(std::string &Board, std::string &Move)
    {
        int pitn = -1;
        if (player && Move[0] >= 'A' && Move[0] <= 'F')
        {
            pitn = Move[0] - 'A' + 6;
        }
        if (!player && Move[0] >= 'a' && Move[0] <= 'f')
        {
            pitn = Move[0] - 'a';
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
        int base = Board[14] ? 0 : 6;
        while ((Board[cnt] == 2 || Board[cnt] == 3) &&
               cnt >= base && cnt - base <= 5)
        {
            //capture
            capture += Board[cnt];
            Board[cnt] = 0;
            //move cnt backwards and check again
            cnt = (cnt + 11) % 12;
        }
        Board[12 + Board[14]] += capture;
        if (Move.length() == 1)
        {
            Move += std::to_string(sow) + "X" + std::to_string(capture);
        }
        Board[14] = !Board[14];
        UpdateState(Board);
        return true;
    }
    bool Play(std::string &Move)
    {
        int pitn = -1;
        if (player && Move[0] >= 'A' && Move[0] <= 'F')
        {
            pitn = Move[0] - 'A' + 6;
        }
        if (!player && Move[0] >= 'a' && Move[0] <= 'f')
        {
            pitn = Move[0] - 'a';
        }
        if (-1 == pitn || 0 == board[pitn])
        {
            //cannot play the move
            return false;
        }
        int sow = board[pitn], capture = 0;
        //Sow
        int cnt = pitn, nstone = sow;
        board[pitn] = 0;
        while (nstone-- > 0)
        {
            //move up one pit
            cnt = (cnt + 1) % 12;
            //skip self
            cnt += cnt == pitn;
            cnt %= 12;
            //put a stone
            board[cnt]++;
        }
        //Capture
        int base = player ? 0 : 6;
        while ((board[cnt] == 2 || board[cnt] == 3) &&
               cnt >= base && cnt - base <= 5)
        {
            //capture
            capture += board[cnt];
            board[cnt] = 0;
            //move cnt backwards and check again
            cnt = (cnt + 11) % 12;
        }
        board[12 + player] += capture;
        if (Move.length() == 1)
        {
            Move += std::to_string(sow) + "X" + std::to_string(capture);
        }
        UpdateState();
        player = !player;
        ValidMoves();
        return true;
    }
    //Calculate Valid Moves and stores them
    void ValidMoves()
    {
        if (!GameOn())
        {
            //There are no valid moves
            //If the game has ended
            //missing this code will cause
            //the computer to avoid winning
            //at the last second.
            memset(isMoveValid, 0, sizeof(isMoveValid));
            validCount = 0;
            return;
        }

        if (++M[board] >= 3)
        {
            //if a state is reached the third time,
            //split the rest of the stones
            ClearTable(true);
            UpdateState();
            return;
        }
        bool canReply[6];
        int replyCount;
        replyCount = validCount = 0;
        //try all 6 moves
        std::vector<std::string> moves;
        if (player)
        {
            moves = std::vector<std::string>{"A", "B", "C", "D", "E", "F"};
        }
        else
        {
            moves = std::vector<std::string>{"a", "b", "c", "d", "e", "f"};
        }
        for (int k = moves.size() - 1; k >= 0; --k)
        {
            //store results in NextBoard[k]
            nextBoard[k] = toString();
            nextMove[k] = moves[k];
            isMoveValid[k] = Play(nextBoard[k], nextMove[k]);
            if (isMoveValid[k])
            {
                validCount++;
                canReply[k] = 0;
                for (int base = player ? 0 : 6, i = 0; i < 6; ++i)
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
    std::vector<std::string> ValidMovesVec()
    {
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
    //Returns the new Board after playing the move 'Move'
    //Returns empty string if the move is not valid.
    std::string IfPlay(std::string Move)
    {
        for (int k = 0; k < 6; ++k)
        {
            if (isMoveValid[k] && Move[0] == nextMove[k][0])
            {
                return nextBoard[k];
            }
        }
        return "";
    }
    //return Board
    std::string toString()
    {
        return board + char(player) + char(state);
    }
    //does the current palyer win if both player
    //plays randomly.
    //return 1,0,-1 for win,draw and lose.
    int RandomSimulation()
    {
        bool cnt_player = player;
        int Count = 0;
        while (GameOn() && Count++ < 300)
        {
            std::vector<std::string> Moves = ValidMovesVec();
            int N = Moves.size();
            if (!N)
                break;
            std::string move = Moves[rand() % N];
            Play(move);
        }
        if (GameOn())
        {
            ClearTable(true);
            UpdateState();
        }
        return state == 2 ? 0 : state == cnt_player ? 1 : -1;
    }
    void Show()
    {
        printf("      f   e   d   c   b   a\n");
        printf("%3d %3d %3d %3d %3d %3d %3d\n", board[12], board[5], board[4], board[3], board[2], board[1], board[0]);
        printf("    %3d %3d %3d %3d %3d %3d %3d\n", board[6], board[7], board[8], board[9], board[10], board[11], board[13]);
        printf("      A   B   C   D   E   F\n\n");
        printf("--------------------------------\n\n\n");
    }
};
#endif
#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#ifndef AWARI_HEADER
#define AWARI_HEADER

/*
    Game Board:
        f   e   d   c   b   a    
    6   5   4   3   2   1   0    
        7   8   9   10  11  12  13
        A   B   C   D   E   F    
    
    Player 0 controls pits 0 to 6. pit 6 is vault.
    Player 1 controls pits 7 to 13.pit 13 is vault.
    pits 0 to 5 are called a to f.
    pits 7 to 12 are called A to F.
    pit 6 and 13 are called vaults, they are used to store captured stones.
*/

/*
    Mancala Rules(one of the versions):
    1. At the start of the game, each pit(except for 6 and 13) has 4 stones.
    2. At each step a player selects a non-empty pit X from his/her row, starting 
        with X's neighbor, he/she then sows all stones from X, one at a time, 
        counter-clockwise over the board(skip opponent vault). Captured Stones 
        are removed and stored in either pit 6 or 13, depending on the player.
    3. Stones are captured if the last stone sown lands in the player's empty pit X. 
        In this case, the last stone and all the stones on the opponent board next to X 
        are all put into player's vault.
        This happens between (A,f),(B,e),(C,d),(D,c),(E,b) and (F,a).
    4. If a move ends in the player's vault, that player can move again.
    5. End of the game:
    5.1. The Goal is to capture more stones than the opponent. The game ends 
        as soon as one of the player has collected 25 or more stones.
    5.2. As soon as a player has no stones left on his/her side(excluding vault), 
        the remaining stones are captured by the opponent. 
    5.3. If the same position is encountered for the third time, with the same player to move, 
        the remaining stones on the board are evenly devided among the players.
    6. The winner is the player who captured the most stones. If both players capture 24 stones, 
        the game is drawn.
    */

class Mancala
{
    //Set One: The Game
    std::string board;
    //player 0 always goes first
    //player = board[14]

    //-1 for ongoing
    //0,1 for player 0 and 1 wins
    //2 for draw
    //state = board[15]

  public:
    Mancala()
    {
        Init();
    }
    Mancala(std::string Board)
    {
        Init(Board);
    }
    //Initialize the Game
    void Init()
    {
        board = std::string(16, char(4));
        board[6] = board[13] = 0;
        board[14] = false; //player = false
        board[15] = -1;    //state = -1
    }
    //Initialize the Game using Board
    void Init(std::string Board)
    {
        if (Board.length() == 16)
        {
            board = Board;
        }
        else
        {
            Init();
        }
    }
    bool GameOn()
    {
        return board[15] == -1;
    }
    void ClearTable(bool even_split)
    {
        //Sum up the stones on the board
        int Sum0 = 0, Sum1 = 0;
        for (int k = 0; k < 6; ++k)
        {
            Sum0 += board[k];
            Sum1 += board[k + 7];
            board[k] = board[k + 7] = 0;
        }
        int Sum = Sum0 + Sum1;

        if (even_split)
        {
            //split evenly
            board[6] += Sum / 2;
            board[13] += Sum / 2;
        }
        else
        {
            //If one player cannot reply at his turn
            //all the stones belong to the other player
            if (board[14])
                board[13] += Sum;
            else
                board[6] += Sum;
        }
    }
    int getState()
    {
        return board[15];
    }
    int getPlayer()
    {
        return board[14];
    }
    void UpdateState()
    {
        UpdateState(board);
    }
    //Board = board(0:14) + player(14) + state(15)
    void UpdateState(std::string &Board)
    {
        //If Game has ended, return
        if (Board[15] != -1)
        {
            return;
        }
        //Check for rule 5.2
        int Sum0 = Board[0] + Board[1] + Board[2] + Board[3] + Board[4] + Board[5];
        int Sum1 = Board[7] + Board[8] + Board[9] + Board[10] + Board[11] + Board[12];
        if ((!Sum0 && Sum1) || (Sum0 && !Sum1))
        {
            Board[6] += Sum0;
            Board[13] += Sum1;
            Board[0] = Board[1] = Board[2] = Board[3] = Board[4] = Board[5] = 0;
            Board[7] = Board[8] = Board[9] = Board[10] = Board[11] = Board[12] = 0;
        }
        //Check for winner
        if (Board[6] >= 25 || Board[13] >= 25)
        {
            //a player wins
            Board[15] = Board[13] >= 25;
        }
        else if (Board[6] + Board[13] == 48 &&
                 Board[6] == Board[13])
        {
            Board[15] = 2;
        }
    }
    int getPitN(bool player, std::string Move)
    {
        if (Move == "")
            return -1;
        if (player && Move[0] >= 'A' && Move[0] <= 'F')
            return Move[0] - 'A' + 7;
        if (!player && Move[0] >= 'a' && Move[0] <= 'f')
            return Move[0] - 'a';
        return -1;
    }
    //Play the move if possible, update Board and Move
    bool Play(std::string &Board, std::string &Move)
    {
        int pitn = getPitN(Board[14], Move);
        if (-1 == pitn || 0 == Board[pitn])
        {
            //cannot play the move
            return false;
        }
        int sow = Board[pitn], capture = 0;
        //Sow
        int cnt = pitn, nstone = sow, ownVault = Board[14] ? 13 : 6;
        int ownVaultCount = Board[ownVault];
        Board[pitn] = 0;
        while (nstone-- > 0)
        {
            //move up one pit
            cnt = (cnt + 1) % 14;
            //skip opponent vault
            cnt += cnt == 19 - ownVault;
            cnt %= 14;
            //put a stone
            Board[cnt]++;
        }
        std::string end;
        if (cnt == ownVault)
        {

            //if lands on own vault, can move again
            end = "|";
        }
        else
        {
            //Capture
            if (Board[cnt] == 1 && cnt <= ownVault && cnt >= ownVault - 6)
            {
                capture += Board[cnt] + Board[12 - cnt];
                Board[cnt] = 0;
                Board[12 - cnt] = 0;
            }
            if (Board[14])
                Board[13] += capture;
            else
                Board[6] += capture;
            //change player
            Board[14] = !Board[14];
        }
        if (Move.length() == 1)
        {
            Move += std::to_string(sow) + "X" + std::to_string(Board[ownVault] - ownVaultCount) + end;
        }
        UpdateState(Board);
        return true;
    }
    bool Play(std::string &Move)
    {
        return Play(board, Move);
    }
    std::vector<std::string> ValidMovesVec()
    {
        std::vector<std::string> ret;
        int basePit;
        char baseChar;
        if (board[14])
        {
            basePit = 7;
            baseChar = 'A';
        }
        else
        {
            basePit = 0;
            baseChar = 'a';
        }
        for (int k = 0; k < 6; ++k)
        {
            if (board[basePit + k])
            {
                ret.push_back(std::string() + char(baseChar + k));
            }
        }
        return ret;
    }
    //Returns the new Board after playing the move 'Move'
    //Returns empty string if the move is not valid.
    std::string IfPlay(std::string &Move)
    {
        int pitn = getPitN(board[14], Move);
        if (-1 == pitn || 0 == board[pitn])
        {
            return "";
        }
        std::string tempboard = board;
        Play(tempboard, Move);
        return tempboard;
    }
    //return Board
    std::string toString()
    {
        return board;
    }
    //does the current palyer win if both player
    //plays randomly.
    //return 1,0,-1 for win,draw and lose.
    int RandomSimulation()
    {
        bool cnt_player = board[14];
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
        return board[15] == 2 ? 0 : board[15] == cnt_player ? 1 : -1;
    }
    void Show()
    {
        printf("      f   e   d   c   b   a\n");
        printf("%3d %3d %3d %3d %3d %3d %3d\n", board[6], board[5], board[4], board[3], board[2], board[1], board[0]);
        printf("    %3d %3d %3d %3d %3d %3d %3d\n", board[7], board[8], board[9], board[10], board[11], board[12], board[13]);
        printf("      A   B   C   D   E   F\n\n");
        printf("--------------------------------\n\n\n");
    }
};
#endif
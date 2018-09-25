#include <iostream>
//#include <cstdio>
//#include <cstdlib>
#include <cmath>
#include <map>
using namespace std;
#define LL long long
#ifndef AWARI_HEADER
#define AWARI_HEADER
/*
    There are many different rules using the same board.
    The Goal is to make it easy to change the rules without 
    affecting the later codes that plays the game.
*/

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
        are removed and stored in either pit 6 or 13, depending on the player.
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

struct Move{
    char pit;
    char sow;
    char capture;
    Move():pit(0){}
    Move(char pit, char sow = 0, char capture = 0):pit(pit),sow(sow),capture(capture){}
    bool IsValid(){
        return pit != 0 && sow != 0;
    }
    void Show(bool newline = true, bool full = true){
        if(pit){
            if(full)
                printf("%c%d X %d",pit,sow,capture);
            else
                printf("%c",pit);
        }
        else
            printf("invalid move");
        if(newline){
            printf("\n");
        }
        else{
            printf("  ");
        }
    }
};
struct AwariBoardDense{
    //14 pits each has at most 48 stones
    //plus one bit to indicate the player to move
    //(48^14)*2  ~  80 bits fits in two 64bit int.
    LL A[2];
    AwariBoardDense(){}
    AwariBoardDense(bool player, string pit){
        SetValue(player,pit);
    }
    void SetValue(bool player, string pit){
        for(int i = 0 ; i < 14 ; ++i){
            A[i / 8] |= (LL) pit[i] << (8 *(i % 8));
        }
        A[1] |= (player ? 1LL : 0LL) << 48;
    }
    void GetValue(bool &player, string &pit){
        pit = string(14,0);
        for(int i = 0 ; i < 14 ; ++i){
            A[i / 8] |= (LL) pit[i] << (8 * (i % 8));
            pit[i] = (A[i / 8] >> (i % 8)) & 255LL;
        }
        player = A[1] >> 48;
    }
    bool operator < (const AwariBoardDense &B)const{
        return A[0] < B.A[0] || (A[0] == B.A[0] && A[1] < B.A[1]);
    }
};

struct AwariBoard{
    //player: 0 for player 0's turn, 1 for player 1's turn
    bool player;
    //   Game Board:
    //    f   e   d   c   b   a    
    //12  5   4   3   2   1   0    
    //    6   7   8   9  10  11  13
    //    A   B   C   D   E   F    
    string pit;
    AwariBoard(bool player = false){
        Init(player);
    }
    void Init(bool player = false){
        this->player = player;
        pit = string(14,char(4));
        pit[12] = pit[13] = 0;
    }
    AwariBoardDense Compress(){
        AwariBoardDense ABD;
        ABD.SetValue(player,pit);
        return ABD;
    }
    // Make a move, does not check
    void sow(Move &M, int pitn){
        M.sow = pit[pitn];
        pit[pitn] = 0;
        
        int NStone = M.sow, I = pitn;
        //Sow
        while(NStone-- > 0){
            I = (I + 1) % 12;
            I = (I + (I == pitn)) % 12;
            pit[I]++;
        }
        
        //Capture
        int Capture = 0;
        if(player){
            while(I < 6 && (pit[I] == 2 || pit[I] == 3) ){
                Capture += pit[I];
                pit[I] = 0;
                I = (I + 11) % 12;
            }
        }
        else{
            while(I >= 6 && (pit[I] == 2 || pit[I] == 3) ){
                Capture += pit[I];
                pit[I] = 0;
                I = (I + 11) % 12;
            }
        }
        M.capture = Capture;
        pit[12 + player] += Capture;
        player = !player;
    }
    void ClearTable(int option){
        int Sum = 0;
        for(int i = 0 ; i < 12 ; ++i){
            Sum += pit[i];
            pit[i] = 0;
        }
        if(option == 2){
            //split the remaining stones
            //skip one if there are odd remaining stones
            pit[12] += Sum / 2;
            pit[13] += Sum / 2;
        }
        else{
            if(option){
                pit[13] += Sum;
            }
            else{
                pit[12] += Sum;
            }
        }
    }
    int CheckWinner(){
        //0 for player 0, 1 for player 1, 2 for draw, -1 for unknown
        if(pit[12] >= 25 || pit[13] >= 25){
            return pit[13] >= 25;//one side wins
        }
        if(pit[12] == 24 && pit[13] == 24){
            return 2;//draw
        }
        return -1;//unknown
    }
    bool canReply(){
        int MoveCount = 0;
        for(int pitn = player * 6, k = 0; k < 6 ; ++k, ++pitn)
            MoveCount += pit[pitn] > 0;
        return MoveCount != 0;
    }
    void Show(){
        printf("      f   e   d   c   b   a\n");
        printf("%3d %3d %3d %3d %3d %3d %3d\n",pit[12],pit[5],pit[4],pit[3],pit[2],pit[1],pit[0]);
        printf("    %3d %3d %3d %3d %3d %3d %3d\n",pit[6],pit[7],pit[8],pit[9],pit[10],pit[11],pit[13]);
        printf("      A   B   C   D   E   F\n\n");
        printf("--------------------------------\n\n\n");
    }
};

struct AwariGame{
    AwariBoard Board;
    map<AwariBoardDense,int> M;
    //-1 for on going
    //0,1 for players 0,1 
    //2 for draw
    int GameState;
    string MoveHistory;
    
    //Indicating if a move(A~F or a~f) is valid
    int validCount;
    bool isMoveValid[6];
    AwariBoard NextBoard[6];
    Move NextMove[6];
    AwariGame(){}
    AwariGame(AwariBoard Board){
        this->Board = Board;
        GameState = -1;
        MoveHistory = "";
        ValidMoves();
        CheckWinner();
    }
    AwariGame(string Moves){
        if(!SetGame(Moves)){
            printf("SetGame Failed\n");
        }
    }
    bool SetGame(string Moves){
        int L = Moves.length();
        if(!L) return false;
        Board.Init(Moves[0] < 'a');
        GameState = -1;
        MoveHistory = "";
        ValidMoves();
        CheckWinner();
        
        for(int i = 0 ; i < L ; ++i){
            Move M = Sow(Moves[i]);
            if(!M.IsValid()){
                printf("Invalid Moves[%d]=%c\n",i,Moves[i]);
                return false;
            }
        }
        return true;
    }
    void Init(bool player = false){
        //set the board
        Board.Init(player);
        GameState = -1;
        MoveHistory = "";
        ValidMoves();
        CheckWinner();
    }
    char NtoC(int pitn){
        return pitn < 6 ? char(pitn + 'a')
                        : char(pitn + 'A' - 6);
    }
    void ValidMoves(){
        if(-1 != GameState) return;
        if(++M[Board.Compress()] == 3){
            //if reach a state 3 times, 
            //split the rest of the stones
            Board.ClearTable(2);
            return;
        }
        bool canReply[6];
        int replyCount;
        replyCount = validCount = 0;
        //try all 6 moves
        for(int pitn = Board.player * 6, k = 0 ; k < 6 ; ++k, ++pitn){
            if(!Board.pit[pitn]) {
                isMoveValid[k] = false;
                continue;
            }
            //store results in NextBoard[k]
            NextBoard[k] = Board;
            NextMove[k] = Move(NtoC(pitn));
            NextBoard[k].sow(NextMove[k],pitn);
            
            isMoveValid[k] = true;
            validCount ++;
            canReply[k] = NextBoard[k].canReply();
            replyCount += canReply[k];
        }
        if(replyCount == 0 || replyCount == validCount){
            //if all can reply or all cannot reply
            //do nothing
        }
        else{
            //if some cannot reply
            //eliminate the moves where the opponent cannot reply
            for(int k = 0 ; k < 6 ; ++k)
                isMoveValid[k] = isMoveValid[k] && canReply[k];
        }
        //Show();
    }
    string validMovesStr(){
        string ret;
        char base = Board.player ? 'A' : 'a';
        for(int k = 0 ; k < 6 ; ++k){
            if(isMoveValid[k]) 
                ret += char(base + k);
        }
        return ret;
    }
    Move Sow(char pitc){
        MoveHistory += pitc;
        int moveID = -1;
        //player = 0, valid moves = {a,b,c,d,e,f}
        //player = 1, valid moves = {A,B,C,D,E,F}
        if(!Board.player && pitc >= 'a' && pitc <= 'f'){
            //player 0 moves
            moveID = pitc - 'a';
        }
        if(Board.player && pitc >= 'A' && pitc <= 'F'){
            //player 1 moves
            moveID = pitc - 'A';
        }
        if(-1 == moveID || !isMoveValid[moveID])
            return Move();
        Board = NextBoard[moveID];
        Move retMove = NextMove[moveID];
        ValidMoves();
        CheckWinner();
        return retMove;
    }
    void CheckWinner(){
        if(GameState != -1) return;
        //if one player cannot make a move
        //all remaining stones belongs to the other player
        if(validCount == 0){
            Board.ClearTable(!Board.player);
        }
        //0 for player 0, 1 for player 1, 2 for draw, -1 for unknown
        GameState = Board.CheckWinner();
    }
    //Stop the game early and estimate winner
    void Halt(){
        CheckWinner();
        if(GameState != -1) return;
        GameState = Board.pit[12] == Board.pit[13] ? 2 :
                    Board.pit[12] < Board.pit[13];
    }
    void Show(){
        printf("--------------------------------\n");
        printf("Valid Moves are:  ");
        for(int k = 0 ; k < 6 ; ++k){
            if(isMoveValid[k]) 
                NextMove[k].Show(false,false);
        }
        printf("\n");
        Board.Show();
    }
};

#endif
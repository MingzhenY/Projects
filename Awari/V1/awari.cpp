#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <map>
#include "awari.h"
#include "mcts.h"
using namespace std;


void Game_MCTS(){
    AwariGame Game;
    Game.Init(false);
    MCTS MC(Game.Board,0.8,20000);
    Game.Show();
    
    bool computerFirst = true;
    bool flag = computerFirst;
    while(-1 == Game.GameState){
        Move M;
        MC.ShowNextMoves();
        if(flag){
            printf("Computer Thinking...  \n\t\t\t\t\t\t\t\t");
            char move = MC.nextMove();
            M = Game.Sow(move);
        }
        else{
            string input;
            cin>>input;
            char pit = input[0];
            if(pit == 'X') break;
            if( (pit >= 'a' && pit <= 'f') ||
                (pit >= 'A' && pit <= 'F') ){
                M = Game.Sow(pit);
            }
        }
        if(-1 != Game.GameState){
            //Game Ends after computer moves
            M.Show();
            Game.Show();
            break;
        }
        if(M.IsValid()){
            M.Show();
            printf("%s\n",Game.MoveHistory.c_str());
            Game.Show();
            if(MC.Sow(M.pit)){
                printf("SimCount=%d\n",MC.SimCount);
            }
            else{
                //No Moves Left
                printf("Sow Error\n");
                break;
            }
            flag = !flag;
        }
        else{
            printf("Invalid Move! Please Try Again!\n");
        }
    }
    printf("\n\n---------- Game Over ------------\n");
    printf("%d vs %d\n",Game.Board.pit[12],Game.Board.pit[13]);
    printf("%s\n",Game.MoveHistory.c_str());
    if(Game.GameState != computerFirst){
        printf("Computer Wins\n");
    }
    else{
        printf("You win!\n");
    }
}

int main(){
    srand(time(NULL));
    Game_MCTS();
    return 0;
}
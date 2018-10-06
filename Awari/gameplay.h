#include <iostream>
#include <vector>

/*
Here defines the code to generate games given 
the class Game.
*/
template <class Game>
class GamePlay
{
  public:
    GamePlay()
    {
        srand(time(NULL));
    }
    void Game_MCTS(bool computerFirst = true, double C = 0.8, int SimCount = 1000)
    {
        Game game;
        game.Show();

        MCTS<Game> MC(game.Board(), C, SimCount);

        bool flag = computerFirst;
        bool Error = false;
        while (game.GameOn() && !Error)
        {
            std::string Move = "#";
            //MC.ShowNextMoves();
            bool Success = false;
            if (flag)
            {
                printf("Computer Thinking...  \t\t\n");
                Move = MC.nextMove();
                MC.ShowNextMoves();
                //printf("Move = %s\n", Move.c_str());
                if (Move != "#")
                {
                    Success = game.Play(Move);
                }
                else
                {
                    printf("Computer failed\n");
                    Error = true;
                }
            }
            else
            {
                std::cin >> Move;
                if (Move == "exit" || Move == "EXIT")
                    break;
                //std::vector<std::string> Moves = Game.ValidMoves();
                //Move = Moves[rand() % Moves.size()];
                Move = std::string("#") + Move;
                Success = game.Play(Move);
            }

            if (Success)
            {
                printf("%s\n", Move.substr(1).c_str());
                game.Show();
                if (MC.Play(Move))
                {
                    //printf("SimCount=%d\n", MC.SimCount);
                }
                else
                {
                    //No Moves Left
                    printf("Play Error\n");
                    Error = true;
                    break;
                }
                if (Move[0])
                    flag = !flag;
            }
            else
            {
                if (flag)
                {
                    Error = true;
                }
                printf("Invalid Move! Please Try Again!\n");
            }
        }
        if (game.GameOn())
        {
            if (Error)
            {
                printf("Error : no result\n");
            }
            else
            {
                printf("User Interrupt\n");
                printf("History: %s\n", game.History().c_str());
            }
            return;
        }
        printf("\n\n---------- Game Over ------------\n");
        printf("%s\n", game.History().c_str());
        game.Show();
        int S = game.State();
        if (S == 2)
        {
            printf("Draw!\n");
        }
        else if ((S == 0 && computerFirst) || (S == 1 && !computerFirst))
        {
            printf("Computer Wins!\n");
        }
        else
        {
            printf("You win!\n");
        }
    }
};
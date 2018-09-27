#include <vector>
#ifndef GAME_MCTS
#define GAME_MCTS
/*
Write a Monte-Carlo Tree Search that can be used on 
different games.
*/

/*
The minimal requirements of the game class.
A game board is represented by std::string.
A move is represented by std::string.
*/
class GameName
{
  public:
    GameName(std::string Board)
    {
        Init(Board);
    }
    //Initialize the Game
    void Init()
    {
    }
    //Initialize the Game with Board
    void Init(std::string Board)
    {
    }
    //Return valid moves
    std::vector<std::string> ValidMovesVec()
    {
        return {};
    }
    //Returns the new Board after playing the move 'Move'
    std::string IfPlay(std::string &Move)
    {
        std::string newBoard;
        return newBoard;
    }
    //does the current palyer win if both player
    //plays randomly.
    //return 1,0,-1 for win,draw and lose.
    int RandomSimulation()
    {
        return 0;
    }
};

struct MCT
{
    std::string Board;
    std::vector<std::string> Moves;
    MCT *leftchild, *next, *parent;
    int N;    //tree simulation count
    int n;    //node simulation count
    double X; //average value over all N simulations
    MCT(std::string Board)
    {
        this->Board = Board;
        leftchild = next = parent = NULL;
        N = n = 0;
        X = 0.0;
    }
};

template <class Game>
struct MCTS
{
    MCT *Root;
    int SimCount; //Simulation Count
    int SimLimit; //Simulation Limit
    double C;     //exploration parameter
    bool Error;   //Indicate Error
    MCTS(std::string Board, double C = 0.8, int Limit = 10000)
    {
        Init(Board, C, Limit);
    }
    void Init(std::string Board, double C, int Limit)
    {
        Root = new MCT(Board);
        Expand(Root);
        this->C = C;
        SimCount = 0;
        SimLimit = Limit;
        Error = false;
    }
    std::string nextMove()
    {
        if (!Root)
        {
            return "#";
        }
        //Add new nodes until reach Limit
        while (SimCount < SimLimit && !Error)
        {
            MCT *p = Select(Root);
            Expand(p);
        }
        if (Error)
        {
            return "#";
        }
        //choose the most robust child
        //i.e. the child with most simulations
        std::string Move = "#";
        int bestCount, I = 0;
        for (MCT *t = Root->leftchild; t; t = t->next, ++I)
        {
            if (Move == "#" ||
                t->N > bestCount)
            {
                Move = Root->Moves[I];
                bestCount = t->N;
            }
        }
        return Move;
    }
    MCT *Select(MCT *p)
    {
        while (p && p->leftchild)
        {
            //select the best child to expand
            MCT *temp = p->leftchild;
            MCT *best_child = NULL;
            double best_value;
            int I = 0;
            while (temp)
            {
                double value = (p->Moves[I][p->Moves[I].length() - 1] == '|' ? temp->X : -temp->X) + C * sqrt(2 * log(p->N) / temp->N);
                if (best_child == NULL || value > best_value)
                {
                    best_value = value;
                    best_child = temp;
                }
                temp = temp->next;
                ++I;
            }
            p = best_child;
        }
        return p;
    }
    //return the number of SimCount added
    int Expand(MCT *rt)
    {
        if (!rt)
        {
            printf("ERROR : Expanding empty tree\n");
            Error = true;
            return 0;
        }
        if (rt->leftchild)
        {
            //Root is always expanded
            //After Play()
            //the new Root might not have been expanded.
            //So call Expand on new Root
            //Avoid expanding multiple times here.
            return 0;
        }

        Game game(rt->Board);
        rt->Moves = game.ValidMovesVec();
        if (!rt->Moves.empty())
        {
            for (int k = rt->Moves.size() - 1; k >= 0; --k)
            {
                //Create new node
                MCT *temp = new MCT(game.IfPlay(rt->Moves[k]));
                temp->parent = rt;
                temp->next = rt->leftchild;
                rt->leftchild = temp;
                //Simulation + Update
                int result = Simulation(temp);
                Update(temp, result);
            }
            return rt->Moves.size();
        }
        else
        {
            int result = Simulation(rt);
            Update(rt, result);
            return 1;
        }
    }
    //returns 1,0,-1 for win,draw and lose
    int Simulation(MCT *rt)
    {
        Game game(rt->Board);
        int result = game.RandomSimulation();
        rt->n++;    //increase node simulation count
        SimCount++; //increase global simulation count
        return result;
    }
    void Update(MCT *rt, int result)
    {
        while (rt)
        {
            //Update rt->X (the average outcome of 1,0,-1)
            rt->X = (rt->N * rt->X + result) / (rt->N + 1);
            rt->N++; //increase tree simulation count
            //Move towards the root
            rt = rt->parent;
            //flip the result
            result = -result;
        }
    }
    bool Play(std::string Move)
    {
        //Make the move, clear the rest of the tree
        MCT *p = Root->leftchild, *pp = NULL;
        int I = Root->Moves.size(), i = 0;
        while (p)
        {
            if (i >= I)
                return false;
            if (Root->Moves[i] == Move)
            {
                //Make a move(remove p)
                if (pp)
                {
                    pp->next = p->next;
                }
                else
                {
                    Root->leftchild = p->next;
                }
                //Delete the rest of the tree
                Clear(Root);
                //set p as new Root
                Root = p;
                Root->next = NULL;
                Root->parent = NULL;
                Expand(Root);
                return true;
            }
            i++;
            pp = p;
            p = p->next;
        }
        return false;
    }
    void ShowNextMoves()
    {
        MCT *p = Root->leftchild;
        int i = 0;
        while (p)
        {
            printf("%s(%.1f%%)  ", Root->Moves[i].c_str(), p->N * 100.0 / Root->N);
            ++i;
            p = p->next;
        }
        printf("\n");
    }
    void Show()
    {
        show(Root, 0);
    }
    void show(MCT *rt, int depth = 0)
    {
        if (!rt)
            return;
        for (int i = 0; i < depth; ++i)
            printf("    ");
        printf("%d : %.2f\n", rt->Board[14], rt->X);
        MCT *temp = rt->leftchild;
        while (temp)
        {
            show(temp, depth + 1);
            temp = temp->next;
        }
    }
    void Clear(MCT *&rt)
    {
        if (!rt)
            return;
        Clear(rt->leftchild);
        Clear(rt->next);
        SimCount -= rt->n;
        delete (rt);
        rt = NULL;
    }
    ~MCTS()
    {
        Clear(Root);
    }
};

#endif
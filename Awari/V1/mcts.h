#include "awari.h"
using namespace std;

struct MCT{
    AwariBoard Board;
    string Moves;
    MCT *leftchild,*next,*parent;
    int N,n;//N: tree visit count, n: node visit count
    double X;//average return
    MCT(AwariBoard Board){
        this->Board = Board;
        leftchild = next = parent = NULL;
        N = n = 0;
        X = 0.0;
        Moves = "#";//# for not ininitialized
    }
};

struct MCTS{
    MCT *Root;
    int SimCount;//Simulation Count
    int SimLimit;//Simulation Limit
    double C;//exploration parameter
    MCTS(AwariBoard Board,double C = 0.8,int Limit = 10000){
        Init(Board,C,Limit);
    }
    void Init(AwariBoard Board,double C,int Limit){
        Root = new MCT(Board);
        this->C = C;
        SimCount = 0;
        SimLimit = Limit;
    }
    char nextMove(){
        //Add new nodes until reach Limit
        while(SimCount < SimLimit){
            MCT *p = Select(Root);
            Expand(p);
        }
        //choose the most robust child
        //i.e. the child with most simulations
        char bestMove = '#';
        int bestCount, I = 0;
        for(MCT *t = Root->leftchild ; t ; t = t->next, ++I){
            if(bestMove == '#' ||
                t->N > bestCount){
                bestMove = Root->Moves[I];
                bestCount = t->N;
            }
        }
        return bestMove;
    }
    MCT *Select(MCT *p){
        while(p && p->leftchild){
            //select the best child to expand
            MCT *temp = p->leftchild;
            MCT *best_child = NULL;
            double best_value;
            
            while(temp){
                double value = -temp->X + C * sqrt(2 * log(p->N) / temp->N);
                if(best_child == NULL || value > best_value){
                    best_value = value;
                    best_child = temp;
                }
                temp = temp->next;
            }
            p = best_child;
        }
        return p;
    }
    bool Expand(MCT *rt){
        if(!rt){
            printf("ERROR : Expanding empty tree\n");
            return false;
        }
        if(rt->leftchild){
            printf("ERROR : Expanding non-leaf node\n");
            return false;
        }
        AwariGame Game(rt->Board);
        rt->Moves = Game.validMovesStr();
        if(rt->Moves != ""){
            for(int i = rt->Moves.length() - 1 ; i >= 0 ; --i){
                //Create new node
                int I = rt->Moves[i] >= 'a' ? rt->Moves[i] - 'a' : rt->Moves[i] - 'A';
                MCT *temp = new MCT(Game.NextBoard[I]);
                temp->parent = rt;
                temp->next = rt->leftchild;
                rt->leftchild = temp;
                //Simulation + Update
                int result = Simulation(temp);
                Update(temp,result);
            }
            return true;
        }
        else{
            int result = Simulation(rt);
            Update(rt,result);
            return false;
        }
    }
    int Simulation(MCT *rt){
        AwariGame Game(rt->Board);
        while(-1 == Game.GameState){
            string Moves = Game.validMovesStr();
            if(Moves == ""){
                //Bug: this code should not be executed
                Game.Halt();
                printf("Early Game Break!\n");
                break;
            }
            char move = Moves[rand() % Moves.length()];
            Game.Sow(move);
        }
        rt->n++;//increase node simulation count
        SimCount++;
        int ret = Game.GameState == 2 ? 0 :
                  Game.GameState == rt->Board.player ? 1 : -1;
        return ret;
    }
    void Update(MCT *rt, int result){
        while(rt){
            //Update rt
            rt->X = (rt->N * rt->X + result)/(rt->N + 1);
            rt->N++;//increase tree simulation count
            //Move towards the root
            rt = rt->parent;
            result = -result;
        }
    }
    bool Sow(char move){
        if(!Root->leftchild){
            Expand(Root);
        }
        MCT *p = Root->leftchild, *pp = NULL;
        int I = Root->Moves.length(), i = 0;
        while(p){
            if(i >= I)
                return false;
            if(Root->Moves[i] == move){
                //Make a move
                if(pp){
                    pp->next = p->next;
                }
                else{
                    Root->leftchild = p->next;
                }
                Clear(Root);
                Root = p;
                Root->next = NULL;
                Root->parent = NULL;
                return true;
            }
            i++;
            pp = p;
            p = p->next;
        }
        return false;
    }
    void ShowNextMoves(){
        MCT *p = Root->leftchild;
        int i = 0;
        while(p){
            printf("%c(%.2f%%) ",Root->Moves[i],p->N*100.0/Root->N);
            ++i;
            p = p->next;
        }
        printf("\n");
    }
    void Show(){
        show(Root,0);
    }
    void show(MCT *rt,int depth = 0){
        if(!rt)
            return;
        for(int i = 0 ; i < depth ; ++i) 
            printf("    ");
        printf("%.2f\n",rt->X);
        MCT *temp = rt->leftchild;
        while(temp){
            show(temp,depth + 1);
            temp = temp->next;
        }
    }
    void Clear(MCT *&rt){
        if(!rt) return;
        Clear(rt->leftchild);
        Clear(rt->next);
        SimCount -= rt->n;
        delete(rt);
        rt = NULL;
    }
    ~MCTS(){
        Clear(Root);
    }
};
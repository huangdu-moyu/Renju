#include "solver.h"
#include "gamestate.h"
#include "score.h"
#include<QDebug>

const int valueUnknown=INT_MAX;


int Solver::probeHash(int dep, int alpha, int beta) {
    const HashNode& h = hashTable[st->zobrist&tableMask];
    if (h.key == st->zobrist) {
        if (h.dep >= dep) {
            if (h.flag == HashFlag::exact) {
                return h.val;
            }
            if ((h.flag == HashFlag::alpha) && (h.val <= alpha)) {
                return alpha;
            }
            if ((h.flag == HashFlag::beta) && (h.val >= beta)) {
                return beta;
            }
        }
    }
    return valueUnknown;
}

void Solver::recordHash(int dep, int val, int bestMove, HashFlag flag)
{
    HashNode& h = hashTable[st->zobrist&tableMask];
    if(h.flag!=HashFlag::unknown && h.dep>dep)
    {
        return;
    }
    h.key=st->zobrist;
    h.dep=dep;
    h.val=val;
    h.bestMove=bestMove;
    h.flag=flag;
}

int Solver::negmax(int dep, int alpha, int beta,int p)
{
    //qDebug()<<dep<<" "<<alpha<<" "<<beta<<" ";
    int val=probeHash(dep,alpha,beta);
    if(val!=valueUnknown)
    {
        return val;
    }
    if(dep==0)
    {
        int val=st->evaluate(p);
        recordHash(dep,val,0,HashFlag::exact);
        //qDebug()<<"val:"<<val;
        return val;
    }
    bool foundPV=false;
    std::vector<int> moveList=st->genMove(p);
    /*if(dep==4)
    {
        qDebug()<<(p?"黑":"白");
        for(auto u:moveList)
        {
            qDebug()<<(u-17)/16<<" "<<(u-17)%16;
        }
    }*/
    int bestmove=0;
    HashFlag hf=HashFlag::alpha;
    for(auto u:moveList)
    {
        st->putStone(u,p);
        int val;
        if(st->isWin(u))
        {
            val=FIVE;
        }
        else
        {

            if(foundPV)
            {
                val=-negmax(dep-1,-alpha-1,-alpha,p^1);
                if(val>alpha&&val<beta)
                {
                    val=-negmax(dep-1,-beta,-alpha,p^1);
                }
            }
            else {
                val=-negmax(dep-1,-beta,-alpha,p^1);
            }
        }
        st->remove(u);
        if(val>=beta)
        {
            recordHash(dep,val,u,HashFlag::beta);
            return val;
        }
        if(val>alpha)
        {
            bestmove=u;
            hf=HashFlag::exact;
            alpha=val;
            foundPV=true;
        }
    }
    recordHash(dep,val,bestmove,hf);
    return alpha;
}

int Solver::getNextMove()
{
    //memset(hashTable,0,sizeof(hashTable));
    //qDebug()<<"-------------------------------";
    double begin=clock();
    for(int i=2;i<=8;i+=2)
    {
        if((clock()-begin)/CLOCKS_PER_SEC>1)
        {
            break;
        }
        negmax(i,-INF,INF,st->player);
    }
    return hashTable[st->zobrist&tableMask].bestMove;
}

Solver::Solver()
{
    this->st=nullptr;
    memset(hashTable,0,sizeof(hashTable));
}

Solver::Solver(GameState *st)
{
    this->st=new GameState{*st};

    memset(hashTable,0,sizeof(hashTable));
}

Solver::~Solver()
{
    if(st)
    {
        delete st;
    }
}

void Solver::set(GameState *st)
{
    if(this->st)
    {
        delete this->st;
    }
    this->st=new GameState{*st};

    memset(hashTable,0,sizeof(hashTable));
}

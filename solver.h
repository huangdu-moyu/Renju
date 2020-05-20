#ifndef SOLVER_H
#define SOLVER_H

#include "gamestate.h"

typedef unsigned long long ull;

enum class HashFlag{unknown=0,exact,alpha,beta};

struct HashNode
{
    ull key;
    int dep;
    int val;
    HashFlag flag;
    int bestMove;
};

const int tableSize=64*1024*1024;
const ull tableMask=tableSize-1;

class Solver
{
public:
    GameState* st;
    HashNode hashTable[tableSize];
public:
    Solver(GameState* st);
    ~Solver();
    int evaluate();
    int negmax(int dep, int alpha, int beta, int p);
    int probeHash(int dep, int alpha, int beta);
    void recordHash(int dep,int val,int bestMove,HashFlag flag);
    int getNextMove();
};

#endif // SOLVER_H

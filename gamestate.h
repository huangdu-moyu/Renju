#ifndef GAME_H
#define GAME_H

#include <cstring>
#include <vector>
#include <QDebug>
#include <random>

class Solver;

const int INF=0x3f3f3f3f;

typedef unsigned long long ull;


class GameState
{
    static ull zobristBoard[280][2];

    //typedef std::pair<int,int> pii;
private:
    int board[280];//(x,y)->y*(N+1)+x+N+2 (both start from zero) (N=15)
    int score[280][2];//为了与坐标对应 浪费一点空间；score[0][0]与score[0][1]维护总分

    const int dir[4]={1,16,15,17};//right,down,left-down,right-down
    int player;
    ull zobrist;
    void putStone(int u,int p);
    void initScore();
    void remove(int u);
    //std::vector<pii> moves;


    bool hasNeighbor(int i, int j, int dis, int count);
    int scorePoint(int u, int p);
    void updateScore(int u);
    void updatePoint(int t);
public:
    void clear();
    int queryBoard(int x,int y);
    int queryPlayer();
    bool checkAndMove(int x,int y);
    bool isWin(int x,int y);
    int transformCorrdinate(int x,int y)
    {
        return x*16+y+17;
    }
    GameState();
    bool remove(int x, int y);
    friend Solver;
    int evaluate(int p);
    bool checkAndMove(int t);
    std::vector<int> genMove(int p);
    bool isWin(int cur);
    int num;
};

#endif // GAME_H

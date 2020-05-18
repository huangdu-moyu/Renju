#ifndef GAME_H
#define GAME_H

#include <cstring>
#include <vector>

class Game
{
    //typedef std::pair<int,int> pii;
private:
    int board[280];////(x,y)->y*(N+1)+x+N+2 (both start from zero) (N=15)
    const int dir[4]={1,16,15,17};//right,bottom,bottom left,bottom right
    int player;
    //std::vector<pii> moves;
public:
    void init();
    bool can_move();
    int queryBoard(int x,int y);
    int queryPlayer();
    bool checkAndMove(int x,int y);
    bool isWin(int x,int y);
    int TransformCoordinate(int x,int y);
    Game();
};

#endif // GAME_H

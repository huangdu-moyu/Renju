#include "game.h"

Game::Game()
{
    init();
}

void Game::init()
{
    memset(board,0x3f,sizeof(board));
    for(int i=0;i<15;i++)
    {
        for(int j=0;j<15;j++)
        {
            int t=TransformCoordinate(i,j);
            board[t]=0;
        }
    }
    player=1;
}

int Game::queryBoard(int x, int y)
{
    return board[TransformCoordinate(x,y)];
}

int Game::queryPlayer()
{
    return player;
}

bool Game::checkAndMove(int x, int y)
{
    return !queryBoard(x,y)?(void(board[TransformCoordinate(x,y)]=player),player*=-1):false;
}

bool Game::isWin(int x, int y)
{
    int cur=TransformCoordinate(x,y);
    for(int i=0;i<4;i++)
    {
        int t=cur,cnt=0;
        while(board[t]==board[cur])
        {
            t-=dir[i];
            cnt++;
        }
        t=cur+dir[i];
        while(board[t]==board[cur])
        {
            t+=dir[i];
            cnt++;
        }
        if(cnt==5)
        {
            return true;
        }
    }
    return false;
}

int Game::TransformCoordinate(int x,int y)
{
    return x*16+y+17;
}


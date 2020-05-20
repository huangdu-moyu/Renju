#include "gamestate.h"
#include "score.h"
#include <vector>
#include <QDebug>
#include<algorithm>

ull GameState::zobristBoard[280][2]={{0}};

GameState::GameState()
{
    std::random_device rd;
    std::mt19937_64 mt(rd());
    num=0;
    memset(board,0x3f,sizeof(board));
    for(int i=0;i<15;i++)
    {
        for(int j=0;j<15;j++)
        {
            int t=transformCorrdinate(i,j);
            board[t]=-1;
            if(!zobristBoard[t][0])
            {
                zobristBoard[t][0]=mt();
                zobristBoard[t][1]=mt();
            }
        }
    }
    zobrist=0;
    player=1;
    memset(score,0,sizeof(score));
}

void GameState::putStone(int u,int p)
{
    board[u]=p;
    zobrist^=zobristBoard[u][p];
    updateScore(u);
}

bool GameState::hasNeighbor(int x, int y, int dis, int count)
{
    int startX = x - dis;
    int endX = x + dis;
    int startY = y - dis;
    int endY = y + dis;
    for (int i = startX; i <= endX; i++)
    {
        if (i < 0 || i >= 15)
            continue;
        for (int j = startY; j <= endY; j++)
        {
            if (j < 0 || j >= 15 || (i == x && j == y))
                continue;
            int t = transformCorrdinate(i, j);
            if (board[t] != -1)
            {
                count--;
                if (count <= 0)
                    return true;
            }
        }
    }
    return false;
}

int countToScore(int count, int block, int empty)
{
  //没有空位
  if(empty <= 0) {
    if(count >= 5)
        return FIVE;
    if(block == 0) {
      switch(count) {
        case 1: return ONE;
        case 2: return TWO;
        case 3: return THREE;
        case 4: return FOUR;
      }
    }

    if(block == 1) {
      switch(count) {
        case 1: return BLOCKED_ONE;
        case 2: return BLOCKED_TWO;
        case 3: return BLOCKED_THREE;
        case 4: return BLOCKED_FOUR;
      }
    }

  } else if(empty == 1 || empty == count-1) {
    //第1个是空位
    if(count >= 6) {
      return FIVE;
    }
    if(block == 0) {
      switch(count) {
        case 2: return TWO/2;
        case 3: return THREE;
        case 4: return BLOCKED_FOUR;
        case 5: return FOUR;
      }
    }

    if(block == 1) {
      switch(count) {
        case 2: return BLOCKED_TWO;
        case 3: return BLOCKED_THREE;
        case 4: return BLOCKED_FOUR;
        case 5: return BLOCKED_FOUR;
      }
    }
  } else if(empty == 2 || empty == count-2) {
    //第二个是空位
    if(count >= 7) {
      return FIVE;
    }
    if(block == 0) {
      switch(count) {
        case 3: return THREE;
        case 4:
        case 5: return BLOCKED_FOUR;
        case 6: return FOUR;
      }
    }

    if(block == 1) {
      switch(count) {
        case 3: return BLOCKED_THREE;
        case 4: return BLOCKED_FOUR;
        case 5: return BLOCKED_FOUR;
        case 6: return FOUR;
      }
    }

    if(block == 2) {
      switch(count) {
        case 4:
        case 5:
        case 6: return BLOCKED_FOUR;
      }
    }
  } else if(empty == 3 || empty == count-3) {
    if(count >= 8) {
      return FIVE;
    }
    if(block == 0) {
      switch(count) {
        case 4:
        case 5: return THREE;
        case 6: return BLOCKED_FOUR;
        case 7: return FOUR;
      }
    }

    if(block == 1) {
      switch(count) {
        case 4:
        case 5:
        case 6: return BLOCKED_FOUR;
        case 7: return FOUR;
      }
    }

    if(block == 2) {
      switch(count) {
        case 4:
        case 5:
        case 6:
        case 7: return BLOCKED_FOUR;
      }
    }
  } else if(empty == 4 || empty == count-4) {
    if(count >= 9) {
      return FIVE;
    }
    if(block == 0) {
      switch(count) {
        case 5:
        case 6:
        case 7:
        case 8: return FOUR;
      }
    }

    if(block == 1) {
      switch(count) {
        case 4:
        case 5:
        case 6:
        case 7: return BLOCKED_FOUR;
        case 8: return FOUR;
      }
    }

    if(block == 2) {
      switch(count) {
        case 5:
        case 6:
        case 7:
        case 8: return BLOCKED_FOUR;
      }
    }
  } else if(empty == 5 || empty == count-5) {
    return FIVE;
  }

  return 0;
}

int GameState::scorePoint(int u,int p)
{
    int res=0;
    for(int i=0;i<4;i++)
    {
        int count=1,block=0,empty=-1;
        for(int t=u+dir[i];true;t+=dir[i])
        {
            if(board[t]==INF)
            {
                block++;
                break;
            }
            if(board[t]==-1)
            {
                if(empty==-1&&board[t+dir[i]]==p)
                {
                    empty=count;
                    continue;
                }
                else
                {
                    break;
                }
            }
            else if(board[t]!=p)
            {
                block++;
                break;
            }
            else {
                count++;
            }
        }
        for(int t=u-dir[i];true;t-=dir[i])
        {
            if(board[t]==INF)
            {
                block++;
                break;
            }
            if(board[t]==-1)
            {
                if(empty==-1&&board[t-dir[i]]==p)
                {
                    empty=0;
                    continue;
                }
                else
                {
                    break;
                }
            }
            else if(board[t]!=p)
            {
                block++;
                break;
            }
            else
            {
                count++; //secoundCount++
                if(empty!=-1)
                {
                    empty++;
                }
            }
        }
        //count+=secoundCount
        res+=countToScore(count,block,empty);
    }
    return res;
}

void GameState::initScore()
{
    /*
    score[0][0]=score[0][1]=0;
    for(int i=0;i<15;i++)
    {
        for(int j=0;j<15;j++)
        {
            int t=transformCorrdinate(i,j);
            if(board[t] == -1)
            {
                  if(hasNeighbor(i, j, 2, 2)) //以其为中心的5*5区域内 应当至少有两个棋子，否则对其打分无意义
                  {
                     score[t][0] = scorePoint(t, 0);
                     score[t][1] = scorePoint(t,1);
                  }
                  else {
                      score[t][0]=score[t][1]=0;
                  }
            }
            else
            {
                 score[t][board[t]]=scorePoint(t,board[t]);
                 score[t][board[t]^1]=0;
            }
            score[0][0]+=score[t][0];
            score[0][1]+=score[t][1];
        }
    }*/
    memset(score,0,sizeof(score));
}

void GameState::removeStone(int t)
{
    zobrist^=zobristBoard[t][board[t]];
    board[t]=-1;
    updatePoint(t);
}

void GameState::clear()
{
    memset(board,0x3f,sizeof(board));
    for(int i=0;i<15;i++)
    {
        for(int j=0;j<15;j++)
        {
            int t=transformCorrdinate(i,j);
            board[t]=-1;
        }
    }
    zobrist=0;
    player=1;
    num=0;
    moves.clear();
    initScore();
}

int GameState::queryBoard(int x, int y)
{
    return board[transformCorrdinate(x,y)];
}

int GameState::queryPlayer()
{
    return player;
}


bool GameState::checkAndMove(int x, int y)
{
    int t=transformCorrdinate(x,y);
    return checkAndMove(t);
}

bool GameState::checkAndMove(int t)
{
    if(board[t]!=-1)
    {
        return false;
    }
    putStone(t,player);
    player^=1;
    num++;
    moves.push_back(t);
    return true;
}

bool GameState::remove(int t)
{
    if(board[t]!=-1)
    {
        removeStone(t);
        player^=1;
        moves.pop_back();
        return true;
    }
    return false;
}

bool GameState::isWin(int x, int y)
{
    int cur=transformCorrdinate(x,y);
    return isWin(cur);
}

bool GameState::isWin(int cur)
{
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

int fixScore(int type)
{
    if(type < FOUR && type >= BLOCKED_FOUR) {
        if(type >= BLOCKED_FOUR && type < (BLOCKED_FOUR + THREE)) {
          //单独冲四，意义不大
          return THREE;
        } else if(type >= BLOCKED_FOUR + THREE && type < BLOCKED_FOUR * 2) {
          return FOUR;  //冲四活三，比双三分高，相当于自己形成活四
        } else {
          //双冲四 比活四分数也高
          return FOUR * 2;
        }
      }
      return type;
}

int GameState::evaluate(int p)
{
    int maxScore[2] = {0, 0};

    //遍历出最高分，开销不大

    for (int t = 17; t < 256; t++)
    {
        if (board[t] == 0)
        {
            maxScore[0] += fixScore(score[t][0]);
        }
        else if (board[t] == 1)
        {
            maxScore[1] += fixScore(score[t][1]);
        }
    }
    return maxScore[p]-maxScore[p^1];
}

void GameState::updatePoint(int t)
{
    score[0][0]-=score[t][0];
    score[0][1]-=score[t][1];
    if(board[t]!=-1)
    {
        score[t][board[t]]=scorePoint(t,board[t]);
        score[t][board[t]^1]=0;
    }
    else {
        score[t][0]=scorePoint(t,0);
        score[t][1]=scorePoint(t,1);
    }
    score[0][0]+=score[t][0];
    score[0][1]+=score[t][1];
}

void GameState::updateScore (int u) {
  const int radius = 4;

   for(int d=0;d<4;d++)
   {
       for(int i=1;i<=radius;i++)
       {
           if(board[u+i*dir[d]]==INF)
           {
               break;
           }
           updatePoint(u+i*dir[d]);
       }
       for(int i=1;i<=radius;i++)
       {
           if(board[u-i*dir[d]]==INF)
           {
               break;
           }
           updatePoint(u-i*dir[d]);
       }
   }
   updatePoint(u);
}

std::vector<int> GameState::genMove(int p)
{
    static int tscore[280];
    if(zobrist==0)
    {
        return std::vector<int>({transformCorrdinate(7,7)});
    }

    std::vector<int> fives,comfours,humfours,comblockedfours,
            humblockedfours,comtwothrees,humtwothrees,
            comthrees,humthrees,comtwos,humtwos,neighbors;
    std::vector<int> res;
    int t=17;
    for(int i=0;i<15;i++,t++)
    {
        for(int j=0;j<15;j++,t++)
        {
            if(board[t]==-1)
            {
                if(num<6)
                {
                    if(!hasNeighbor(i,j,1,1))
                    {
                        continue;
                    }
                }
                else if(!hasNeighbor(i,j,2,2))
                {
                    continue;
                }
                /*
                if(score[t][p]>=FIVE)
                {
                    fives.push_back(t);
                    return fives;
                }
                if(score[t][p^1]>=FIVE)
                {
                    fives.push_back(t);
                }
                else if(score[t][p] >= FOUR) {
                            comfours.push_back(t);
                          } else if(score[t][p^1] >= FOUR) {
              humfours.push_back(t);
            } else if(score[t][p] >= BLOCKED_FOUR) {
              comblockedfours.push_back(t);
            } else if(score[t][p^1] >= BLOCKED_FOUR) {
              humblockedfours.push_back(t);
            } else if(score[t][p] >= 2*THREE) {
              //能成双三也行
              comtwothrees.push_back(t);
            } else if(score[t][p^1] >= 2*THREE) {
              humtwothrees.push_back(t);
            } else if(score[t][p] >= THREE) {
              comthrees.push_back(t);
            } else if(score[t][p^1] >= THREE) {
              humthrees.push_back(t);
            } else if(score[t][p] >= TWO) {
              comtwos.push_back(t);
            } else if(score[t][p^1] >= TWO) {
              humtwos.push_back(t);
            } else neighbors.push_back(t);
            }*/
                res.push_back(t);
                tscore[t]=score[t][p]+score[t][p^1];
            }
        }
    }
    /*if(!fives.empty()) return fives;
    if(comfours.size()) return comfours;
    if(humfours.size()&&!comblockedfours.size()) return humfours;
#define CAT(x,y) x.insert(x.end(),y.begin(),y.end())
    if(humfours.size()&&comblockedfours.size())
    {
        //humfours.insert(humfours.end(),comblockedfours.begin(),comblockedfours.end());
        CAT(humfours,comblockedfours);
        return humfours;
    }
    CAT(comtwothrees,humtwothrees);
    CAT(comtwothrees,comblockedfours);
    CAT(comtwothrees,humblockedfours);
    CAT(comtwothrees,comthrees);
    CAT(comtwothrees,humthrees);
    CAT(comtwothrees,comtwos);
    CAT(comtwothrees,humtwos);
    CAT(comtwothrees,neighbors);
    return comtwothrees;*/
    sort(res.begin(),res.end(),[=](const int a,const int b)
    {
        return tscore[a]>tscore[b];
    });

    if(res.size()>8)
    {
        res.resize(8);
    }

    return res;
}

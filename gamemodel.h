#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include<vector>
#include<time.h>
#include<stdlib.h>
using namespace std;
enum GameType{
    MAN,
    AI
};
enum GameStatus{
    PLAYING,
    WIN,
    DEAD
};

const int BOARD_GRAD_SIZE=15;
const int MARGIN=30;
const int MARK_SIZE=6;
const int BLOCK_SIZE=40;
const int POS_OFFSET=BLOCK_SIZE*0.4;
const int AI_THINK_TIME=700;
const int CHESS_RADIUS=15;
class GameModel{
public:
    GameModel(){};
public:
    vector<vector<int>> gameMapVec;
    vector<vector<int>> scoreMapVec;
    bool playerFlag;
    GameType gameType;
    GameStatus gameStatus;

    void startGame(GameType type);
    void actionByPerson(int row,int col);
    void updateGameMap(int row,int col);
    bool isWin(int row,int col);
    void calculateScore();
    void actionByAI(int &clickRow,int &clickCol);
    /*bool isDeadGame();*/
};


#endif // GAMEMODEL_H

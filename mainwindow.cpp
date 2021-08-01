#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QMouseEvent>
#include<Qpainter>
#include<math.h>
#include<QMessageBox>
#include<QTimer>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(MARGIN*2+BLOCK_SIZE*BOARD_GRAD_SIZE,
                 MARGIN*2+BLOCK_SIZE*BOARD_GRAD_SIZE);
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, Qt::white);
    this->setPalette(palette);
    initGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::getGameType(GameType g){
    game_type=g;
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing ,true);
    for(int i=0;i<BOARD_GRAD_SIZE+1;++i){
        painter.drawLine(MARGIN+BLOCK_SIZE*i,MARGIN,
         MARGIN+BLOCK_SIZE*i,size().height()-MARGIN);
        painter.drawLine(MARGIN,MARGIN+BLOCK_SIZE*i,
         size().width()-MARGIN,MARGIN+BLOCK_SIZE*i);
    }
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    if(clickPosRow>0&&clickPosRow<BOARD_GRAD_SIZE&&
            clickPosCol>0&&clickPosCol<BOARD_GRAD_SIZE&&
            game->gameMapVec[clickPosRow][clickPosCol]==0){
        if(game->playerFlag)
            brush.setColor(Qt::black);
        else
            brush.setColor(Qt::white);
        painter.setBrush(brush);
        painter.drawRect(MARGIN+BLOCK_SIZE*clickPosCol-MARK_SIZE/2,MARGIN+BLOCK_SIZE*clickPosRow-MARK_SIZE/2,5,5);//
    }
    for(int i=0;i<BOARD_GRAD_SIZE;++i)
        for(int j=0;j<BOARD_GRAD_SIZE;++j){
            if(game->gameMapVec[i][j]==1){
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(MARGIN+BLOCK_SIZE*j-CHESS_RADIUS,MARGIN+BLOCK_SIZE*i-CHESS_RADIUS,30,30);
            }else if(game->gameMapVec[i][j]==-1){
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(MARGIN+BLOCK_SIZE*j-CHESS_RADIUS,MARGIN+BLOCK_SIZE*i-CHESS_RADIUS,30,30);
            }
        }

    if(clickPosRow>0&&clickPosRow<BOARD_GRAD_SIZE&&
            clickPosCol>0&&clickPosCol<BOARD_GRAD_SIZE&&
            (game->gameMapVec[clickPosRow][clickPosCol]==1||
             game->gameMapVec[clickPosRow][clickPosCol]==-1)){
        if(game->isWin(clickPosRow,clickPosCol)&&game->gameStatus==PLAYING){
            game->gameStatus=WIN;
            //QSound::play(":sound/win.wav");
            QString str;
            if(game->gameMapVec[clickPosRow][clickPosCol]==1)
                str="黑棋";
            else if(game->gameMapVec[clickPosRow][clickPosCol]==-1)
                str="白棋";
            QMessageBox::StandardButton btnValue=QMessageBox::information(this,"五子棋决战",str+"胜利!");
            if(btnValue==QMessageBox::Ok){
                game->startGame(game_type);
                game->gameStatus=PLAYING;
            }
        }
    }



}
void MainWindow::initGame(){
    game=new GameModel;
    initAIGame();
}
void MainWindow::initAIGame(){
    game->gameStatus=PLAYING;

    game->startGame(game_type);
    update();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    int x=event->x();
    int y=event->y();
    
    if(x>=MARGIN+BLOCK_SIZE/2&&
            x<size().width()-MARGIN-BLOCK_SIZE/2&&
            y>=MARGIN+BLOCK_SIZE/2&&
            y<size().height()-MARGIN-BLOCK_SIZE/2){
        int col=(x-MARGIN)/BLOCK_SIZE;
        int row=(y-MARGIN)/BLOCK_SIZE;
        int leftTopPosX=MARGIN+BLOCK_SIZE*col;
        int leftTopPosY=MARGIN+BLOCK_SIZE*row;
        clickPosRow=-1;
        clickPosCol=-1;
        int len=0;
        selectPos=false;
        len=sqrt((x-leftTopPosX)*(x-leftTopPosX)+(y-leftTopPosY)*(y-leftTopPosY));
        if(len<POS_OFFSET){
            clickPosRow=row;
            clickPosCol=col;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0){
                selectPos=true;
            }
        }
        len=sqrt((x-leftTopPosX-BLOCK_SIZE)*(x-leftTopPosX-BLOCK_SIZE)+(y-leftTopPosY)*(y-leftTopPosY));
        if(len<POS_OFFSET){
            clickPosRow=row;
            clickPosCol=col+1;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0){
                selectPos=true;
            }
        }
        len=sqrt((x-leftTopPosX)*(x-leftTopPosX)+(y-leftTopPosY-BLOCK_SIZE)*(y-leftTopPosY-BLOCK_SIZE));
        if(len<POS_OFFSET){
            clickPosRow=row+1;
            clickPosCol=col;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0){
                selectPos=true;
            }
        }
        len=sqrt((x-leftTopPosX-BLOCK_SIZE)*(x-leftTopPosX-BLOCK_SIZE)+(y-leftTopPosY-BLOCK_SIZE)*(y-leftTopPosY-BLOCK_SIZE));
        if(len<POS_OFFSET){
            clickPosRow=row+1;
            clickPosCol=col+1;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0){
                selectPos=true;
            }
        }
    }
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if(!selectPos){
        return;
    }else{
        selectPos=false;
    }
    chessOneByPerson();
    if(game_type==AI){
        QTimer::singleShot(AI_THINK_TIME,this,SLOT(chessOneByAI()));
    }
}

void MainWindow::chessOneByPerson(){
    if(clickPosRow!=-1&&clickPosCol!=-1&&game->gameMapVec[clickPosRow][clickPosCol]==0){
        game->actionByPerson(clickPosRow,clickPosCol);
        update();
    }
}
void MainWindow::chessOneByAI(){
    game->actionByAI(clickPosRow,clickPosCol);
    //QSound::play(":sound/chessone.wav");
    update();
}

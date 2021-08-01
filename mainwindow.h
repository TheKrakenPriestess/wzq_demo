#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"GameModel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void getGameType(GameType g);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GameModel *game;
    GameType game_type;
    
    int clickPosRow,clickPosCol;
    bool selectPos=false;

    void paintEvent(QPaintEvent *event);
    
    void initGame();
    void initAIGame();
    
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void chessOneByPerson();
private slots:
    void chessOneByAI();
};

#endif // MAINWINDOW_H

#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include "stone.h"
#include <QPainter>
#include <QMouseEvent>

class Board : public QWidget
{
    Q_OBJECT

public:
    Board(QWidget *parent = 0);
    ~Board();

    Stone _s[32];
    int _r;//棋子半径
    int _selectid;//被选择的棋子
    bool _bRedTure;//轮到谁走了

    QPoint center(int row,int col);//返回象棋棋盘行列对应的像素坐标
    QPoint center(int id);

    bool getRowCol(QPoint pt,int &row,int &col);


    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    void drawStone(QPainter &painter,int );
    int getStoneId(int ,int );//得到某点棋子的id
    bool isButtonSide(int id);//判断棋子是否在下边
    int getStoneCountAtLine(int row1,int col1,int row2,int col2);//判断2点是否在一条线上并返回这两个棋子之间有多少棋子

    bool canMove(int moveid,int row,int col,int killid);

    bool canMove1(int moveid,int row,int col,int killid);
    bool canMove2(int moveid,int row,int col,int killid);
    bool canMove3(int moveid,int row,int col,int killid);
    bool canMove4(int moveid,int row,int col,int killid);
    bool canMove5(int moveid,int row,int col,int killid);
    bool canMove6(int moveid,int row,int col,int killid);
    bool canMove7(int moveid,int row,int col,int killid);
};

#endif // BOARD_H

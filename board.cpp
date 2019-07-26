#include "board.h"
#include <QPainter>
#include <QPen>
#include "stone.h"
#include <QMouseEvent>

Board::Board(QWidget *parent)
    : QWidget(parent)
{
    for(int i=0;i<32;i++){
        _s[i].init(i);
    }
    _selectid=-1;
    _bRedTure=true;
}

Board::~Board()
{

}

void Board::paintEvent(QPaintEvent *){
    QPainter painter(this);
    int d=40;
    _r=d/2;
    QPen pen;
    pen.setWidth(2);
    painter.setPen(pen);
    //画十条横线
    for(int i=1;i<=10;i++){

        painter.drawLine(QPoint(d,d*i),QPoint(9*d,d*i));

    }
    //画9条竖线
    for(int i=1;i<10;i++){
        if(i==1||i==9)
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,10*d));
        else{
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,5*d));
            //因为楚河汉界所有竖线中间要短开
            painter.drawLine(QPoint(i*d,6*d),QPoint(i*d,10*d));
        }
    }

    //9宫格
    painter.drawLine(QPoint(4*d,d),QPoint(6*d,3*d));
    painter.drawLine(QPoint(4*d,3*d),QPoint(6*d,d));
    painter.drawLine(QPoint(4*d,8*d),QPoint(6*d,10*d));
    painter.drawLine(QPoint(4*d,10*d),QPoint(6*d,8*d));

    //绘制32个棋子
    for(int i=0;i<32;i++){
        drawStone(painter,i);
    }


}

void Board::drawStone(QPainter &painter, int id){
    if(_s[id]._dead)
        return;

    QPoint c=center(id);
    QRect rect=QRect(c.x()-_r,c.y()-_r,_r*2,_r*2);

    if(id==_selectid)
        painter.setBrush(QBrush(Qt::gray));
    else
        painter.setBrush(QBrush(Qt::yellow));

    painter.setPen(Qt::black);

    painter.drawEllipse(center(id),_r,_r);//绘制棋子的圆

    if(_s[id]._red)
        painter.setPen(Qt::red);
    painter.setFont(QFont("system",_r,700));
    painter.drawText(rect,_s[id].getText(),QTextOption(Qt::AlignCenter));//绘制棋子的字


}

QPoint Board::center(int row, int col){
    QPoint ret;
    ret.rx()=(col+1)*_r*2;
    ret.ry()=(row+1)*_r*2;
    return ret;
}

QPoint Board::center(int id){
    return center(_s[id]._row,_s[id]._col);
}

void Board::mouseReleaseEvent(QMouseEvent *ev){
    QPoint pt=ev->pos();
    int row,col;
    //首先将pt转化为象棋的行列值
    //判断这个行列值上有没有棋子
    bool bRet=getRowCol(pt,row,col);
    if(bRet==false)//点到棋盘外
        return;

    int clickid=-1;//棋子id
    int i;
    for(i=0;i<32;++i){
        if(_s[i]._row==row&&_s[i]._col==col&&_s[i]._dead==false){
            break;
        }
    }
    if(i<32){
        clickid=i;//获取被点中的棋子id
    }
    if(_selectid==-1){//初次没有被点中的棋子，则开始点棋子
        if(clickid!=-1){//第一个被点中的棋子
            if(_bRedTure ==_s[clickid]._red){
                _selectid=clickid;//将该点中的棋子id保存在_selectid中
                update();
            }
        }
    }else{//该棋子是被点中的，则开始走棋子
        if(canMove(_selectid,row,col,clickid)){
            /*走棋*/
            _s[_selectid]._row=row;
            _s[_selectid]._col=col;
            if(clickid!=-1){
                _s[clickid]._dead=true;
            }
            _selectid =-1;
            _bRedTure=!_bRedTure;
            update();
        }
    }
}


//效率不搞
bool Board::getRowCol(QPoint pt, int &row, int &col){
    for(row=0;row<=9;row++){
        for(col=0;col<=8;col++){
            QPoint c=center(row,col);
            int dx=c.x()-pt.x();
            int dy=c.y()-pt.y();
            int dist=dx*dx+dy*dy;
            if(dist<_r*_r)
                return true;
        }
    }
    return false;
}

bool Board::canMove(int moveid, int row, int col, int killid){
    if(killid!=-1){
        if(_s[moveid]._red==_s[killid]._red){//判断是否同一方
            //换选择
            _selectid=killid;
            update();

            return false;
        }else{
            switch(_s[moveid]._type){
            case Stone::JIANG:
                return canMove1(moveid,row,col,killid);
                break;
            case Stone::SHI:
                return canMove2(moveid,row,col,killid);
                break;
            case Stone::XIANG:
                return canMove3(moveid,row,col,killid);
                break;
            case Stone::MA:
                return canMove5(moveid,row,col,killid);
                break;
            case Stone::CHE:
                return canMove4(moveid,row,col,killid);
                break;
            case Stone::BING:
                return canMove7(moveid,row,col,killid);
                break;
            case Stone::PAO:
                return canMove6(moveid,row,col,killid);
                break;
            }
        }
    }else{
        switch(_s[moveid]._type){
        case Stone::JIANG:
            return canMove1(moveid,row,col,killid);
            break;
        case Stone::SHI:
            return canMove2(moveid,row,col,killid);
            break;
        case Stone::XIANG:
            return canMove3(moveid,row,col,killid);
            break;
        case Stone::MA:
            return canMove5(moveid,row,col,killid);
            break;
        case Stone::CHE:
            return canMove4(moveid,row,col,killid);
            break;
        case Stone::BING:
            return canMove7(moveid,row,col,killid);
            break;
        case Stone::PAO:
            return canMove6(moveid,row,col,killid);
            break;
        }
    }
        return true;
}

bool Board::canMove1(int moveid,int row,int col,int killid){
    /*
        将
        1.目标任务在九宫格内
        2.移动的步长是一个格子
    */
    if(_s[moveid]._red){
        if(row>2)
            return false;
    }else{
        if(row<7)
            return false;
    }
    if(col<3||col>5)
        return false;

    int dr=_s[moveid]._row-row;
    int dc=_s[moveid]._col-col;
    int d=abs(dr)*10+abs(dc);//这样就转化为了10和1的比较
    if(d==1||d==10)//当为10就是行走了一步，当为1就是列走了一步,这样就必须是只能走一步了
        return true;

    return false;
}

bool Board::canMove2(int moveid,int row,int col,int killid){
    /*
        将
        1.目标任务在九宫格内
        2.必须斜着移动
    */
    if(_s[moveid]._red){
        if(row>2)
            return false;
    }else{
        if(row<7)
            return false;
    }
    if(col<3||col>5)
        return false;

    int dr=_s[moveid]._row-row;
    int dc=_s[moveid]._col-col;
    int d=abs(dr)*10+abs(dc);//这样就转化为了10和1的比较
    if(d==11)//当为10就是行走了一步，当为1就是列走了一步,这样就必须是只能走一步了
        return true;

    return false;

    return true;
}

bool Board::canMove3(int moveid,int row,int col,int){//第四个参数不用，所以叫哑言，可以不写
    /*
     象飞田
     但象心不能有棋子
    */
    int d=abs(_s[moveid]._row-row)*10+abs(_s[moveid]._col-col);//关系值
    if(d!=22)
        return false;
    int rEye=(row+_s[moveid]._row)/2;
    int cEye=(col+_s[moveid]._col)/2;
    if(getStoneId(rEye,cEye)!=-1)
        return false;
    if(isButtonSide(moveid)){
        if(row<=5)
            return false;
    }else{
        if(row>5)
            return false;
    }

    return true;
}

bool Board::canMove4(int moveid,int row,int col,int killid){
    /*
     车走直线，看两点之间的情况
    */
    int ret=getStoneCountAtLine(_s[moveid]._row,_s[moveid]._col,row,col);
    if(ret==0)
        return true;
    return false;
}

bool Board::canMove5(int moveid,int row,int col,int killid){
    /*
     马踏日字但是会被别腿
    */
    int d=abs(_s[moveid]._row-row)*10+abs(_s[moveid]._col-col);//关系值
    if(d!=12 && d!=21)
        return false;
    if(d==12){
        if(getStoneId(_s[moveid]._row,(_s[moveid]._col+col)/2)!=-1)
            return false;
    }else{
        if(getStoneId((_s[moveid]._row+row)/2,_s[moveid]._col)!=-1)
            return false;
    }
    return true;
}

bool Board::canMove6(int moveid,int row,int col,int killid){
    /*
     * 炮打翻山
    */

    int ret =getStoneCountAtLine(_s[moveid]._row,_s[moveid]._col,row,col);
    if(killid!=-1){//判断是否要杀对面棋子
        if(ret==1)
            return true;
    }else{
        if(ret==0)
            return true;
    }
    return false;
}

bool Board::canMove7(int moveid,int row,int col,int killid){
    /*
     * 兵过河才能左右走,且不能回头
    */
    int d=abs(_s[moveid]._row-row)*10+abs(_s[moveid]._col-col);//关系值
    int row1=_s[moveid]._row;
    if(d!=1&&d!=10)
        return false;
    if(isButtonSide(moveid)){
        if(row > row1){
            return false;
        }
        if(row1 >= 5 && row==row1){
            return false;
        }
        return true;
    }else{
        if(row < row1){
            return false;
        }
        if(row1 <= 4 && row == row1){
            return false;
        }
        return true;
    }
}

int Board::getStoneId(int row,int col){//判断某坐标是否存在棋子
    //找到该行该列的id
    int i,id=-1;
    for(i=0;i<32;i++){
        if(_s[i]._row==row&&_s[i]._col==col){
            id=_s[i]._id;
            break;
        }
    }
    return id;
}

bool Board::isButtonSide(int id){//判断棋子是否在下面
    if(_s[id]._red)
        return false;
    else
        return true;
}

int Board::getStoneCountAtLine(int row1,int col1,int row2,int col2){
    int ret=0;
    if(row1!=row2&&col1!=col2)
        return -1;
    if(row1==row2&&col1==col2)
        return -1;
    if(row1==row2){
        int min=col1<col2?col1:col2;
        int max=col1<col2?col2:col1;
        for(int col=min+1;col<max;col++){
            if(getStoneId(row1,col)!=-1)
                ret++;
        }
    }else{
        int min=row1<row2?row1:row2;
        int max=row1<row2?row2:row1;
        for(int row=min+1;row<max;row++){
            if(getStoneId(row,col1)!=-1)
                ret++;
        }
    }
    return ret;
}

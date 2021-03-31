#include "gameengine.h"
#include <QDebug>
#include <QTimer>

QPoint dpos[6];

gameengine::gameengine(QWidget *parent) : QGLWidget(parent)
{
    this->grabKeyboard();
    initblock();
    color[0]="#C1B4A7"; color[2]="#EEE4DA"; color[4] = "#EDE0C8"; color[8]="#F2B179";
    color[16]="#F2B179"; color[32]="#F67C5F";color[64]="#F65E3B"; color[128] = "#EDCF72";
    dpos[0] = QPoint(-25, 0);
    dpos[1] = QPoint(25, 0);
    dpos[2] = QPoint(0, -25 );
    dpos[3] = QPoint(0, 25 );
    dpos[4] = QPoint(-4, -4);
    connect(&timer,&QTimer::timeout,[=](){ update(); } );
}

void gameengine::initblock()
{
    memset( block,0,sizeof block );
    block[rand()%4][rand()%4] = 2;
    int i = rand()%4, j = rand()%4;
    while( block[i][j] )    i = rand()%4, j = rand()%4;
    block[i][j] = 2;
    digcount = 2;
}

void gameengine::keyPressEvent(QKeyEvent *e)
{
    if( isanimation )   return;//如果在播放动画,那就不响应
    animation a;
    bool combine[5][5];//每个格子是否参与过合并
    memset( combine,0,sizeof combine );
    int temp = 0;//本次操作加的分数
    switch(e->key() )
    {
        case Qt::Key_Left:
            for(int i=0;i<4;i++)
            for(int j=0;j<4;j++)
            {
                if( block[i][j]==0 )    continue;
                for(int k=0;k<=j;k++)
                {
                    if( block[i][k]&&j!=k )   continue;
                    a.startpos = QPoint(j*100+5,i*100+5);
                    a.endpos = QPoint(k*100+5,i*100+5);
                    a.type = Move;  a.dirct = LEFT;
                    a.dig1 = a.dig2 = block[i][j];
                    qSwap( block[i][j],block[i][k] );
                    if( k>=1&&block[i][k]==block[i][k-1]&&!combine[i][k-1] )
                    {
                        temp += block[i][k];
                        combine[i][k-1] = 1;//被合并
                        block[i][k-1] <<= 1; block[i][k] = 0; digcount--;
                        a.endpos = QPoint( (k-1)*100+5,i*100+5 );
                        a.dig2 = block[i][k-1];
                    }
                    list.append(a);
                    break;
                }
            }
            break;
        case Qt::Key_Right:
            for(int i=0;i<4;i++)
            for(int j=3;j>=0;j--)
            {
                if( block[i][j]==0 )    continue;
                for(int k=3;k>=j;k--)
                {
                    if( block[i][k]&&j!=k )   continue;
                    a.startpos = QPoint(j*100+5,i*100+5);
                    a.endpos = QPoint(k*100+5,i*100+5);
                    a.type = Move; a.dirct = RIGHT;
                    a.dig1 = a.dig2 = block[i][j];
                    qSwap( block[i][j],block[i][k] );
                    if( k<3&&block[i][k]==block[i][k+1]&&!combine[i][k+1] )
                    {
                        temp += block[i][k];
                        combine[i][k+1] = 1;//被合并
                        block[i][k+1] <<= 1; block[i][k] = 0; digcount--;
                        a.endpos = QPoint( (k+1)*100+5,i*100+5 );
                        a.dig2 = block[i][k+1];
                    }
                    list.append(a);
                    break;
                }
            }
            break;
        case Qt::Key_Up:
            for(int i=0;i<4;i++)//枚举列
            for(int j=0;j<4;j++)//枚举行
            {
                if( block[j][i]==0 )    continue;
                for(int k=0;k<=j;k++)
                {
                    if( block[k][i]&&j!=k )   continue;
                    a.startpos = QPoint(i*100+5,j*100+5);
                    a.endpos = QPoint(i*100+5,k*100+5);
                    a.type = Move;     a.dirct = UP;
                    a.dig1 = a.dig2 = block[j][i];
                    qSwap( block[j][i],block[k][i] );
                    if( k>=1&&block[k][i]==block[k-1][i]&&!combine[k-1][i] )
                    {
                        temp += block[k][i];
                        combine[k-1][i] = 1;//被合并
                        block[k-1][i] <<= 1; block[k][i] = 0; digcount--;
                        a.endpos = QPoint( i*100+5,(k-1)*100+5 );
                        a.dig2 = block[k-1][i];
                    }
                    list.append(a);
                    break;
                }
            }
            break;
        case Qt::Key_Down:
            for(int i=0;i<4;i++)//枚举列
            for(int j=3;j>=0;j--)//枚举行
            {
                if( block[j][i]==0 )    continue;
                for(int k=3;k>=j;k--)
                {
                    if( block[k][i]&&j!=k )   continue;
                    a.startpos = QPoint(i*100+5,j*100+5);
                    a.endpos = QPoint(i*100+5,k*100+5);
                    a.type = Move;     a.dirct = DOWN;
                    a.dig1 = a.dig2 = block[j][i];
                    qSwap( block[j][i],block[k][i] );
                    if( k<3&&block[k][i]==block[k+1][i]&&!combine[k+1][i] )
                    {
                        temp += block[k][i];
                        combine[k+1][i] = 1;//被合并
                        block[k+1][i] <<= 1; block[k][i] = 0; digcount--;
                        a.endpos = QPoint( i*100+5,(k+1)*100+5 );
                        a.dig2 = block[k+1][i];
                    }
                    list.append(a);
                    break;
                }
            }
            break;
    }
    if( digcount!=16 )//没有填满,需要产生新的数字
    {
        int i = rand()%4, j = rand()%4;
        while( block[i][j] )    i = rand()%4, j = rand()%4;
        block[i][j] = ( rand()%2+1 )*2;//随机生成数字
        a.type = Appear;
        a.startpos = a.endpos = QPoint(i*100+5,j*100+5);
        a.startpos += QPoint(50,50);
        a.dig1 = block[i][j];
        digcount++;
    }
    score += temp;
    emit update_score(score,temp);//发出信号告诉需要加分了
    isanimation = true;
    // 启动计时器
    timer.start(10);
}

bool gameengine::playanimation(animation &a,QPainter& painter)
{
    bool ok = false;
    if( a.type==1 )//移动的特效
    {
        switch( a.dirct )
        {
            case LEFT:
                if( a.startpos.x()>a.endpos.x() )   a.startpos += dpos[LEFT];
                else    a.startpos = a.endpos, ok = true;
                break;
            case RIGHT:
                if( a.startpos.x()<a.endpos.x() )   a.startpos += dpos[RIGHT];
                else    a.startpos = a.endpos, ok = true;
                break;
            case UP:
                if( a.startpos.y()>a.endpos.y() )   a.startpos += dpos[UP];
                else    a.startpos = a.endpos, ok = true;
                break;
            case DOWN:
                if( a.startpos.y()<a.endpos.y() )   a.startpos += dpos[DOWN];
                else    a.startpos = a.endpos, ok = true;
                break;
        }
        if( ok )//到终点了
        {
            int x = a.dig2;
            QBrush res(QColor(color[x]));
            painter.setBrush(res);
            painter.setPen(Qt::NoPen);
            painter.drawRoundRect(a.startpos.x(),a.startpos.y(),90,90);
            painter.setPen(QColor::fromRgb(0,0,0));
            painter.drawText(a.startpos.x(),a.startpos.y(),90,90,Qt::AlignCenter,QString::number(a.dig2));
        }
        else//没到终点
        {
            int x = a.dig1;
            QBrush res(QColor(color[x]));
            painter.setBrush(res);
            painter.setPen(Qt::NoPen);
            painter.drawRoundRect(a.startpos.x(),a.startpos.y(),90,90);
            painter.setPen(QColor::fromRgb(0,0,0));
            painter.drawText(a.startpos.x(),a.startpos.y(),90,90,Qt::AlignCenter,QString::number(a.dig1));
        }
    }
    else
    {
        if( a.startpos.x()>a.endpos.x() )   a.startpos += dpos[4];
        else    a.startpos = a.endpos, ok = true;
        int x = a.dig2;
        QBrush res(QColor(color[x]));
        painter.setBrush(res);
        painter.setPen(Qt::NoPen);
        painter.drawRoundRect(a.startpos.x(),a.startpos.y(),90,90);
        painter.setPen(QColor::fromRgb(0,0,0));
        painter.drawText(a.startpos.x(),a.startpos.y(),90,90,Qt::AlignCenter,QString::number(a.dig1));
    }
    return ok;
}

void gameengine::drawbackground(QPainter &painter)
{
    painter.setRenderHint(QPainter::Antialiasing);//去掉锯齿
    QBrush temp(QColor("#BBADA0"));
    painter.setBrush(temp);
    painter.setPen(Qt::NoPen);//画之前设置为空笔,否则会画出边框来
    painter.drawRoundRect(0,0,400,400,10,10);
    QFont font;
    font.setPixelSize(50); font.setFamily("Consolas");
    font.setBold(true);//设置粗体
    painter.setFont(font);

    for(int i=0;i<4;i++)
    for(int j=0;j<4;j++)
    {
        int zero = 0;
        QBrush res(QColor(color[zero]));
        painter.setBrush(res);
        painter.setPen(Qt::NoPen);
        painter.drawRoundRect(j*100+5,i*100+5,90,90);
    }
}

void gameengine::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    drawbackground(painter);//绘制背景
    if( isanimation )//如果需要播放动画走这个分支
    {

        bool ok = true;
        for(QList<animation>::iterator it=list.begin(); it!=list.end(); it++ )
        {
            if( !playanimation(*it,painter) )    ok = false;//只要有一个没画好就整体没画好
        }
        if( ok==false ) return;
        timer.stop();
        list.clear();
        if( checkgameover() )   emit over();//发送游戏失败的信号
        if( checkgamewin() )    emit win();//发送游戏胜利的信号
        isanimation = false;
    }
    else//不需要播放动画,普通绘制就好了
    {
        for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
        {
            QBrush res(QColor(color[block[i][j]]));
            painter.setBrush(res);
            painter.setPen(Qt::NoPen);
            painter.drawRoundRect(j*100+5,i*100+5,90,90);
            if( block[i][j]==0 )    continue;
            painter.setPen(QColor::fromRgb(0,0,0));
            painter.drawText(j*100+5,i*100+5,90,90,Qt::AlignCenter,QString::number(block[i][j]));
        }
    }
}

void gameengine::Restart_game()
{
    initblock();
    update();
}

bool gameengine::checkgameover()
{
    for(int i=0;i<=3;i++)
    for(int j=0;j<=3;j++)
        if( block[i][j]==0) return false;
    for(int i=0;i<=3;i++)
    for(int j=0;j<=3;j++)
        if( j>=1&&block[i][j]==block[i][j-1] ) return false;
        else if( i>=1&&block[i][j]==block[i-1][j] ) return false;
    return true;
}

bool gameengine::checkgamewin()
{
    for(int i=0;i<=3;i++)
    for(int j=0;j<=3;j++)
        if( block[i][j]==16 )   return true;
    return false;
}

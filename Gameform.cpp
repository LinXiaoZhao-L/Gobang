#include "Gameform.h"

Gameform::Gameform(QWidget* parent)
    : QWidget(parent)
{
    InitChess();
}

//������������,��굥������
void Gameform::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.drawPixmap(this->rect(), QPixmap("../Gobang/image/bg.jpg")); //���̱���

    //���û���
    QPen pen_1;
    pen_1.setColor(Qt::black);
    pen_1.setStyle(Qt::SolidLine);
    pen_1.setWidth(2);
    painter.setPen(pen_1);

    //������
    for (int i = 0; i <= 14; i++) {
        painter.drawLine(startX, startY + i * gridHeight, 15 * gridWidth, startY + i * gridHeight);
        painter.drawLine(startX + i * gridWidth, startY, startX + i * gridWidth, 15 * gridHeight);
    }

    //������
    QString chessfilename;
    for (int i = 0; i < 14; i++) {
        for (int j = 0; j < 14; j++) {
            if (chessdata[i][j] == white) {
                chessfilename = "../Gobang/image/white1.png";
            }
            else if (chessdata[i][j] == black) {
                chessfilename = "../Gobang/image/black1.png";
            }
            else {
                chessfilename.clear();
                continue;
            }
            painter.drawPixmap(startX + i * gridWidth, startY + j * gridHeight, gridWidth, gridHeight, QPixmap(chessfilename));
        }
    }
}
void Gameform::resizeEvent(QResizeEvent* event)
{
    gridWidth = event->size().width() / GridCount;
    gridHeight = event->size().height() / GridCount;
    startX = gridWidth;
    startY = gridHeight;
}
void Gameform::mousePressEvent(QMouseEvent* event)
{
    //������¼�����������[x,y]
    int x = event->x();
    int y = event->y(); 

    if (x >= startX && (x <= startX + 14 * gridWidth)) {
        if (y >= startY && (y <= startY + 14 * gridHeight)) {
            
            //ת������Ϊ��������[i,j]
            int i = 0, j = 0;
            i = (x - startX) / gridWidth;
            j = (y - startY) / gridHeight;

            this->update();
            SignalSendChessdata(i, j);
        }
    }
}

//��ʼ�����̺������ź�����
void Gameform::InitChess()
{
    for (int i = 0; i < 14; i++) {
        for (int j = 0; j < 14; j++) {
            chessdata[i][j] = Empty;
        }
    }
}
void Gameform::setChessStatus(void* p) 
{
    memcpy(chessdata, p, sizeof(int) * 14 * 14);
    this->update();
}
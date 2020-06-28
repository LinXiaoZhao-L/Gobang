#pragma once

#include <QWidget>
#include "ui_Game.h"
#include <QPainter>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QDebug>

#define GridCount 16

class Gameform : public QWidget
{
	Q_OBJECT

public:
	Gameform(QWidget* parent = Q_NULLPTR);

	//��������
	enum ChessType
	{
		Empty = 0,
		white,
		black
	};

	// ���ӵĿ�ߣ�������ʼ��
	int gridWidth, gridHeight, startX, startY;
	int chessdata[14][14];

	//�ı����̵���ʾֵ
	void setChessStatus(void* p);

protected:
	void paintEvent(QPaintEvent*);
	void resizeEvent(QResizeEvent*);
	void mousePressEvent(QMouseEvent*);

private:
	void InitChess();

signals:
	void SignalSendChessdata(int i, int j);//���͵��������
};
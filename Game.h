#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_Game.h"
#include <QPainter>
#include "Gameform.h"
#include "ComeGobang.h"
#include <QDebug>
#include <QMessagebox>
#include <QTime> 
#include <ctime>  

#define PTIF 2147483647  //正无穷 Beta
#define NGIF -2147483648 //负无穷 Alpha

class Game : public QWidget
{
	Q_OBJECT

public:
	Game(QWidget *parent = Q_NULLPTR);
	~Game();

	enum PKtype{offensive,defensive}; // 先手和后手

	int AI_X, AI_Y;//AI落子
	typedef struct
	{
		int data;
		int Alpha;
		int Beta;
		int X;
		int Y;
	}Tree;   // 树结构

protected:
	void paintEvent(QPaintEvent*);

	void RoleInit(const QString whitefilename, const QString blackfilename);
	void setRole(Gameform::ChessType Role);
	void Rolechange();
	Gameform::ChessType currentRole;
	PKtype currentPK;

	int ChessData[14][14]; int HistoryData[14][14];
	int Score[14][14]; 
	int Player_score[14][14]; int AI_score[14][14];
	void setChessInit();

	void FindbestScore(int &x,int &y, Gameform::ChessType Role);
	void ChessScore(Gameform::ChessType Role);

	bool chessCan(int x, int y, int n);
	int chessScore(int n, Gameform::ChessType Role, int CHESSData[14][14]);
	int GetMinMaxScore(int Step,int Alpha, int Beta,Gameform::ChessType Role, int CHESSDATA[14][14]);

	void GobangWIN(const char* Player);

	void Sleep(int msec);
	
private slots:
	void on_btn_start_clicked();
	void on_btn_regret_clicked();
	void on_btn_peace_clicked();
	void on_btn_giveup_clicked();

	void doProcess(int i,int j);

private:
	Ui::Game ui;

	Gameform* myform;
	void Init();
	void playwithAI(Gameform::ChessType Role);
	int checkwin(int color, void* myform);
	bool checkpeace();
};

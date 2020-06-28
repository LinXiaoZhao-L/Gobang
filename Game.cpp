#include "Game.h"

Game::Game(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QObject::connect(ui.btn_start, SIGNAL(clicked()), this, SLOT(on_btn_start_clicked()), Qt::UniqueConnection);
	QObject::connect(ui.btn_regret, SIGNAL(clicked()), this, SLOT(on_btn_regret_clicked()),Qt::UniqueConnection);
	QObject::connect(ui.btn_peace, SIGNAL(clicked()), this, SLOT(on_btn_peace_clicked()), Qt::UniqueConnection);
	QObject::connect(ui.btn_giveup, SIGNAL(clicked()), this, SLOT(on_btn_giveup_clicked()), Qt::UniqueConnection);

	Init();
}
Game::~Game()
{
}

void Game::Init()
{
	//申请棋盘类空间
	myform = new Gameform();
	connect(myform, SIGNAL(SignalSendChessdata(int, int)), this, SLOT(doProcess(int, int)));

	//显示棋盘
	ui.grid_chess->addWidget(myform);

	RoleInit("../Gobang/image/black.png", "../Gobang/image/white.png");
	currentRole = Gameform::Empty;

}

//添加背景
void Game::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.drawPixmap(this->rect(), QPixmap("../Gobang/image/bg.png"));
}

//棋盘初始化、发送坐标
void Game::setChessInit()
{
	for (int i = 0; i < 14; i++) {
		for (int j = 0; j < 14; j++) {
			ChessData[i][j] = Gameform::Empty;
		}
	}
	if (currentPK == defensive)  //AI先手，默认在[7，7]处落黑子
		ChessData[6][6] = Gameform::black;
	myform->setChessStatus(ChessData);
}
void Game::doProcess(int i,int j)
{
	memcpy(HistoryData, ChessData, sizeof(int) * 14 * 14);

	qDebug() << "玩家：" << i << "," << j;
	if (ChessData[i][j] == Gameform::Empty) {
		ChessData[i][j] = currentRole;
		myform->setChessStatus(ChessData);
		GobangWIN("YOU");
		Rolechange();

		Sleep(500);

		playwithAI(currentRole);
		GobangWIN("AI");
		Rolechange();
	}
	
}

//黑白子落子显示、切换落子显示、轮换落子
void Game::RoleInit(const QString blackfilename, const QString whitefilename)
{
	ui.label_black->setPixmap(QPixmap(blackfilename));
	ui.label_black->setScaledContents(true);
	ui.label_white->setPixmap(QPixmap(whitefilename));
	ui.label_white->setScaledContents(true);
}
void Game::setRole(Gameform::ChessType Role)
{
	currentRole = Role;
	if (Role == Gameform::black)
		RoleInit("../Gobang/image/black.png", "../Gobang/image/blank.png");
	else
		RoleInit("../Gobang/image/blank.png", "../Gobang/image/white.png");
}
void Game::Rolechange()
{
	if (currentRole == Gameform::black)
		currentRole = Gameform::white;
	else
		currentRole = Gameform::black;
	setRole(currentRole);
}

//AI落子函数
void Game::playwithAI(Gameform::ChessType Role)
{
	/*                    难度一(实现)                      */
	/* playwithAI(int& x, int& y,Gameform::ChessType Role) */

	//FindbestScore(x,y,Role);
	//myform->setChessStatus(ChessData);

	/*                    难度二(未实现)                    */
	/*         playwithAI(Gameform::ChessType Role)        */
	GetMinMaxScore(2, NGIF, PTIF, Role, ChessData);
	qDebug() << "AI:" << AI_X << "," << AI_Y;
	ChessData[AI_X][AI_Y] = Role;
	myform->setChessStatus(ChessData);
}

//重新开始与退出游戏，判断五子连珠、判断平局
void Game::GobangWIN(const char* Player)
{
	if (checkwin(currentRole, ChessData) == 1) {
		qDebug() << Player << "Win!\nGame Over!";
		int Restart = QMessageBox::question(NULL, "大佬，请", "想重新开始吗？骚年", QMessageBox::Yes | QMessageBox::No);
		if (Restart == QMessageBox::Yes) {
			this->hide();
			ComeGobang::to_Gobang->show(); //返回主窗口
		}
		else
			this->close();
	}
	else if (checkpeace() == true) {
		qDebug() << "World Peace!";
		int Restart = QMessageBox::question(NULL, "大佬，请", "想重新开始吗？骚年", QMessageBox::Yes | QMessageBox::No);
		if (Restart == QMessageBox::Yes) {
			this->hide();
			ComeGobang::to_Gobang->show(); //返回主窗口
		}
		else
			this->close();
	}
}
int Game::checkwin(int color, void* myform)
{
	typedef int(*p)[14];
	p chessData = p(myform);
	for (int i = 0; i < 14; i++) {              // 遍历数组查找五子连珠
		for (int j = 0; j < 14; j++) {

			//和右边4子连成5个
			if (i < 10
				&& chessData[i][j] == color
				&& chessData[i + 1][j] == color
				&& chessData[i + 2][j] == color
				&& chessData[i + 3][j] == color
				&& chessData[i + 4][j] == color)
			{
				return 1;
			}
			//和下边4子连成5个
			if (j < 10
				&& chessData[i][j] == color
				&& chessData[i][j + 1] == color
				&& chessData[i][j + 2] == color
				&& chessData[i][j + 3] == color
				&& chessData[i][j + 4] == color)
			{
				return 1;
			}
			//和左下4子连成5个
			if (i > 3 && j < 10
				&& chessData[i][j] == color
				&& chessData[i - 1][j + 1] == color
				&& chessData[i - 2][j + 2] == color
				&& chessData[i - 3][j + 3] == color
				&& chessData[i - 4][j + 4] == color)
			{
				return 1;
			}
			//和右下4子连成5个
			if (i < 10 && j < 10
				&& chessData[i][j] == color
				&& chessData[i + 1][j + 1] == color
				&& chessData[i + 2][j + 2] == color
				&& chessData[i + 3][j + 3] == color
				&& chessData[i + 4][j + 4] == color)
			{
				return 1;
			}
		}
	}
	return 0;
}
bool Game::checkpeace()
{
	int count = 0;
	for (int i = 0; i < 14; i++) {
		for (int j = 0; j < 14; j++) {
			if (ChessData[i][j] == Gameform::Empty)
				count++;
		}
	}
	return (count == 0);
}

//游戏开始(选择先后手)、悔棋(未实现)、求和、认负
void Game::on_btn_start_clicked()
{
	int ret = QMessageBox::question(NULL, "大佬，请", "是否选择先下", QMessageBox::Yes | QMessageBox::No);
	if (ret == QMessageBox::Yes) {
		currentPK = offensive;
		setRole(Gameform::black);
	}
	else {
		currentPK = defensive;
		setRole(Gameform::white);
	}
	setChessInit();
}
void Game::on_btn_regret_clicked()
{
	int ret = QMessageBox::question(NULL, "哥", "你真的要悔棋吗", QMessageBox::Yes | QMessageBox::No);
	if (ret == QMessageBox::Yes) {
		memcpy(ChessData, HistoryData, sizeof(int) * 14 * 14);
		myform->setChessStatus(ChessData);
	}
}
void Game::on_btn_peace_clicked()
{
	qDebug() << "就这？！";
	GobangWIN("Nobody");
}
void Game::on_btn_giveup_clicked()
{
	qDebug() << "你个菜鸡";
	GobangWIN("AI");
}


/*      难度一 实现函数     */
/*         评分函数        */
/*      查找最高分坐标     */
void Game::ChessScore(Gameform::ChessType Role)
{
	// 设置双方颜色(因为涉及换手，不能直接固定颜色)
	int AI_role, Player_role;
	if (Role == Gameform::black) {AI_role = Role; Player_role = Gameform::white; }
	else {AI_role = Role;Player_role = Gameform::black; }

	memset(Score, 0, sizeof(Score));  //初始化评分数组
	int Player_num = 0, AI_num = 0;   //玩家或AI棋子连成统计个数
	int empty = 0;  //空点统计个数

    /* 
	防守
    对每一次落子进行全盘的遍历，若某处为空白，检查其四周八个方向（在边缘位置判断边界）
    若在空白处，且发现在某一对角线方向发现有一个其他颜色的棋子，空白位置的分数数组赋分
    继续往这个方向检测是否还有连续的同一颜色的棋子，若没有则检查其他方向或者检测下一个空白位置。
	依次类推，继续检测。每一遍遍历都会使得分数数组得到累加，会导致AI判断的变化。

	进攻
    在防守分数数组的基础上，对本身棋子也进行类似的遍历，判断自己的形势，对分数数组赋分。
	*/

	for (int x = 0; x < 14; x++) {
		for (int y = 0; y < 14; y++) {
			if (ChessData[x][y] == Gameform::Empty) {

				/*判断该空白棋子的八个方向 */
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						if (i != 0 || j != 0) {  // [i,j]=[0,0]就是空白棋子位置

							/* 对玩家棋子计数 */
							for (int k = 1; k <= 4; k++) {
								if (x + k * i >= 0 && x + k * i < 14 && y + k * j >= 0 && y + k * j < 14  //边界判断
									&& ChessData[x + k * i][y + k * j] == Player_role)
									Player_num++;
								else if (ChessData[x + k * i][y + k * j] == Gameform::Empty) { empty++; break; }  //空点+1退出
								else { break; }
							}
							for (int k = -1; k >= -4; k--) {
								if (x + k * i >= 0 && x + k * i < 14 && y + k * j >= 0 && y + k * j < 14
									&& ChessData[x + k * i][y + k * j] == Player_role)
									Player_num++;
								else if (ChessData[x + k * i][y + k * j] == Gameform::Empty) { empty++; break; }  //空点+1退出
								else { break; }
							}

							/* 对玩家棋子评分 */                                 //  与空白棋子可形成的阵型
							if (Player_num == 1) { Score[x][y] += 1; }          //  2个棋子  +   1分 单子
							else if (Player_num == 2) {                         //  3个棋子
								if (empty == 1) { Score[x][y] += 5; }           //  1个空点  +   5分 死三
								else if (empty == 2) { Score[x][y] += 10; }     //  2个空点  +  10分 活三/连活三/跳活三(眠四)
							}
							else if (Player_num == 3) {                         //  4个棋子
								if (empty == 1) { Score[x][y] += 20; }          //  1个空点  +  20分 冲四/死四
								else if (empty == 2) { Score[x][y] += 100; }    //  2个空点  + 100分 活四
							}
							else if (Player_num >= 4) { Score[x][y] += 1000; }  //  5个棋子  +1000分 成五
							                                                    //  无法判断双活三、双冲四、活三+冲死等阵型
							empty = 0; //空点清零

							/* AI自我落点评分 */
							for (int k = 1; k <= 4; k++) {
								if (x + k * i >= 0 && x + k * i < 14 && y + k * j >= 0 && y + k * j < 14
									&& ChessData[x + k * i][y + k * j] == AI_role)
									AI_num++;
								else if (ChessData[x + k * i][y + k * j] == Gameform::Empty) { empty++; break; }  //空点+1退出
								else { break; }
							}
							for (int k = -1; k >= -4; k--) {
								if (x + k * i >= 0 && x + k * i < 14 && y + k * j >= 0 && y + k * j < 14
									&& ChessData[x + k * i][y + k * j] == AI_role)
									AI_num++;
								else if (ChessData[x + k * i][y + k * j] == Gameform::Empty) { empty++; break; }  //空点+1退出
								else { break; }
							}

							/* AI自我棋子评分 */                                  // 与空白棋子可形成阵型
							if (AI_num == 0) { Score[x][y] += 1; }               // 1个棋子  +    1分 单子
							else if (AI_num == 1) { Score[x][y] += 2; }          // 2个棋子  +    2分 活二
							else if (AI_num == 2) {                              // 3个棋子
								if (empty == 1) { Score[x][y] += 8; }            // 1个空点  +    8分 死三
								else if (empty == 2) { Score[x][y] += 30; }      // 2个空点  +   30分 活三/连活三/跳活三(眠四)
							}
							else if (AI_num == 3) {                              // 4个棋子
								if (empty == 1) { Score[x][y] += 50; }           // 1个空点  +   50分 冲四/死四
								else if (empty == 2) { Score[x][y] += 2000; }    // 2个空点  + 2000分 活四
							}
							else if (AI_num >= 4) { Score[x][y] += 10000; }      // 5个棋子  +10000分 成五

							Player_num = 0;AI_num = 0;empty = 0; //全部清零用于下次遍历
						}
					}
				}
			}
		}
	}
}
void Game::FindbestScore(int& x, int& y, Gameform::ChessType Role)
{
	srand((unsigned)time(0)); //调节系统时间，将伪随机数彻底随机
	ChessScore(Role); // 引用评分函数

	//最高分、相同最高分坐标的数组
	int Max = 0;
	int best_x[225], best_y[225], best_i = 1, bestx, besty;

	//获取所有点中最高分坐标
	for (int i = 0; i < 14; i++) {
		for (int j = 0; j < 14; j++) {
			if (Score[i][j] > Max) {
				Max = Score[i][j];  
				bestx = i; 
				besty = j;
			}
		}
	}
	best_x[0] = bestx;best_y[0] = besty;

	//一个局面具有多个最高分坐标
	for (int i = 0; i < 14; i++){
		for (int j = 0; j < 14; j++){
			if (Score[i][j] == Max && i!=bestx && j!=besty){
				best_x[best_i] = i;
				best_y[best_i] = j;
				best_i++;
			}
		}
	}

	//当存在多个最高分，在数组中随机取一坐标
	if ( best_i == 1 ) {x = best_x[0]; y = best_y[0];}
	else { 
		int r = rand() % best_i;
		x = best_x[r]; y = best_y[r];
	}

	qDebug() << "AI：" << x << "，" << y;
	ChessData[x][y] = Role;  //AI落子
	Max = 0; //重设最高分数值
}


/*                 难度二 实现函数                   */
/*              判断[x,y]是否可生成棋子              */
/*              对落点的小范围评分函数                */
/*     极小极大值搜索+α-β剪枝 递归查找最高分坐标       */
bool Game::chessCan(int x, int y, int n)
{ 
	if (n == 1)  // n=1,判断内侧,即[x,y] 3*3 范围的 8个点(第一圈范围)(包括[x,y]自身)
	{
		for (int i = x - 1; i < x + 2; i++)
			for (int j = y - 1; j < y + 2; j++)
				if ((i > 0 && i < 14 && j > 0 && j < 14) && ChessData[i][j] != Gameform::Empty)  //当[i,j]上有棋子则[x,y]可生成棋子
					return true;
	}
	else         // n=2,判断外侧,即[x,y] 3*3范围外 5*5范围内八个方向对应的 8个点(第二圈范围)(包括[x,y]自身)
	{
		for (int i = x - 2; i < x + 3; i += 2)
			for (int j = y - 2; j < y + 3; j += 2)
				if ((i > 0 && i < 14 && j > 0 && j < 14) && ChessData[i][j] != Gameform::Empty)  //当[i,j]上有棋子则[x,y]可生成棋子
					return true;
	}
	return false;
}
int Game::chessScore(int n,Gameform::ChessType Role,int CHESSData[14][14])
{
	int AI_role, Player_role;
	if (Role == Gameform::black) { AI_role = Role; Player_role = Gameform::white; }
	else { AI_role = Role; Player_role = Gameform::black; }

	memset(AI_score, 0, sizeof(AI_score));
	memset(Player_score, 0, sizeof(Player_score)); // 初始化小范围评分数组
	int Player_num = 0,AI_num = 0, empty = 0, max = 0,MAX = 0; // 初始化棋子连成统计个数、空点统计个数、最高分

	for (int x = 0; x < 14; x++) {
		for (int y = 0; y < 14; y++) {
			if (CHESSData[x][y] == Gameform::Empty) {

				/*遍历每个坐标的八个方向 */
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						if (i != 0 || j != 0) {  // [i,j]=[0,0]就是原棋子位置
                            /* 对玩家棋子计数 */
							for (int k = 1; k <= 4; k++) {
								if (x + k * i >= 0 && x + k * i < 14 && y + k * j >= 0 && y + k * j < 14  //边界判断
									&& CHESSData[x + k * i][y + k * j] == Player_role)
									Player_num++;
								else if (CHESSData[x + k * i][y + k * j] == Gameform::Empty) { empty++; break; }  //空点+1退出
								else { break; }
							}
							for (int k = -1; k >= -4; k--) {
								if (x + k * i >= 0 && x + k * i < 14 && y + k * j >= 0 && y + k * j < 14
									&& CHESSData[x + k * i][y + k * j] == Player_role)
									Player_num++;
								else if (CHESSData[x + k * i][y + k * j] == Gameform::Empty) { empty++; break; }  //空点+1退出
								else { break; }
							}

							/* 对玩家棋子评分 */                                 //  与空白棋子可形成的阵型
							if (Player_num == 1) { Player_score[x][y] += 1; }          //  2个棋子  +   1分 单子
							else if (Player_num == 2) {                         //  3个棋子
								if (empty == 1) { Player_score[x][y] += 5; }           //  1个空点  +   5分 死三
								else if (empty == 2) { Player_score[x][y] += 10; }     //  2个空点  +  10分 活三/连活三/跳活三(眠四)
							}
							else if (Player_num == 3) {                         //  4个棋子
								if (empty == 1) { Player_score[x][y] += 20; }          //  1个空点  +  20分 冲四/死四
								else if (empty == 2) { Player_score[x][y] += 100; }    //  2个空点  + 100分 活四
							}
							else if (Player_num >= 4) { Player_score[x][y] += 1000; }  //  5个棋子  +1000分 成五
																				//  无法判断双活三、双冲四、活三+冲死等阵型
							empty = 0; //空点清零

							/* AI自我落点评分 */
							for (int k = 1; k <= 4; k++) {
								if (x + k * i >= 0 && x + k * i < 14 && y + k * j >= 0 && y + k * j < 14
									&& CHESSData[x + k * i][y + k * j] == AI_role)
									AI_num++;
								else if (CHESSData[x + k * i][y + k * j] == Gameform::Empty) { empty++; break; }  //空点+1退出
								else { break; }
							}
							for (int k = -1; k >= -4; k--) {
								if (x + k * i >= 0 && x + k * i < 14 && y + k * j >= 0 && y + k * j < 14
									&& CHESSData[x + k * i][y + k * j] == AI_role)
									AI_num++;
								else if (CHESSData[x + k * i][y + k * j] == Gameform::Empty) { empty++; break; }  //空点+1退出
								else { break; }
							}

							/* AI自我棋子评分 */                                  // 与空白棋子可形成阵型
							if (AI_num == 0) { AI_score[x][y] += 1; }               // 1个棋子  +    1分 单子
							else if (AI_num == 1) { AI_score[x][y] += 2; }          // 2个棋子  +    2分 活二
							else if (AI_num == 2) {                              // 3个棋子
								if (empty == 1) { AI_score[x][y] += 8; }            // 1个空点  +    8分 死三
								else if (empty == 2) { AI_score[x][y] += 30; }      // 2个空点  +   30分 活三/连活三/跳活三(眠四)
							}
							else if (AI_num == 3) {                              // 4个棋子
								if (empty == 1) { AI_score[x][y] += 50; }           // 1个空点  +   50分 冲四/死四
								else if (empty == 2) { AI_score[x][y] += 2000; }    // 2个空点  + 2000分 活四
							}
							else if (AI_num >= 4) { AI_score[x][y] += 10000; }      // 5个棋子  +10000分 成五

							Player_num = 0; AI_num = 0; empty = 0; //全部清零用于下次遍历
						}
					}
				}
			}	
		}
	}

	for (int i = 0; i < 14; i++) {
		for (int j = 0; j < 14; j++) {
			if (AI_score[i][j] > MAX)
				MAX = AI_score[i][j];
			if (Player_score[i][j] > max)
				max = Player_score[i][j];
		}
	}

	return MAX - max;
}
int Game::GetMinMaxScore(int Step,int Alpha,int Beta,Gameform::ChessType Role,int CHESSDATA[14][14])
{
	int minmax, Data[14][14],n = 1;
	int other_Role;
	if (Role == Gameform::black) { other_Role = Gameform::white; }
	else { other_Role = Gameform::black; }
	
	Tree tree;
	tree.Alpha = Alpha; tree.Beta = Beta; tree.X = 0; tree.Y = 0;

	/*
	α-β剪枝的实现
	博弈树中每一个节点都有一个α和β，初始设置α为负无穷大，β为正无穷大。 α = Alpha , β = Beta 。

	对于MAX层，为了使自己利益最大化，会从下一层选择最大的分数，当传递上来的分数大于α时，则修改α值为此分数。
	即α剪枝，触发条件是MIN层方案的子结点出现大于α的得分，则剪去该条分枝。

	对于MIN层，为了使对手利益最小化，会从下一层选择最小的分数，当传递上来的分数小于β时，则修改β值为此分数。
	即β剪枝，触发条件是MAX层方案的子结点出现小于β的得分，则剪去该条分枝。

	当每一个节点要生成下一步棋子时，先判断α与β，如果α>β，则不用在生成下一步的走法，而是返回上一层。
	*/

	if (Step > 0) {
		for (int i = 0; i < 14; i++) {
			for (int j = 0; j < 14; j++) {

				/* α-β剪枝 */
				if (CHESSDATA[i][j] == Gameform::Empty && tree.Alpha < tree.Beta && chessCan(i, j, 1)) {  //
					memcpy(Data, CHESSDATA, sizeof(int) * 14 * 14); //拷贝棋面
					Data[i][j] = Role;
					minmax = GetMinMaxScore(Step - 1, tree.Alpha, tree.Beta, (Gameform::ChessType)other_Role, Data); //递归调用
					//minmax = -minmax;  //对方的负分是我方的正分
					if (Step % 2 == 0) {             // Max层
						if (n == 1) {
							tree.data = minmax; n++;
						}
						
						if (tree.Alpha < minmax) {  // 在Max层的下一层取最大的分数
							tree.Alpha = minmax;
							tree.data = minmax;
							tree.X = i;
							tree.Y = j;
						}
					}
					else {                          // Min层
						if (n == 1) {
							tree.data = minmax; n++;
						}
						if (tree.Beta > minmax) {   // 在Min层的下一层取最小的分数
							tree.Beta = minmax;
							tree.data = minmax;
							tree.X = i;
							tree.Y = j;
						}
					}
				}
				if (CHESSDATA[i][j] == Gameform::Empty && tree.Alpha < tree.Beta && chessCan(i, j, 2)) { // 
					memcpy(Data, CHESSDATA, sizeof(int) * 14 * 14); // 拷贝棋面
					Data[i][j] = Role;
					minmax = GetMinMaxScore(Step - 1, tree.Alpha, tree.Beta, (Gameform::ChessType)other_Role, Data);
					//minmax = -minmax;
					if (Step % 2 == 0) {                  // Max层
						if (tree.Alpha < minmax) {        // 在Max层的下一层取最大的分数
							tree.Alpha = minmax;
							tree.data = minmax;
							tree.X = i;
							tree.Y = j;
						}
					}
					else {                                // Min层
						if (tree.Beta > minmax) {         // 在Min层的下一层取最小的分数
							tree.Beta = minmax;
							tree.data = minmax;
							tree.X = i;
							tree.Y = j;
						}
					}
				}
			}
		}
		AI_X = tree.X;
		AI_Y = tree.Y;
		qDebug() << "Max:" << tree.data;
		return tree.data;
	}
	else { return chessScore(1, (Gameform::ChessType)Role, CHESSDATA); }  //局面评分
}


void Game::Sleep(int msec)
{
	QTime dieTime = QTime::currentTime().addMSecs(msec);
	while (QTime::currentTime() < dieTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
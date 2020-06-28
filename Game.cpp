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
	//����������ռ�
	myform = new Gameform();
	connect(myform, SIGNAL(SignalSendChessdata(int, int)), this, SLOT(doProcess(int, int)));

	//��ʾ����
	ui.grid_chess->addWidget(myform);

	RoleInit("../Gobang/image/black.png", "../Gobang/image/white.png");
	currentRole = Gameform::Empty;

}

//��ӱ���
void Game::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.drawPixmap(this->rect(), QPixmap("../Gobang/image/bg.png"));
}

//���̳�ʼ������������
void Game::setChessInit()
{
	for (int i = 0; i < 14; i++) {
		for (int j = 0; j < 14; j++) {
			ChessData[i][j] = Gameform::Empty;
		}
	}
	if (currentPK == defensive)  //AI���֣�Ĭ����[7��7]�������
		ChessData[6][6] = Gameform::black;
	myform->setChessStatus(ChessData);
}
void Game::doProcess(int i,int j)
{
	memcpy(HistoryData, ChessData, sizeof(int) * 14 * 14);

	qDebug() << "��ң�" << i << "," << j;
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

//�ڰ���������ʾ���л�������ʾ���ֻ�����
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

//AI���Ӻ���
void Game::playwithAI(Gameform::ChessType Role)
{
	/*                    �Ѷ�һ(ʵ��)                      */
	/* playwithAI(int& x, int& y,Gameform::ChessType Role) */

	//FindbestScore(x,y,Role);
	//myform->setChessStatus(ChessData);

	/*                    �Ѷȶ�(δʵ��)                    */
	/*         playwithAI(Gameform::ChessType Role)        */
	GetMinMaxScore(2, NGIF, PTIF, Role, ChessData);
	qDebug() << "AI:" << AI_X << "," << AI_Y;
	ChessData[AI_X][AI_Y] = Role;
	myform->setChessStatus(ChessData);
}

//���¿�ʼ���˳���Ϸ���ж��������顢�ж�ƽ��
void Game::GobangWIN(const char* Player)
{
	if (checkwin(currentRole, ChessData) == 1) {
		qDebug() << Player << "Win!\nGame Over!";
		int Restart = QMessageBox::question(NULL, "���У���", "�����¿�ʼ��ɧ��", QMessageBox::Yes | QMessageBox::No);
		if (Restart == QMessageBox::Yes) {
			this->hide();
			ComeGobang::to_Gobang->show(); //����������
		}
		else
			this->close();
	}
	else if (checkpeace() == true) {
		qDebug() << "World Peace!";
		int Restart = QMessageBox::question(NULL, "���У���", "�����¿�ʼ��ɧ��", QMessageBox::Yes | QMessageBox::No);
		if (Restart == QMessageBox::Yes) {
			this->hide();
			ComeGobang::to_Gobang->show(); //����������
		}
		else
			this->close();
	}
}
int Game::checkwin(int color, void* myform)
{
	typedef int(*p)[14];
	p chessData = p(myform);
	for (int i = 0; i < 14; i++) {              // �������������������
		for (int j = 0; j < 14; j++) {

			//���ұ�4������5��
			if (i < 10
				&& chessData[i][j] == color
				&& chessData[i + 1][j] == color
				&& chessData[i + 2][j] == color
				&& chessData[i + 3][j] == color
				&& chessData[i + 4][j] == color)
			{
				return 1;
			}
			//���±�4������5��
			if (j < 10
				&& chessData[i][j] == color
				&& chessData[i][j + 1] == color
				&& chessData[i][j + 2] == color
				&& chessData[i][j + 3] == color
				&& chessData[i][j + 4] == color)
			{
				return 1;
			}
			//������4������5��
			if (i > 3 && j < 10
				&& chessData[i][j] == color
				&& chessData[i - 1][j + 1] == color
				&& chessData[i - 2][j + 2] == color
				&& chessData[i - 3][j + 3] == color
				&& chessData[i - 4][j + 4] == color)
			{
				return 1;
			}
			//������4������5��
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

//��Ϸ��ʼ(ѡ���Ⱥ���)������(δʵ��)����͡��ϸ�
void Game::on_btn_start_clicked()
{
	int ret = QMessageBox::question(NULL, "���У���", "�Ƿ�ѡ������", QMessageBox::Yes | QMessageBox::No);
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
	int ret = QMessageBox::question(NULL, "��", "�����Ҫ������", QMessageBox::Yes | QMessageBox::No);
	if (ret == QMessageBox::Yes) {
		memcpy(ChessData, HistoryData, sizeof(int) * 14 * 14);
		myform->setChessStatus(ChessData);
	}
}
void Game::on_btn_peace_clicked()
{
	qDebug() << "���⣿��";
	GobangWIN("Nobody");
}
void Game::on_btn_giveup_clicked()
{
	qDebug() << "����˼�";
	GobangWIN("AI");
}


/*      �Ѷ�һ ʵ�ֺ���     */
/*         ���ֺ���        */
/*      ������߷�����     */
void Game::ChessScore(Gameform::ChessType Role)
{
	// ����˫����ɫ(��Ϊ�漰���֣�����ֱ�ӹ̶���ɫ)
	int AI_role, Player_role;
	if (Role == Gameform::black) {AI_role = Role; Player_role = Gameform::white; }
	else {AI_role = Role;Player_role = Gameform::black; }

	memset(Score, 0, sizeof(Score));  //��ʼ����������
	int Player_num = 0, AI_num = 0;   //��һ�AI��������ͳ�Ƹ���
	int empty = 0;  //�յ�ͳ�Ƹ���

    /* 
	����
    ��ÿһ�����ӽ���ȫ�̵ı�������ĳ��Ϊ�հף���������ܰ˸������ڱ�Եλ���жϱ߽磩
    ���ڿհ״����ҷ�����ĳһ�Խ��߷�������һ��������ɫ�����ӣ��հ�λ�õķ������鸳��
    ����������������Ƿ���������ͬһ��ɫ�����ӣ���û����������������߼����һ���հ�λ�á�
	�������ƣ�������⡣ÿһ���������ʹ�÷�������õ��ۼӣ��ᵼ��AI�жϵı仯��

	����
    �ڷ��ط�������Ļ����ϣ��Ա�������Ҳ�������Ƶı������ж��Լ������ƣ��Է������鸳�֡�
	*/

	for (int x = 0; x < 14; x++) {
		for (int y = 0; y < 14; y++) {
			if (ChessData[x][y] == Gameform::Empty) {

				/*�жϸÿհ����ӵİ˸����� */
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						if (i != 0 || j != 0) {  // [i,j]=[0,0]���ǿհ�����λ��

							/* ��������Ӽ��� */
							for (int k = 1; k <= 4; k++) {
								if (x + k * i >= 0 && x + k * i < 14 && y + k * j >= 0 && y + k * j < 14  //�߽��ж�
									&& ChessData[x + k * i][y + k * j] == Player_role)
									Player_num++;
								else if (ChessData[x + k * i][y + k * j] == Gameform::Empty) { empty++; break; }  //�յ�+1�˳�
								else { break; }
							}
							for (int k = -1; k >= -4; k--) {
								if (x + k * i >= 0 && x + k * i < 14 && y + k * j >= 0 && y + k * j < 14
									&& ChessData[x + k * i][y + k * j] == Player_role)
									Player_num++;
								else if (ChessData[x + k * i][y + k * j] == Gameform::Empty) { empty++; break; }  //�յ�+1�˳�
								else { break; }
							}

							/* ������������� */                                 //  ��հ����ӿ��γɵ�����
							if (Player_num == 1) { Score[x][y] += 1; }          //  2������  +   1�� ����
							else if (Player_num == 2) {                         //  3������
								if (empty == 1) { Score[x][y] += 5; }           //  1���յ�  +   5�� ����
								else if (empty == 2) { Score[x][y] += 10; }     //  2���յ�  +  10�� ����/������/������(����)
							}
							else if (Player_num == 3) {                         //  4������
								if (empty == 1) { Score[x][y] += 20; }          //  1���յ�  +  20�� ����/����
								else if (empty == 2) { Score[x][y] += 100; }    //  2���յ�  + 100�� ����
							}
							else if (Player_num >= 4) { Score[x][y] += 1000; }  //  5������  +1000�� ����
							                                                    //  �޷��ж�˫������˫���ġ�����+����������
							empty = 0; //�յ�����

							/* AI����������� */
							for (int k = 1; k <= 4; k++) {
								if (x + k * i >= 0 && x + k * i < 14 && y + k * j >= 0 && y + k * j < 14
									&& ChessData[x + k * i][y + k * j] == AI_role)
									AI_num++;
								else if (ChessData[x + k * i][y + k * j] == Gameform::Empty) { empty++; break; }  //�յ�+1�˳�
								else { break; }
							}
							for (int k = -1; k >= -4; k--) {
								if (x + k * i >= 0 && x + k * i < 14 && y + k * j >= 0 && y + k * j < 14
									&& ChessData[x + k * i][y + k * j] == AI_role)
									AI_num++;
								else if (ChessData[x + k * i][y + k * j] == Gameform::Empty) { empty++; break; }  //�յ�+1�˳�
								else { break; }
							}

							/* AI������������ */                                  // ��հ����ӿ��γ�����
							if (AI_num == 0) { Score[x][y] += 1; }               // 1������  +    1�� ����
							else if (AI_num == 1) { Score[x][y] += 2; }          // 2������  +    2�� ���
							else if (AI_num == 2) {                              // 3������
								if (empty == 1) { Score[x][y] += 8; }            // 1���յ�  +    8�� ����
								else if (empty == 2) { Score[x][y] += 30; }      // 2���յ�  +   30�� ����/������/������(����)
							}
							else if (AI_num == 3) {                              // 4������
								if (empty == 1) { Score[x][y] += 50; }           // 1���յ�  +   50�� ����/����
								else if (empty == 2) { Score[x][y] += 2000; }    // 2���յ�  + 2000�� ����
							}
							else if (AI_num >= 4) { Score[x][y] += 10000; }      // 5������  +10000�� ����

							Player_num = 0;AI_num = 0;empty = 0; //ȫ�����������´α���
						}
					}
				}
			}
		}
	}
}
void Game::FindbestScore(int& x, int& y, Gameform::ChessType Role)
{
	srand((unsigned)time(0)); //����ϵͳʱ�䣬��α������������
	ChessScore(Role); // �������ֺ���

	//��߷֡���ͬ��߷����������
	int Max = 0;
	int best_x[225], best_y[225], best_i = 1, bestx, besty;

	//��ȡ���е�����߷�����
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

	//һ��������ж����߷�����
	for (int i = 0; i < 14; i++){
		for (int j = 0; j < 14; j++){
			if (Score[i][j] == Max && i!=bestx && j!=besty){
				best_x[best_i] = i;
				best_y[best_i] = j;
				best_i++;
			}
		}
	}

	//�����ڶ����߷֣������������ȡһ����
	if ( best_i == 1 ) {x = best_x[0]; y = best_y[0];}
	else { 
		int r = rand() % best_i;
		x = best_x[r]; y = best_y[r];
	}

	qDebug() << "AI��" << x << "��" << y;
	ChessData[x][y] = Role;  //AI����
	Max = 0; //������߷���ֵ
}


/*                 �Ѷȶ� ʵ�ֺ���                   */
/*              �ж�[x,y]�Ƿ����������              */
/*              ������С��Χ���ֺ���                */
/*     ��С����ֵ����+��-�¼�֦ �ݹ������߷�����       */
bool Game::chessCan(int x, int y, int n)
{ 
	if (n == 1)  // n=1,�ж��ڲ�,��[x,y] 3*3 ��Χ�� 8����(��һȦ��Χ)(����[x,y]����)
	{
		for (int i = x - 1; i < x + 2; i++)
			for (int j = y - 1; j < y + 2; j++)
				if ((i > 0 && i < 14 && j > 0 && j < 14) && ChessData[i][j] != Gameform::Empty)  //��[i,j]����������[x,y]����������
					return true;
	}
	else         // n=2,�ж����,��[x,y] 3*3��Χ�� 5*5��Χ�ڰ˸������Ӧ�� 8����(�ڶ�Ȧ��Χ)(����[x,y]����)
	{
		for (int i = x - 2; i < x + 3; i += 2)
			for (int j = y - 2; j < y + 3; j += 2)
				if ((i > 0 && i < 14 && j > 0 && j < 14) && ChessData[i][j] != Gameform::Empty)  //��[i,j]����������[x,y]����������
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
	memset(Player_score, 0, sizeof(Player_score)); // ��ʼ��С��Χ��������
	int Player_num = 0,AI_num = 0, empty = 0, max = 0,MAX = 0; // ��ʼ����������ͳ�Ƹ������յ�ͳ�Ƹ�������߷�

	for (int x = 0; x < 14; x++) {
		for (int y = 0; y < 14; y++) {
			if (CHESSData[x][y] == Gameform::Empty) {

				/*����ÿ������İ˸����� */
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						if (i != 0 || j != 0) {  // [i,j]=[0,0]����ԭ����λ��
                            /* ��������Ӽ��� */
							for (int k = 1; k <= 4; k++) {
								if (x + k * i >= 0 && x + k * i < 14 && y + k * j >= 0 && y + k * j < 14  //�߽��ж�
									&& CHESSData[x + k * i][y + k * j] == Player_role)
									Player_num++;
								else if (CHESSData[x + k * i][y + k * j] == Gameform::Empty) { empty++; break; }  //�յ�+1�˳�
								else { break; }
							}
							for (int k = -1; k >= -4; k--) {
								if (x + k * i >= 0 && x + k * i < 14 && y + k * j >= 0 && y + k * j < 14
									&& CHESSData[x + k * i][y + k * j] == Player_role)
									Player_num++;
								else if (CHESSData[x + k * i][y + k * j] == Gameform::Empty) { empty++; break; }  //�յ�+1�˳�
								else { break; }
							}

							/* ������������� */                                 //  ��հ����ӿ��γɵ�����
							if (Player_num == 1) { Player_score[x][y] += 1; }          //  2������  +   1�� ����
							else if (Player_num == 2) {                         //  3������
								if (empty == 1) { Player_score[x][y] += 5; }           //  1���յ�  +   5�� ����
								else if (empty == 2) { Player_score[x][y] += 10; }     //  2���յ�  +  10�� ����/������/������(����)
							}
							else if (Player_num == 3) {                         //  4������
								if (empty == 1) { Player_score[x][y] += 20; }          //  1���յ�  +  20�� ����/����
								else if (empty == 2) { Player_score[x][y] += 100; }    //  2���յ�  + 100�� ����
							}
							else if (Player_num >= 4) { Player_score[x][y] += 1000; }  //  5������  +1000�� ����
																				//  �޷��ж�˫������˫���ġ�����+����������
							empty = 0; //�յ�����

							/* AI����������� */
							for (int k = 1; k <= 4; k++) {
								if (x + k * i >= 0 && x + k * i < 14 && y + k * j >= 0 && y + k * j < 14
									&& CHESSData[x + k * i][y + k * j] == AI_role)
									AI_num++;
								else if (CHESSData[x + k * i][y + k * j] == Gameform::Empty) { empty++; break; }  //�յ�+1�˳�
								else { break; }
							}
							for (int k = -1; k >= -4; k--) {
								if (x + k * i >= 0 && x + k * i < 14 && y + k * j >= 0 && y + k * j < 14
									&& CHESSData[x + k * i][y + k * j] == AI_role)
									AI_num++;
								else if (CHESSData[x + k * i][y + k * j] == Gameform::Empty) { empty++; break; }  //�յ�+1�˳�
								else { break; }
							}

							/* AI������������ */                                  // ��հ����ӿ��γ�����
							if (AI_num == 0) { AI_score[x][y] += 1; }               // 1������  +    1�� ����
							else if (AI_num == 1) { AI_score[x][y] += 2; }          // 2������  +    2�� ���
							else if (AI_num == 2) {                              // 3������
								if (empty == 1) { AI_score[x][y] += 8; }            // 1���յ�  +    8�� ����
								else if (empty == 2) { AI_score[x][y] += 30; }      // 2���յ�  +   30�� ����/������/������(����)
							}
							else if (AI_num == 3) {                              // 4������
								if (empty == 1) { AI_score[x][y] += 50; }           // 1���յ�  +   50�� ����/����
								else if (empty == 2) { AI_score[x][y] += 2000; }    // 2���յ�  + 2000�� ����
							}
							else if (AI_num >= 4) { AI_score[x][y] += 10000; }      // 5������  +10000�� ����

							Player_num = 0; AI_num = 0; empty = 0; //ȫ�����������´α���
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
	��-�¼�֦��ʵ��
	��������ÿһ���ڵ㶼��һ�����ͦ£���ʼ���æ�Ϊ������󣬦�Ϊ������� �� = Alpha , �� = Beta ��

	����MAX�㣬Ϊ��ʹ�Լ�������󻯣������һ��ѡ�����ķ����������������ķ������ڦ�ʱ�����޸Ħ�ֵΪ�˷�����
	������֦������������MIN�㷽�����ӽ����ִ��ڦ��ĵ÷֣����ȥ������֦��

	����MIN�㣬Ϊ��ʹ����������С���������һ��ѡ����С�ķ����������������ķ���С�ڦ�ʱ�����޸Ħ�ֵΪ�˷�����
	���¼�֦������������MAX�㷽�����ӽ�����С�ڦµĵ÷֣����ȥ������֦��

	��ÿһ���ڵ�Ҫ������һ������ʱ�����жϦ���£������>�£�������������һ�����߷������Ƿ�����һ�㡣
	*/

	if (Step > 0) {
		for (int i = 0; i < 14; i++) {
			for (int j = 0; j < 14; j++) {

				/* ��-�¼�֦ */
				if (CHESSDATA[i][j] == Gameform::Empty && tree.Alpha < tree.Beta && chessCan(i, j, 1)) {  //
					memcpy(Data, CHESSDATA, sizeof(int) * 14 * 14); //��������
					Data[i][j] = Role;
					minmax = GetMinMaxScore(Step - 1, tree.Alpha, tree.Beta, (Gameform::ChessType)other_Role, Data); //�ݹ����
					//minmax = -minmax;  //�Է��ĸ������ҷ�������
					if (Step % 2 == 0) {             // Max��
						if (n == 1) {
							tree.data = minmax; n++;
						}
						
						if (tree.Alpha < minmax) {  // ��Max�����һ��ȡ���ķ���
							tree.Alpha = minmax;
							tree.data = minmax;
							tree.X = i;
							tree.Y = j;
						}
					}
					else {                          // Min��
						if (n == 1) {
							tree.data = minmax; n++;
						}
						if (tree.Beta > minmax) {   // ��Min�����һ��ȡ��С�ķ���
							tree.Beta = minmax;
							tree.data = minmax;
							tree.X = i;
							tree.Y = j;
						}
					}
				}
				if (CHESSDATA[i][j] == Gameform::Empty && tree.Alpha < tree.Beta && chessCan(i, j, 2)) { // 
					memcpy(Data, CHESSDATA, sizeof(int) * 14 * 14); // ��������
					Data[i][j] = Role;
					minmax = GetMinMaxScore(Step - 1, tree.Alpha, tree.Beta, (Gameform::ChessType)other_Role, Data);
					//minmax = -minmax;
					if (Step % 2 == 0) {                  // Max��
						if (tree.Alpha < minmax) {        // ��Max�����һ��ȡ���ķ���
							tree.Alpha = minmax;
							tree.data = minmax;
							tree.X = i;
							tree.Y = j;
						}
					}
					else {                                // Min��
						if (tree.Beta > minmax) {         // ��Min�����һ��ȡ��С�ķ���
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
	else { return chessScore(1, (Gameform::ChessType)Role, CHESSDATA); }  //��������
}


void Game::Sleep(int msec)
{
	QTime dieTime = QTime::currentTime().addMSecs(msec);
	while (QTime::currentTime() < dieTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
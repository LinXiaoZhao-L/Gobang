#include "ComeGame.h"

Game* ComeGame::to_Game = NULL;
ComeGame::ComeGame()
{

}

void ComeGame::InitForm()
{
	to_Game = new Game();
}
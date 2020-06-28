#include "ComeGobang.h"
Gobang* ComeGobang::to_Gobang = NULL;
ComeGobang::ComeGobang()
{

}

void ComeGobang::InitForm()
{
	to_Gobang = new Gobang();
}
#include "ComeOnline.h"

Online* ComeOnline::to_Online = NULL;
ComeOnline::ComeOnline()
{

}

void ComeOnline::InitForm()
{
	to_Online = new Online();
}
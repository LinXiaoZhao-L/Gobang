#include "Gobang.h"
#include <QtWidgets/QApplication>
#include "ComeOnline.h"
#include "ComeGame.h"
#include "ComeGobang.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ComeOnline::InitForm();
    ComeGame::InitForm();
    ComeGobang::InitForm();
    Gobang w;
    w.show();

    return a.exec();
}

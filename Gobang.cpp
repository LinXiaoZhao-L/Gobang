#include "Gobang.h"
#include "ComeOnline.h"
#include "ComeGame.h"
#include "Game.h"

Gobang::Gobang(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    QObject::connect(ui.btn_pvp, SIGNAL(clicked()), this, SLOT(on_btn_pvp_clicked()));
    QObject::connect(ui.btn_pve, SIGNAL(clicked()), this, SLOT(on_btn_pve_clicked()));
}

void Gobang::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    //画背景
    QRect rec(QPoint(0,0),QPoint(this->width(), this->height()));
    QPixmap pix("../Gobang/image/bg_start.png");
    painter.drawPixmap(rec,pix);
    
}

void Gobang::on_btn_pvp_clicked()
{
    this->hide();
    ComeOnline::to_Online->show();  // 前往联机平台(暂定)(未完成)
}
void Gobang::on_btn_pve_clicked()
{
    this->hide();
    ComeGame::to_Game->show();      // 前往人机对战窗口
}
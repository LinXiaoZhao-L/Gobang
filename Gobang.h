#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Gobang.h"
#include <QPainter>
#include <QImage>

class Gobang : public QMainWindow
{
    Q_OBJECT

public:
    Gobang(QWidget *parent = Q_NULLPTR);


private:
    void paintEvent(QPaintEvent *);
    

private slots:
    void on_btn_pvp_clicked();
    void on_btn_pve_clicked();

private:
    Ui::GobangClass ui;
};

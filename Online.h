#pragma once
#pragma execution_character_set("utf-8")

#include <QWidget>
#include "ui_Online.h"

class Online : public QWidget
{
	Q_OBJECT

public:
	Online(QWidget *parent = Q_NULLPTR);
	~Online();

private:
	Ui::Online ui;
};

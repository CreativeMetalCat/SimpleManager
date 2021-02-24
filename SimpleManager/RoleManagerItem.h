#pragma once

#include <QWidget>
#include "ui_RoleManagerItem.h"

class RoleManagerItem : public QWidget
{
	Q_OBJECT

public:
	RoleManagerItem(QWidget *parent = Q_NULLPTR);
	~RoleManagerItem();

private:
	Ui::RoleManagerItem ui;
};

#pragma once

#include <QWidget>
#include "ui_UserManager.h"

class UserManager : public QWidget
{
	Q_OBJECT

public:
	UserManager(QWidget *parent = Q_NULLPTR);
	~UserManager();

private:
	Ui::UserManager ui;
};

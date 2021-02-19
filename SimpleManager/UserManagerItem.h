#pragma once

#include <QWidget>
#include "ui_UserManagerItem.h"

class UserManagerItem : public QWidget
{
	Q_OBJECT

public:
	UserManagerItem(QWidget *parent = Q_NULLPTR);
	~UserManagerItem();

private:
	Ui::UserManagerItem ui;
};

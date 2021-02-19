#pragma once

#include <QWidget>
#include "ui_UserManagerItem.h"
#include "Info.h"

using namespace ManagerInfo;
class UserManagerItem : public QWidget
{
	Q_OBJECT

public:
	UserManagerItem(SUserInfo userInfo, QWidget *parent = Q_NULLPTR);
	~UserManagerItem();

	SUserInfo UserInfo;
private:
	Ui::UserManagerItem ui;
};

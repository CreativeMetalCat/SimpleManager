#pragma once

#include <QWidget>
#include "ui_UserAdditionWindow.h"
#include <QDialog>
#include "Info.h"

class UserAdditionWindow : public QDialog
{
	Q_OBJECT

public:
	UserAdditionWindow(QWidget *parent = Q_NULLPTR);
	~UserAdditionWindow();

signals:
	void OnUserCreationFinished(ManagerInfo::SUserInfo);
private:

	void CreateUserInfo();

	Ui::UserAdditionWindow ui;
};

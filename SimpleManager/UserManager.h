#pragma once

#include <QWidget>
#include "ui_UserManager.h"
#include <QLayout>
#include "RoleSelectionWindow.h"
#include "UserManagerItem.h"

class UserManager : public QWidget
{
	Q_OBJECT

public:
	UserManager(QSqlDatabase dataBase,QWidget *parent = Q_NULLPTR);
	~UserManager();
protected:
	QSqlDatabase DataBase;
	/// <summary>
	/// Generates a list of users based on data from data table
	/// </summary>
	void GenerateUserList();
private:
	Ui::UserManager ui;

	QWidget* scrollWidget;

	QVBoxLayout* scrollBox;
};

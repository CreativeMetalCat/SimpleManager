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
	UserManager(ManagerInfo::SUserInfo currentUserInfo,QSqlDatabase dataBase,QWidget *parent = Q_NULLPTR);
	~UserManager();
protected:
	QSqlDatabase DataBase;
	/// <summary>
	/// Generates a list of users based on data from data table
	/// </summary>
	void GenerateUserList();
private slots:
	void ShowUserCreationWindow();

	void DeleteSelectedUsers();
private:

	void WriteNewUser(ManagerInfo::SUserInfo userInfo);


	ManagerInfo::SUserInfo CurrentUserInfo;

	Ui::UserManager ui;

	QWidget* scrollWidget;

	QVBoxLayout* scrollBox;
};

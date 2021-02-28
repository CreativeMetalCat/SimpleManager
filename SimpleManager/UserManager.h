#pragma once

#include <QWidget>
#include "ui_UserManager.h"
#include <QLayout>
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

	void AddUserToTable(int userId,QVector<int> roles);

private:
	/// <summary>
	/// This value is used during user addition and is used by lambda
	/// </summary>
	QVector<int> RoleIds = QVector<int>();

	void WriteNewUser(ManagerInfo::SUserInfo userInfo);


	ManagerInfo::SUserInfo CurrentUserInfo;

	Ui::UserManager ui;

	QWidget* scrollWidget;

	QVBoxLayout* scrollBox;
};

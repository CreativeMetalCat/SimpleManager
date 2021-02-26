#pragma once

#include <QDialog>
#include "ui_UserLogInDialog.h"
#include "Info.h"
#include <QSqlDatabase>

/// <summary>
/// This class is used for accessing the database itself and getting the user info. It should be the first window to be shown to the user
/// </summary>
class UserLogInDialog : public QDialog
{
	Q_OBJECT

public:
	UserLogInDialog(QSqlDatabase dataBase, QWidget *parent = Q_NULLPTR);
	~UserLogInDialog();

	ManagerInfo::SUserInfo Info;
signals:
	void OnLogInSuccessful();
private slots:
	void AttemptToLogIn();

	void OpenNewDBCreationWindow();
private:
	QSqlDatabase DataBase;

	Ui::UserLogInDialog ui;
};

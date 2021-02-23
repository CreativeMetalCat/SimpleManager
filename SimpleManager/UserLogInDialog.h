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

signals:
	void OnLogInSuccessful(ManagerInfo::SUserInfo);
private slots:
	void AttemptToLogIn();

	void OpenNewDBCreationWindow();
private:
	QSqlDatabase DataBase;

	Ui::UserLogInDialog ui;
};

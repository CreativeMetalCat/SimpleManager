#pragma once

#include <QWidget>
#include "ui_RoleManagerWindow.h"
#include "Info.h"
#include <QSqlDatabase>

class RoleManagerWindow : public QWidget
{
	Q_OBJECT

public:
	RoleManagerWindow(ManagerInfo::SUserInfo currentUserInfo, QSqlDatabase dataBase, QWidget* parent = Q_NULLPTR);
	~RoleManagerWindow();


private:
	QVector<QWidget*> Items = QVector<QWidget*>();
	void GenerateRoleList();

	ManagerInfo::SUserInfo CurrentUserInfo;

	QSqlDatabase DataBase;

	Ui::RoleManagerWindow ui;

	QWidget* scrollWidget;

	QVBoxLayout* scrollBox;
};

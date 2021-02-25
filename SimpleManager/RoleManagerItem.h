#pragma once

#include <QWidget>
#include "ui_RoleManagerItem.h"
#include "Info.h"

#include <QSqlDatabase>

class RoleManagerItem : public QWidget
{
	Q_OBJECT

public:
	RoleManagerItem(ManagerInfo::SUserInfo currentUserInfo, QSqlDatabase dataBase, QWidget *parent = Q_NULLPTR);
	~RoleManagerItem();

private:
	void GenerateList();

	Ui::RoleManagerItem ui;

	QWidget* scrollWidget;

	QVBoxLayout* scrollBox;

	ManagerInfo::SUserInfo CurrentUserInfo;

	QSqlDatabase DataBase;
};

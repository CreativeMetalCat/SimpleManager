#pragma once

#include <QWidget>
#include "ui_GroupManager.h"
#include <QSqlDatabase>
#include  "Info.h"
#include <QLayout>

class GroupManager : public QWidget
{
	Q_OBJECT

public:
	GroupManager(ManagerInfo::SUserInfo currentUserInfo, QSqlDatabase dataBase, QWidget *parent = Q_NULLPTR);
	~GroupManager();

private slots:
	void DeleteGroups();
private:
	void GenerateItems();

	QVector<QWidget*>Items = QVector<QWidget*>();

	QSqlDatabase DataBase;

	ManagerInfo::SUserInfo CurrentUserInfo;

	Ui::GroupManager ui;

	QWidget* scrollWidget;

	QVBoxLayout* scrollBox;
};

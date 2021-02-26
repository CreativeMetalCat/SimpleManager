#pragma once

#include <QWidget>
#include "ui_TableSettingsTab.h"
#include "Info.h"
#include <QSqlDatabase>

class TableSettingsTab : public QWidget
{
	Q_OBJECT

public:
	TableSettingsTab(ManagerInfo::SUserInfo currentUserInfo, QSqlDatabase dataBase, QWidget *parent = Q_NULLPTR);
	~TableSettingsTab();

	ManagerInfo::SUserInfo CurrentUserInfo;

	QSqlDatabase DataBase;
signals:
	void OnTableSetDeleted();
private slots:
	void Delete();
private:
	Ui::TableSettingsTab ui;
};

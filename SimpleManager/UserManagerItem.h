#pragma once

#include <QWidget>

#include <QCheckBox>
#include "ui_UserManagerItem.h"
#include "Info.h"

#include <QSqlDatabase>

using namespace ManagerInfo;
class UserManagerItem : public QWidget
{
	Q_OBJECT

public:
	UserManagerItem(SUserInfo userInfo, QSqlDatabase dataBase, QWidget *parent = Q_NULLPTR);
	~UserManagerItem();

	SUserInfo UserInfo;

	//if player wants a context menu we create it in this function
	virtual void contextMenuEvent(QContextMenuEvent* event)override;

	bool IsSelected();
private slots:
	//void RecordRoleChanges(QVector<int> roles);
private:

	QSqlDatabase DataBase;
	Ui::UserManagerItem ui;

	virtual bool eventFilter(QObject* object, QEvent* event)override;
};

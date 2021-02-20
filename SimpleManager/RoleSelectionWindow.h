#pragma once

#include <QWidget>
#include "ui_RoleSelectionWindow.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDialog>
#include <QSqlDatabase>
#include <QCheckBox>
#include "Info.h"

using namespace ManagerInfo;

class RoleSelectionWindow : public QDialog
{
	Q_OBJECT

public:
	RoleSelectionWindow(SUserInfo user, QSqlDatabase dataBase, QWidget *parent = Q_NULLPTR);
	~RoleSelectionWindow();
protected:
	void GenerateRoleList();

signals:
	void OnRolesChosen(QVector<int>);
private slots:
	void AcceptChanges();
private:

	QVector<QCheckBox*> Checkboxes = QVector<QCheckBox*>();

	QVector<int> Roles;
	SUserInfo UserInfo;

	Ui::RoleSelectionWindow ui;

	QWidget* scrollWidget;

	QVBoxLayout* scrollBox;
};

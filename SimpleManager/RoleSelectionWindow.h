#pragma once

#include <QWidget>
#include "ui_RoleSelectionWindow.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDialog>
#include <QSqlDatabase>

class RoleSelectionWindow : public QDialog
{
	Q_OBJECT

public:
	RoleSelectionWindow(QSqlDatabase dataBase, QWidget *parent = Q_NULLPTR);
	~RoleSelectionWindow();
protected:
	void GenerateRoleList();
private:
	Ui::RoleSelectionWindow ui;

	QWidget* scrollWidget;

	QVBoxLayout* scrollBox;
};

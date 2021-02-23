#pragma once

#include <QWidget>
#include "ui_TableCreationWindow.h"
#include <QLineEdit>
#include <QDialog>

#include <QSqlDatabase>
#include "SqlQueryStrings.h"

class TableCreationWindow : public QDialog
{
	Q_OBJECT

public:
	TableCreationWindow(QSqlDatabase database,QWidget *parent = Q_NULLPTR);
	~TableCreationWindow();
private slots:
	void AddNewRoleEdit();

	void AttemptToCreateTable();
private:
	QSqlDatabase Database;

	QWidget* scrollWidget;

	QVBoxLayout* scrollBox;

	QVector<QWidget*> Roles = QVector<QWidget*>();

	Ui::TableCreationWindow ui;
};

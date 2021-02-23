#pragma once

#include <QWidget>
#include "ui_TableCreationWindow.h"
#include <QLineEdit>
#include <QDialog>

class TableCreationWindow : public QDialog
{
	Q_OBJECT

public:
	TableCreationWindow(QWidget *parent = Q_NULLPTR);
	~TableCreationWindow();
private slots:
	void AddNewRoleEdit();
private:
	QWidget* scrollWidget;

	QVBoxLayout* scrollBox;

	QVector<QWidget*> Roles = QVector<QWidget*>();

	Ui::TableCreationWindow ui;
};

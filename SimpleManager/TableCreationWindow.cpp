#include "TableCreationWindow.h"
#include <QLayout>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>

TableCreationWindow::TableCreationWindow(QSqlDatabase database, QWidget *parent):
	QDialog(parent),Database(database)
{
	ui.setupUi(this);

	scrollWidget = new QWidget(this);
	scrollBox = new QVBoxLayout(scrollWidget);
	ui.scrollArea->setWidget(scrollWidget);

	//Bind role addition to button click
	connect(ui.button_AddRole, &QPushButton::clicked, this, &TableCreationWindow::AddNewRoleEdit);

	//Attempt to create table on button click
	connect(ui.button_Create, &QPushButton::clicked, this, &TableCreationWindow::AttemptToCreateTable);
}

void TableCreationWindow::AddNewRoleEdit() 
{
	QWidget* roleEdit = new QWidget(scrollWidget);

	QHBoxLayout* layout = new QHBoxLayout(roleEdit);
	QLineEdit* lineEdit = new QLineEdit(roleEdit);
	QPushButton* deleteButton = new QPushButton(roleEdit);

	deleteButton->setText("Delete This Role");

	layout->addWidget(lineEdit);
	layout->addWidget(deleteButton);

	scrollBox->addWidget(roleEdit);
	Roles.append(roleEdit);
	//if user wants to delete the role -> bind this even to button
	connect(deleteButton, &QPushButton::clicked, this, [this, roleEdit]
	{
		Roles.removeOne(roleEdit);
		delete roleEdit;
	});
}

void TableCreationWindow::AttemptToCreateTable()
{
	if (Database.isOpen())
	{
		QSqlQuery query;
		//first we check if table exists -> if it does -> give user an error message and ask them to change the name
		query.exec(SqlHelpers::GetTableExistanceCheckString("Role", ui.lineEdit_DBName->text()));
		bool has = false;
		while (query.next())
		{
			//if we found an entry with that name set to true and end the search
			has = true;
			break;
		}

		if (!has)
		{
			if (!query.exec(SqlHelpers::GetRoleTableCreationString("Role", ui.lineEdit_DBName->text())))
			{

			}
		}
		else
		{
			QMessageBox::critical(this, "Error!", "Failed to create data base with this name! This name is already used!");
		}
	}
}


TableCreationWindow::~TableCreationWindow()
{
}


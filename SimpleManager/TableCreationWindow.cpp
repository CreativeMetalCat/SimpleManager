#include "TableCreationWindow.h"
#include <QLayout>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QSqlRecord>

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
		if (ui.lineEdit_Name->text().remove(" ") != " " && ui.lineEdit_Password->text().remove(" ") != " " && ui.lineEdit_DBName->text().remove(" ") != " ")
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
				query.exec("INSERT INTO TableSets (name) VALUES (\"" + ui.lineEdit_DBName->text() + "\")");
				qWarning() << query.lastError().text();
				if (query.exec(SqlHelpers::GetRoleTableCreationString("Role", ui.lineEdit_DBName->text())))
				{

					int TableSetId = 0;
					;
					if (query.exec("SELECT Id FROM TableSets WHERE Name = \"" + ui.lineEdit_DBName->text() + "\""))
					{
						query.next();
						QSqlRecord record = query.record();

						//get the id of newly created DB
						TableSetId = query.value(record.indexOf("Id")).toInt();

						//create user
						ManagerInfo::SUserInfo user;
						user.Name = ui.lineEdit_Name->text();
						//generate encrypted password string
						user.Password = QString(QCryptographicHash::hash(ui.lineEdit_Password->text().toUtf8(), QCryptographicHash::Md5).toHex());

						user.TableSetId = TableSetId;



						QSqlQuery query;
						query.prepare("INSERT INTO Users (Name,RoleId,Password,ContactInfo,TableSetId) VALUES (:Name,:RoleId,:Password,:ContactInfo,:TableSetId);SELECT * FROM TableSets Where id = last_insert_rowid()");
						query.bindValue(":Name", ui.lineEdit_Name->text());
						query.bindValue(":Password", QString(QCryptographicHash::hash(ui.lineEdit_Password->text().toUtf8(), QCryptographicHash::Md5).toHex()));
						//it's generated like that because the newly created db will only have on, all powerful, role
						query.bindValue(":RoleId", "'{\"roles\":[0]}'");
						query.bindValue(":ContactInfo", "");
						//assign user to be a part of newly created db
						query.bindValue(":TableSetId", TableSetId);

						//this adds the user and returns the data at the same time
						query.exec();

						record = query.record();
						query.next();

						user.Id = query.value(record.indexOf("Id")).toInt();

						emit OnFinished(user);
						close();
					}
					qWarning() << query.lastError().text();
				}
				else
				{
					//work with errors
					qWarning() << query.lastError().text();
				}
			}
			else
			{
				QMessageBox::critical(this, "Error!", "Failed to create data base with this name! This name is already used!");
			}
		}
		else
		{
			QMessageBox::critical(this, "Error!", "All text fields must be written into");
		}
	}
}


TableCreationWindow::~TableCreationWindow()
{
}


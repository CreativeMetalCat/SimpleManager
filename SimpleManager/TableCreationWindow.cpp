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

			auto code = QString(QCryptographicHash::hash(ui.lineEdit_Code->text().toUtf8(),
				QCryptographicHash::Md5).toHex());
			if (query.exec("INSERT INTO TableSets (name,ControlCode) VALUES (\"" + ui.lineEdit_DBName->text() + "\",\""+ code+"\")"))
			{

				int TableSetId = 0;
				;
				if (query.exec("SELECT * FROM TableSets Where id = last_insert_rowid()"))
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

					user.IsAdmin = true;

					

					QSqlQuery query;
					//add defult admin role
					query.exec("INSERT INTO Roles (Name,TableSetId,Admin) VALUES ('Admin'," + QString::number(TableSetId) + ",1)");

					//read the new id. Because it's a new account there will be only one Role with that TableSetId
					query.exec("SELECT Id FROM Roles WHERE TableSetId = " + QString::number(TableSetId) + " AND Name = 'Admin'");
					query.next();
					int roleId = query.value(0).toInt();
					user.Roles.append(roleId);


					query.prepare("INSERT INTO Users (Name,RoleId,Password,ContactInfo,TableSetId) VALUES (:Name,:RoleId,:Password,:ContactInfo,:TableSetId);");
					query.bindValue(":Name", ui.lineEdit_Name->text());
					query.bindValue(":Password", QString(QCryptographicHash::hash(ui.lineEdit_Password->text().toUtf8(), QCryptographicHash::Md5).toHex()));
					//it's generated like that because the newly created db will only have on, all powerful, role
					query.bindValue(":RoleId", "'{\"roles\":["+ QString::number(roleId) + "]}'");
					query.bindValue(":ContactInfo", "");
					//assign user to be a part of newly created db
					query.bindValue(":TableSetId", TableSetId);
					query.exec();
					qWarning() << query.lastError().text();
					query.exec("SELECT * FROM Users Where id = last_insert_rowid()");
					query.next();
					
					record = query.record();

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
			QMessageBox::critical(this, "Error!", "All text fields must be written into");
		}
	}
}


TableCreationWindow::~TableCreationWindow()
{
}


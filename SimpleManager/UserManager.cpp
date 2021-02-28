#include "UserManager.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonArray>
#include <QPushButton>
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QGroupBox>
#include <QLineEdit>
#include <QIntValidator>

UserManager::UserManager(ManagerInfo::SUserInfo currentUserInfo, QSqlDatabase dataBase, QWidget *parent)
	: QWidget(parent),DataBase(dataBase),CurrentUserInfo(currentUserInfo)
{
	ui.setupUi(this);

	scrollWidget = new QWidget(this);
	scrollBox = new QVBoxLayout(scrollWidget);
	ui.scrollArea->setWidget(scrollWidget);

	if (DataBase.isOpen())
	{
		GenerateUserList();
	}

	connect(ui.button_AddUser, &QPushButton::clicked, this, &UserManager::ShowUserCreationWindow);

	connect(ui.button_Delete, &QPushButton::clicked, this, &UserManager::DeleteSelectedUsers);
}

void UserManager::ShowUserCreationWindow()
{
	//create user addition dialog
	QDialog* dialog = new QDialog(this);
	dialog->setFixedSize(200, 400);
	dialog->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);

	QVBoxLayout* mainLayout = new QVBoxLayout(dialog);
	dialog->setLayout(mainLayout);

	//user id input
	QLabel* label1 = new QLabel("User Id", dialog);
	QLineEdit* idInput = new QLineEdit(dialog);
	idInput->setValidator(new QIntValidator(0, 100, dialog));
	connect(idInput, &QLineEdit::textChanged, this, [this,dialog, idInput](QString newText)
	{
		dialog->setProperty("UserId", newText.toInt());
	});
	mainLayout->addWidget(label1);
	mainLayout->addWidget(idInput);

	//are where all roles will be displayed
	QScrollArea* area = new QScrollArea(dialog);
	QVBoxLayout* areaLayout = new QVBoxLayout(area);
	area->setLayout(areaLayout);

	mainLayout->addWidget(area);

	QGroupBox* roleBox = new QGroupBox("Roles", dialog);
	QVBoxLayout* roleBoxLayout = new QVBoxLayout(roleBox);
	roleBox->setLayout(roleBoxLayout);
	areaLayout->addWidget(roleBox);

	
	if (DataBase.isOpen())
	{
		QSqlQuery query;
		query.exec("SELECT Id,Name,Groups,PowerLevel FROM Roles WHERE TableSetId = " + QString::number(CurrentUserInfo.TableSetId));

		QSqlRecord record = query.record();
		while (query.next())
		{
			bool canBeDisplayed = false;
			QVector<QString> tableGroups = ManagerInfo::ConvertJsonStringToStringArray(query.value(record.indexOf("Groups")).toString(), "groups");
			for (auto it = tableGroups.begin(); it != tableGroups.end(); ++it)
			{
				int id;
				if (CurrentUserInfo.ContainsRoleFromGroup(*it, id))
				{
					if (CurrentUserInfo.RolesInfo[id].PowerLevel < query.value(record.indexOf("PowerLevel")).toInt())
					{
						canBeDisplayed = true;
						break;
					}
				}
			}
			if (canBeDisplayed)
			{
				int Id = query.value(record.indexOf("Id")).toInt();

				QWidget* role = new QWidget(this);
				QHBoxLayout* mainLayout = new QHBoxLayout(role);
				role->setLayout(mainLayout);

				QCheckBox* checkBox = new QCheckBox(role);
				mainLayout->addWidget(checkBox);

				QLabel* label = new QLabel(query.value(record.indexOf("Name")).toString(),this);
				mainLayout->addWidget(label);

				role->setProperty("Id", Id);
				connect(checkBox, &QCheckBox::clicked, this, [this, Id](bool state)
				{
					if (state && !this->RoleIds.contains(Id))
					{
						this->RoleIds.append(Id);
					}
					else if (this->RoleIds.contains(Id))
					{
						this->RoleIds.removeOne(Id);
					}
				});

				roleBoxLayout->addWidget(role);
			}
			
		}
	}

	QPushButton* button = new QPushButton("Add", dialog);
	mainLayout->addWidget(button);
	connect(button, &QPushButton::clicked, this, [this, button,dialog]()
	{
		QSqlQuery query;
		query.exec("SELECT TableSetId FROM Users WHERE Id = " + QString::number(dialog->property("UserId").toInt()));
		qWarning() << query.lastError().text();
		qWarning() << "Id: " + QString::number(dialog->property("UserId").toInt());
		if (query.next())
		{
			//check if user is already in the db. If it is not -> add them 
			if (!ManagerInfo::ConvertJsonStringToIntArray(query.value(0).toString(),"tables").contains(CurrentUserInfo.TableSetId))
			{
				AddUserToTable(dialog->property("UserId").toInt(), this->RoleIds);
			}
		}
		qWarning() << query.lastError().text();
		dialog->close();
	});
	dialog->exec();
}

UserManager::~UserManager()
{
}

void UserManager::GenerateUserList()
{
	QSqlQuery query;
	query.exec("SELECT Id,Name,RoleId,TableSetId FROM Users");
	QSqlRecord record = query.record();

	QJsonDocument doc;
	QJsonObject object;
	while (query.next())
	{
		auto uiui = ManagerInfo::ConvertJsonStringToIntArray(query.value(record.indexOf("TableSetId")).toString(),"tables");
		if (ManagerInfo::ConvertJsonStringToIntArray(query.value(record.indexOf("TableSetId")).toString(),
			"tables").contains(CurrentUserInfo.TableSetId))
		{
			auto roleStringBytes = query.value(record.indexOf("RoleId")).toString().remove("'").toUtf8();
			doc = QJsonDocument::fromJson(roleStringBytes);
			object = doc.object();
			// object["roles"]; <- this is the array we are looking for

			QVector<int>RoleIds = QVector<int>();

			
			//in case something went wrong when writting to the table
			if (object["roles"].isArray())
			{
				QJsonArray array = object["roles"].toArray();
				for (auto it = array.begin(); it != array.end(); ++it)
				{
					if ((*it).isDouble())
					{
						RoleIds.append((*it).toInt());
					}
				}
			}
			ManagerInfo::SUserInfo info;
			info.Id = query.value(record.indexOf("Id")).toInt();
			info.Name = query.value(record.indexOf("Name")).toString();
			UserManagerItem* item = new UserManagerItem(info,DataBase,this);

			scrollBox->addWidget(item);
		}
	}
}

void UserManager::DeleteSelectedUsers()
{

	auto items = scrollWidget->children();
	if (items.count() > 0)
	{
		//ask user if they really want to do that
		QMessageBox::StandardButton reply = QMessageBox::question(this, "Are you sure?", "You are going to delete " + QString::number(items.count()) + " users", QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::Yes)
		{
			//if user agreed -> delete them
			for (auto it = items.begin(); it != items.end(); ++it)
			{
				//if this is correct child
				if (UserManagerItem* item = qobject_cast<UserManagerItem*>(*it))
				{
					//if this is selected
					if (item->IsSelected())
					{
						//we delete it

						//delete from table
						if (DataBase.isOpen())
						{
							QSqlQuery query;
							if (!query.exec("DELETE FROM Users WHERE id = " + QString::number(item->UserInfo.Id)))
							{
								qWarning() << query.lastError().text();
							}
						}

						//delete object
						delete item;
					}
				}
			}
		}
	}
}

void UserManager::AddUserToTable(int userId, QVector<int> roles)
{
	if (DataBase.isOpen())
	{
		QSqlQuery query;
		query.exec("SELECT TableSetId FROM Users WHERE Id = " + QString::number(userId));
		qWarning() << query.lastError().text();
		if (query.next())
		{
			QJsonObject obj = QJsonDocument::fromJson(query.value(0).toString().toUtf8()).object();
			if (obj["tables"].isArray())
			{
				auto array = obj["tables"].toArray();
				array.append(CurrentUserInfo.TableSetId);
				obj["tables"] = array;
			}
			QJsonDocument doc = QJsonDocument(obj);
			query.exec("UPDATE Users SET TableSetId  = '" + QString(doc.toJson()) + "' WHERE Id = " + QString::number(userId));
			qWarning() << query.lastError().text();
		}
	}

}

void UserManager::WriteNewUser(ManagerInfo::SUserInfo userInfo)
{
	QString RoleString;
	for (auto it = userInfo.RolesInfo.begin(); it != userInfo.RolesInfo.end(); ++it)
	{
		RoleString += QString::number((*it).Id);
		//if there is still something else we have to append comma otherwise JSON file won't work
		if ((it + 1) != userInfo.RolesInfo.end())
		{
			RoleString += ",";
		}
	}

	//QString ContactInfoString;
	//for (auto it = userInfo.ContactInfo.begin(); it != userInfo.ContactInfo.end(); ++it)
	//{
	//	ContactInfoString += "\"" + (*it).TypeName + "\":\"" + (*it).Info + "\"";
	//	//if there is still something else we have to append comma otherwise JSON file won't work
	//	if ((it + 1) != userInfo.ContactInfo.end())
	//	{
	//		ContactInfoString += ",";
	//	}
	//}

	QSqlQuery query;
	query.prepare("INSERT INTO Users (Name,RoleId,Password,ContactInfo,TableSetId) VALUES (:Name,:RoleId,:Password,:ContactInfo,:TableSetId);");
	query.bindValue(":Name", userInfo.Name);
	query.bindValue(":Password", userInfo.Password);
	query.bindValue(":RoleId", "'{\"roles\":[" + RoleString + "]}'");
	query.bindValue(":ContactInfo", "{}");
	query.bindValue(":TableSetId", CurrentUserInfo.TableSetId);

	//this adds the user and returns the data at the same time
	if (query.exec())
	{
		query.exec("SELECT * FROM TableSets Where id = last_insert_rowid()");
		QSqlRecord record = query.record();
		query.next();
		userInfo.Id = query.value(record.indexOf("Id")).toInt();

		UserManagerItem* item = new UserManagerItem(userInfo, DataBase, this);
		scrollBox->addWidget(item);
	}
	else
	{
		QMessageBox::critical(this, "Error!", query.lastError().text());
	}
}

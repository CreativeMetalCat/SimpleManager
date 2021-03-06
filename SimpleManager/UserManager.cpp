#include "UserManager.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonArray>
#include <QPushButton>
#include "UserAdditionWindow.h"
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>


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
	UserAdditionWindow* userAdd = new UserAdditionWindow(this);
	userAdd->show();
	connect(userAdd, &UserAdditionWindow::OnUserCreationFinished, this, [this, userAdd]()
	{
		this->WriteNewUser(userAdd->Info);
		userAdd->close();
	});
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
		if (query.value(record.indexOf("TableSetId")).toInt() == CurrentUserInfo.TableSetId)
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
			info.Roles = RoleIds;
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

void UserManager::WriteNewUser(ManagerInfo::SUserInfo userInfo)
{
	QString RoleString;
	for (auto it = userInfo.Roles.begin(); it != userInfo.Roles.end(); ++it)
	{
		RoleString += QString::number(*it);
		//if there is still something else we have to append comma otherwise JSON file won't work
		if ((it + 1) != userInfo.Roles.end())
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

#include "UserManager.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonArray>
#include <QPushButton>
#include "UserAdditionWindow.h"
#include <QSqlRecord>


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
}

void UserManager::ShowUserCreationWindow()
{
	UserAdditionWindow* userAdd = new UserAdditionWindow(this);
	userAdd->show();
	connect(userAdd, &UserAdditionWindow::OnUserCreationFinished, this, &UserManager::WriteNewUser);
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
			doc = QJsonDocument::fromJson(query.value(record.indexOf("RoleId")).toByteArray());
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

			UserManagerItem* item = new UserManagerItem(
				{
				query.value(record.indexOf("Id")).toInt(),
				query.value(record.indexOf("Name")).toString(),
				RoleIds
				},
				DataBase,
				this);

			scrollBox->addWidget(item);
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

	QString ContactInfoString;
	QStringList keys = userInfo.ContactInfo.keys();
	for (auto it = keys.begin(); it != keys.end(); ++it)
	{
		ContactInfoString += "\"" + (*it) + "\":\"" + userInfo.ContactInfo.value((*it)).toString() + "\"";
		//if there is still something else we have to append comma otherwise JSON file won't work
		if ((it + 1) != keys.end())
		{
			ContactInfoString += ",";
		}
	}

	QSqlQuery query;
	query.prepare("INSERT INTO Users (Name,RoleId,Password,ContactInfo,TableSetId) VALUES (:Name,:RoleId,:Password,:ContactInfo,:TableSetId);SELECT * FROM TableSets Where id = last_insert_rowid()");
	query.bindValue(":Name", userInfo.Name);
	query.bindValue(":Password", userInfo.Password);
	query.bindValue(":RoleId","'{\"roles\":[" + RoleString + "]}'");
	query.bindValue(":ContactInfo", ContactInfoString);
	query.bindValue(":TableSetId", CurrentUserInfo.TableSetId);

	//this adds the user and returns the data at the same time
	query.exec();

	QSqlRecord record = query.record();
	query.next();
	userInfo.Id = query.value(record.indexOf("Id")).toInt();

	UserManagerItem* item = new UserManagerItem(userInfo, DataBase, this);
	scrollBox->addWidget(item);
}

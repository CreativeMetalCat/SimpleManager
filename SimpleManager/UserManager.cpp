#include "UserManager.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonArray>


UserManager::UserManager(QSqlDatabase dataBase, QWidget *parent)
	: QWidget(parent),DataBase(dataBase)
{
	ui.setupUi(this);

	scrollWidget = new QWidget(this);
	scrollBox = new QVBoxLayout(scrollWidget);
	ui.scrollArea->setWidget(scrollWidget);

	if (DataBase.isOpen())
	{
		GenerateUserList();
	}
}

UserManager::~UserManager()
{
}

void UserManager::GenerateUserList()
{
	QSqlQuery query;
	query.exec("SELECT Id,Name,RoleId FROM Users");
	QSqlRecord record = query.record();

	QJsonDocument doc;
	QJsonObject object;
	while (query.next())
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
			this);

		scrollBox->addWidget(item);
	}
}

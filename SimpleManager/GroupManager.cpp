#include "GroupManager.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QCheckBox>
#include <QLayout>
#include <QInputDialog>
#include <QSqlError>
#include <QMessageBox>

GroupManager::GroupManager(ManagerInfo::SUserInfo currentUserInfo, QSqlDatabase dataBase, QWidget* parent)
	: QWidget(parent), DataBase(dataBase), CurrentUserInfo(currentUserInfo)
{
	ui.setupUi(this);

	scrollWidget = new QWidget(this);
	scrollBox = new QVBoxLayout(scrollWidget);
	ui.scrollArea->setWidget(scrollWidget);

	GenerateItems();

	connect(ui.button_Delete, &QPushButton::clicked, this, &GroupManager::DeleteGroups);

	connect(ui.button_AddUser, &QPushButton::clicked, this, &GroupManager::AddGroup);
}

void GroupManager::DeleteGroups() 
{
	//record all of the ids we need to delete
	QVector<int>idsToDelete = QVector<int>();
	QVector<QString> namesToDelete = QVector<QString>();
	//yes, i'm aware that Qt has QMap. But i don't trust it
	for (auto it = Items.begin(); it != Items.end(); ++it)
	{
		if ((*it)->property("Checked").toBool())
		{
			//because we add them together they can not go out of sync
			idsToDelete.append((*it)->property("Id").toInt());
			namesToDelete.append((*it)->property("Name").toString());
		}
	}
	//delete ids from table and group names from roles
	QSqlQuery query;
	for (auto it = idsToDelete.begin(); it != idsToDelete.end(); ++it)
	{
		//delete from table
		query.exec("DELETE FROM Groups WHERE Id = " + QString::number(*it));		
	}
	//we have to go thru each role from this table set and remove any mention of the 
	query.exec("SELECT Groups,Id FROM Roles WHERE TableSetId = " + QString::number(CurrentUserInfo.TableSetId));
	QSqlRecord record = query.record();
	while (query.next())
	{
		int id = query.value(record.indexOf("Id")).toInt();
		//read group of every role and clean it from wrong names
		QJsonObject obj = QJsonDocument::fromJson(query.value(record.indexOf("Groups")).toString().toUtf8()).object();
		if (obj["groups"].isArray())
		{
			auto array = obj["groups"].toArray();
			for (auto it = array.begin(); it != array.end() && !array.isEmpty(); ++it)
			{			
				for (auto i = namesToDelete.begin(); i != namesToDelete.end(); ++i)
				{
					if ((*i) == (*it).toString())
					{
						array.removeAt(it.i);
					}
				}
			}
			//array is copy of the original. so we just relpace the original with copy
			obj["groups"] = array;
		}

		
		QJsonDocument doc = QJsonDocument(obj);
		QSqlQuery writeQuery;
		writeQuery.exec("UPDATE Roles SET Groups  = '" + QString(doc.toJson()) + "' WHERE Id = " + QString::number(id));
	}

	GenerateItems();
}

GroupManager::~GroupManager()
{
}

void GroupManager::AddGroup()
{
	QString name = QInputDialog::getText(this, "Enter New Name", "");
	QSqlQuery query;
	query.exec("SELECT Id FROM GROUPS WHERE Name = " + name + " AND TableSetId = " + QString::number(CurrentUserInfo.TableSetId));
	if (!query.next())
	{
		query.prepare("INSERT INTO Groups (Name,TableSetId) VALUES(:Name,:TableSetId)");
		query.bindValue(":Name", name);
		query.bindValue(":TableSetId", QString::number(CurrentUserInfo.TableSetId));
		query.exec();

		GenerateItems();
	}
	else 
	{
		QMessageBox::critical(this, "Error!", "Groups with this name already exists in this Database\n Id = " + 
		query.value(0).toString());
	}

}

void GroupManager::GenerateItems()
{
	if (!Items.isEmpty())
	{		
		for (auto it = Items.begin(); it != Items.end(); ++it)
		{
			(*it)->deleteLater();
		}
		Items.clear();
	}

	if (DataBase.isOpen())
	{
		QSqlQuery query;
		query.exec("SELECT Id,Name FROM Groups WHERE TableSetId = " + QString::number(CurrentUserInfo.TableSetId));
		QSqlRecord record = query.record();
		qWarning() << query.lastError().text();
		while (query.next())
		{
			QWidget* item = new QWidget(this);

			QHBoxLayout* layout = new QHBoxLayout(item);
			item->setLayout(layout);

			//set id for future use(in deletion actions)
			item->setProperty("Id", query.value(record.indexOf("Id")).toInt());
			//set name for future use where you don't need to cast to object
			item->setProperty("Name", query.value(record.indexOf("Name")).toString());

			//check box will be used for deletion
			QCheckBox* checkBox = new QCheckBox(item);
			layout->addWidget(checkBox);

			QString name = query.value(record.indexOf("Name")).toString();
			int id = query.value(record.indexOf("Id")).toInt();

			QPushButton* nameButton = new QPushButton(name, item);
			layout->addWidget(nameButton);
			connect(nameButton, &QPushButton::clicked, this, [this, nameButton,name, id]()
			{
				//update name of the group
				nameButton->setText(QInputDialog::getText(this, "Enter new name", name));
				if (name != nameButton->text())
				{
					QSqlQuery query;
					query.exec("UPDATE Groups SET Name = \"" + nameButton->text() + "\" WHERE Id = " + QString::number(id));
				}
			});

			//bind on checked event to set value in widget itself to avoid having to search in chilren later
			connect(checkBox, &QCheckBox::clicked, this, [item](bool state)
			{
				item->setProperty("Checked", state);
			});

			Items.append(item);

			scrollBox->addWidget(item);
		}
	}
}

#include "RoleManagerWindow.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QLabel>
#include <QLayout>
#include <QCheckBox>
#include <QSqlError>
#include <QDebug>

RoleManagerWindow::RoleManagerWindow(ManagerInfo::SUserInfo currentUserInfo, QSqlDatabase dataBase, QWidget *parent)
	: QWidget(parent), DataBase(dataBase), CurrentUserInfo(currentUserInfo)
{
	ui.setupUi(this);

	scrollWidget = new QWidget(this);
	scrollBox = new QVBoxLayout(scrollWidget);
	ui.scrollArea->setWidget(scrollWidget);

	if (DataBase.isOpen())
	{
		GenerateRoleList();
	}
	connect(ui.button_Delete, &QPushButton::clicked, this, &RoleManagerWindow::DeleteRoles);
}


void RoleManagerWindow::DeleteRoles() 
{
	if (!Items.isEmpty())
	{
		QSqlQuery query;
		query.exec("SELECT Name FROM TableSets WHERE Id = " + QString::number(CurrentUserInfo.TableSetId));
		query.next();
		QString TableSetName = query.value(0).toString();
		for (auto it = Items.begin(); it != Items.end(); ++it)
		{
			if ((*it)->property("Checked").toBool())
			{
				if (query.exec("DELETE FROM Roles WHERE Id = " + QString::number((*it)->property("RoleId").toInt())))
				{
					Items.removeOne(*it);
					delete (*it);
				}
				else
				{
					qWarning() << query.lastError().text();
				}
			}
		}
	}
}

void RoleManagerWindow::GenerateRoleList()
{
	QSqlQuery query;
	QString TableSetName;
	//Get table set name
	

		query.exec("SELECT Name,Id FROM Roles WHERE TableSetId = " + QString::number(CurrentUserInfo.TableSetId));
		QSqlRecord record = query.record();
		while (query.next())
		{
			QWidget* item = new QWidget(this);

			QHBoxLayout* Layout = new QHBoxLayout(item);
			QLabel* Name = new QLabel(query.value(record.indexOf("Name")).toString(), item);
			QCheckBox* CheckBox = new QCheckBox(item);

			Layout->addWidget(Name);
			Layout->addWidget(CheckBox);

			scrollBox->addWidget(item);
			Items.append(item);

			item->setProperty("RoleId", query.value(record.indexOf("Id")).toInt());

			connect(CheckBox, &QCheckBox::stateChanged, this, [item, CheckBox]()
			{
				item->setProperty("Checked", CheckBox->isChecked());
			});
		}
	
}


RoleManagerWindow::~RoleManagerWindow()
{
}

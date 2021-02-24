#include "RoleManagerWindow.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QLabel>
#include <QLayout>
#include <QCheckBox>
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
}

RoleManagerWindow::~RoleManagerWindow()
{
}

void RoleManagerWindow::GenerateRoleList()
{
	QSqlQuery query;
	QString TableSetName;
	//Get table set name
	if (query.exec("SELECT Name FROM TableSets WHERE Id = " + QString::number(CurrentUserInfo.TableSetId)))
	{
		query.next();
		TableSetName = query.value(0).toString();

		query.exec("SELECT Name FROM Roles_" + TableSetName);
		while (query.next())
		{
			QWidget* item = new QWidget(this);

			QHBoxLayout* Layout = new QHBoxLayout(item);
			QLabel* Name = new QLabel(query.value(0).toString(), item);
			QCheckBox* CheckBox = new QCheckBox(item);

			Layout->addWidget(Name);
			Layout->addWidget(CheckBox);

			scrollBox->addWidget(item);
			Items.append(item);
		}
	}
}

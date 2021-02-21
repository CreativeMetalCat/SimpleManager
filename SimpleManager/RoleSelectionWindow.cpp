#include "RoleSelectionWindow.h"
#include <QCheckBox>
#include <QDebug>

RoleSelectionWindow::RoleSelectionWindow(SUserInfo user, QSqlDatabase dataBase, QWidget *parent)
	: QDialog(parent),UserInfo(user),Roles(user.Roles)
{
	ui.setupUi(this);

	scrollWidget = new QWidget(this);
	scrollBox = new QVBoxLayout(scrollWidget);
	ui.scrollArea->setWidget(scrollWidget);

	if (!dataBase.isOpen())
	{
		//database connection wasn't established
		//try that now

		dataBase.open();
	}
	GenerateRoleList();

	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &RoleSelectionWindow::AcceptChanges);

	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, [this]() {close(); });
}

void RoleSelectionWindow::AcceptChanges()
{
	Roles = QVector<int>();
	for (auto it = Checkboxes.begin(); it != Checkboxes.end(); ++it)
	{
		if ((*it)->isChecked())
		{
			Roles.append((*it)->property("Id").toInt());
		}
	}

	emit OnRolesChosen(Roles);
	close();
}

RoleSelectionWindow::~RoleSelectionWindow()
{
}


void RoleSelectionWindow::GenerateRoleList()
{

	QSqlQuery query;
	query.exec("SELECT Name,Id FROM Roles");
	QSqlRecord record = query.record();
	while (query.next())
	{
		int id = query.value(record.indexOf("Id")).toInt();
		QCheckBox* checkBox = new QCheckBox(query.value(record.indexOf("Name")).toString(), this);
		checkBox->setProperty("Id", id);

		checkBox->setChecked(UserInfo.Roles.indexOf(id) != -1);
		

		scrollBox->addWidget(checkBox);

		Checkboxes.append(checkBox);
	}
}

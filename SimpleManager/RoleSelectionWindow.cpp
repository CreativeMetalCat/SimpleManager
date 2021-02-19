#include "RoleSelectionWindow.h"
#include <QCheckBox>

RoleSelectionWindow::RoleSelectionWindow(QSqlDatabase dataBase, QWidget *parent)
	: QDialog(parent)
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
}

RoleSelectionWindow::~RoleSelectionWindow()
{
}

void RoleSelectionWindow::GenerateRoleList()
{
	QSqlQuery query;
	query.exec("SELECT Name FROM Roles");
	while (query.next())
	{
		QCheckBox* checkBox = new QCheckBox(query.value(0).toString(), this);
		scrollBox->addWidget(checkBox);
	}
}

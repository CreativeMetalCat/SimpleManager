#include "TableCreationWindow.h"
#include <QLayout>

TableCreationWindow::TableCreationWindow(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	scrollWidget = new QWidget(this);
	scrollBox = new QVBoxLayout(scrollWidget);
	ui.scrollArea->setWidget(scrollWidget);

	connect(ui.button_AddRole, &QPushButton::clicked, this, &TableCreationWindow::AddNewRoleEdit);
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

	connect(deleteButton, &QPushButton::clicked, this, [this, roleEdit]
	{
		Roles.removeOne(roleEdit);
		delete roleEdit;
	});
}

TableCreationWindow::~TableCreationWindow()
{
}

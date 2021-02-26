#include "TableSettingsTab.h"
#include < QCryptographicHash>
#include <QInputDialog>
#include <QSqlQuery>
#include <QMessageBox>

TableSettingsTab::TableSettingsTab(ManagerInfo::SUserInfo currentUserInfo, QSqlDatabase dataBase, QWidget *parent)
	: QWidget(parent), DataBase(dataBase), CurrentUserInfo(currentUserInfo)
{
	ui.setupUi(this);

	connect(ui.button_Delete, &QPushButton::clicked, this, &TableSettingsTab::Delete);
}

void TableSettingsTab::Delete()
{
	QString password = QString(QCryptographicHash::hash(QInputDialog::getText(this, "Confirmation Needed", "Enter Conformation Code").toUtf8(), 
		QCryptographicHash::Md5).toHex());
	QSqlQuery query;

	//read the code from table
	query.exec("SELECT ControlCode FROM TableSets WHERE Id = " + QString::number(CurrentUserInfo.TableSetId));
	if (query.next())
	{
		//check if code is correct
		if (query.value(0).toString() == password)
		{
			//start deletion sequence
			emit OnTableSetDeleted();
		}
		else
		{
			QMessageBox::critical(this, "Error!", "Incorrect Control Code");
		}
	}
}

TableSettingsTab::~TableSettingsTab()
{
}

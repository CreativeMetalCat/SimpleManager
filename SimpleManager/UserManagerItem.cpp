#include "UserManagerItem.h"
#include "RoleSelectionWindow.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QSqlError>
#include <QMenu>
#include <QContextMenuEvent>

UserManagerItem::UserManagerItem(SUserInfo userInfo, QSqlDatabase dataBase,QWidget *parent)
	: QWidget(parent),UserInfo(userInfo), DataBase(dataBase)
{
	ui.setupUi(this);

	ui.label_UserName->setText(UserInfo.Name);
	for (auto it = UserInfo.Roles.begin(); it != UserInfo.Roles.end(); ++it)
	{
		ui.label_Roles->setText(ui.label_Roles->text() + QString::number((*it)));
	}
	ui.label_Roles->installEventFilter(this);
	ui.label_id->setText(QString::number(UserInfo.Id));
}



void UserManagerItem::contextMenuEvent(QContextMenuEvent* event)
{
	QMenu menu(this);
	//add actions here
	menu.exec(event->globalPos());
}

bool UserManagerItem::IsSelected()
{
	return ui.checkBox->isChecked();
}

void UserManagerItem::RecordRoleChanges(QVector<int> roles)
{
	UserInfo.Roles = roles;
	QString result;
	QSqlQuery query;
	for (auto it = roles.begin(); it != roles.end(); ++it)
	{
		result += QString::number(*it);
		if ((it + 1) != roles.end())
		{
			result += ",";
		}
	}
	qWarning() << result;
	if (!query.exec("UPDATE Users SET RoleId='{\"roles\":[" + result + "]}' WHERE Id = " + QString::number(UserInfo.Id)))
	{
		qWarning() << query.lastError().text();
		if (query.lastError().type() == QSqlError::ConnectionError && !DataBase.isOpen())
		{
			DataBase.open();
			RecordRoleChanges(UserInfo.Roles);
		}
	}
}

bool UserManagerItem::eventFilter(QObject* object, QEvent* event)
{
	if (object == ui.label_Roles)
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			RoleSelectionWindow* roleSelect = new  RoleSelectionWindow(UserInfo, DataBase, this);
			connect(roleSelect, &RoleSelectionWindow::OnRolesChosen, this, &UserManagerItem::RecordRoleChanges);
			roleSelect->show();

		}
	}
	return false;
}

UserManagerItem::~UserManagerItem()
{
}
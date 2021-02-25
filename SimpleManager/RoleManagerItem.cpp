#include "RoleManagerItem.h"
#include <QSqlQuery>

RoleManagerItem::RoleManagerItem(ManagerInfo::SUserInfo currentUserInfo, QSqlDatabase dataBase, QWidget *parent)
	: QWidget(parent), DataBase(dataBase), CurrentUserInfo(currentUserInfo)
{
	ui.setupUi(this);

}

RoleManagerItem::~RoleManagerItem()
{
}

void RoleManagerItem::GenerateList()
{
	if (DataBase.isOpen())
	{
		QSqlQuery query;
		
	}
}

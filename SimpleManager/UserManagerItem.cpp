#include "UserManagerItem.h"

UserManagerItem::UserManagerItem(SUserInfo userInfo, QWidget *parent)
	: QWidget(parent),UserInfo(userInfo)
{
	ui.setupUi(this);

	ui.label_UserName->setText(UserInfo.Name);
	for (auto it = UserInfo.Roles.begin(); it != UserInfo.Roles.end(); ++it)
	{
		ui.label_Roles->setText(ui.label_Roles->text() + QString::number((*it)));
	}
	
}

UserManagerItem::~UserManagerItem()
{
}

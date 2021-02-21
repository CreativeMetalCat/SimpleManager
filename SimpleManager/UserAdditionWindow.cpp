#include "UserAdditionWindow.h"
#include <QCryptographicHash>

using namespace ManagerInfo;

UserAdditionWindow::UserAdditionWindow(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &UserAdditionWindow::CreateUserInfo);

	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, [this]() {close(); });
}

UserAdditionWindow::~UserAdditionWindow()
{
}

void UserAdditionWindow::CreateUserInfo()
{
	SUserInfo info;
	/*
	* info.Id and info.TableSetId will be set by the class that has direct access to database and user info, based on that info
	*/
	info.Name = ui.lineEdit_Name->text();

	//we convert password into encrypted string so we would not write password in the table
	info.Password = QString(QCryptographicHash::hash(ui.lineEdit_Password->text().toUtf8(), QCryptographicHash::Md5).toHex());

	info.ContactInfo["email"] = "nothing@fakeserver.fake";

	emit OnUserCreationFinished(info);

	close();
}

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
	/*
	* info.Id and info.TableSetId will be set by the class that has direct access to database and user info, based on that info
	*/
	Info.Name = ui.lineEdit_Name->text();

	//we convert password into encrypted string so we would not write password in the table
	Info.Password = QString(QCryptographicHash::hash(ui.lineEdit_Password->text().toUtf8(), QCryptographicHash::Md5).toHex());

	//Info.ContactInfo.append({ "email","nothing@fakeserver.fake" });

	emit OnUserCreationFinished();
}

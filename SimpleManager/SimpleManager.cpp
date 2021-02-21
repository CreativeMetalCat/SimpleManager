#include "SimpleManager.h"
#include "RoleSelectionWindow.h"
#include "UserManager.h"
#include "UserLogInDialog.h"
#include "Info.h"

SimpleManager::SimpleManager(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QSqlDatabase Database = QSqlDatabase::addDatabase("QSQLITE");
    Database.setDatabaseName("Users.db");
    if (Database.open())
    {
        UserLogInDialog* dialog = new UserLogInDialog(Database, this);
        dialog->show();
        connect(dialog, &UserLogInDialog::OnLogInSuccessful, this, [this](ManagerInfo::SUserInfo info)
        {
            qWarning() << info.Password;
        });
        UserManager* selection = new UserManager(Database, this);
        selection->show();
    }
}
